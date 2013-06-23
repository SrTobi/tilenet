#include "includes.hpp"

#include "utils/field.hpp"


#include "main_com_handler_v1_0.hpp"
#include "renderer_v1_0.hpp"
#include "tile_mapper_v1_0.hpp"

#include "client/client.hpp"

#include "network/message.hpp"

namespace client {
namespace com {
namespace v1_0 {



class MainComHandler::TileLayerCommitManager
{
	typedef std::map<TNID, Delta> History;
	struct CommitBuffer{ CommitBuffer():nextDelta(1){} TNID nextDelta; History history; };
public:
	TileLayerCommitManager()
	{
	}

	~TileLayerCommitManager()
	{
	}

	std::vector<Delta> addDelta(TNID layer, TNID commitnr, Delta&& dt)
	{
		auto& b = mCommitBuffers[layer];

		std::vector<Delta> result;
		if(commitnr == b.nextDelta)
		{
			++b.nextDelta;
			result.push_back(std::move(dt));
			addNextCommits(b, result);
		}else{
			b.history[commitnr] = std::move(dt);
		}

		return result;
	}

	std::vector<Delta> notifyFullLayerCommit(TNID layer, TNID commitnr)
	{
		std::vector<Delta> result;
		auto& b = mCommitBuffers[layer];
		auto& h = b.history;

		// erase all commits older then the new fill commit
		for(auto it = h.begin(); it != h.end();)
		{
			if(it->first <= commitnr)
			{
				it = h.erase(it);
			}else
				++it;
		}

		b.nextDelta = commitnr + 1;

		addNextCommits(b, result);
		return std::move(result);
	}

private:
	void addNextCommits(CommitBuffer& b, std::vector<Delta>& result)
	{
		for(auto it = b.history.find(b.nextDelta);
			it != b.history.end() && it->first == b.nextDelta;
			++b.nextDelta)
		{
			result.push_back(std::move(it->second));
			it = b.history.erase(it);
		}
	}

	CommitBuffer* buffer(TNID layer)
	{
		auto it = mCommitBuffers.find(layer);

		return it == mCommitBuffers.end()? nullptr : &it->second;
	}
private:
	std::unordered_map<TNID, CommitBuffer> mCommitBuffers;
};



MainComHandler::MainComHandler( ClientApp& app, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<ServerInfo>& svr_info)
	: mApp(app)
	, mPort(port)
	, mServerInfo(svr_info)
	, mCommitManager(new TileLayerCommitManager())
{
	mDispatcher.add(&MainComHandler::handleLayerControl_attachLayer, this);
	mDispatcher.add(&MainComHandler::handleLayerControl_sendFullLayer, this);
	mDispatcher.add(&MainComHandler::handleLayerControl_sendLayerUpdate, this);
	mDispatcher.add(&MainComHandler::handleAnswer_StdTileNameRequest, this);
	mDispatcher.add(&MainComHandler::handleLayerControl_sendFrame, this);
}

void MainComHandler::init()
{
	mTileMapper = std::make_shared<TileMapper>(mPort);
	mRenderer = std::make_shared<Renderer>(mTileMapper, mApp.pmanager(), mServerInfo);
}



MainComHandler::~MainComHandler()
{
}

OVERRIDE shared_ptr<ComHandler> MainComHandler::handleMessage(const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return shared_ptr<ComHandler>();
}

void MainComHandler::handleLayerControl_attachLayer( proto::v1_0::to_client::LayerControl_AttachLayer& msg )
{
	mRenderer->setTopLayer(msg.layerId);
}


void MainComHandler::handleLayerControl_sendLayerUpdate( proto::v1_0::to_client::LayerControl_SendLayerUpdate& msg )
{
	auto deltas = mCommitManager->addDelta(msg.layerId, msg.commitNr, std::move(msg));

	for(auto& dt : deltas)
		mRenderer->applyDelta(dt);
}



void MainComHandler::handleLayerControl_sendFullLayer(proto::v1_0::to_client::LayerControl_SendFullLayer& msg )
{
	// Todo: Check values
	Rect size(msg.width, msg.height);
	Ratio ratio(msg.xratio, msg.yratio);

	mRenderer->defineLayer(msg.layerId, size, ratio);

	// Check content size
	if(msg.layerContent.size() != size.area())
	{
		NOT_IMPLEMENTED();
	}

	// get deltas waiting to be put
	auto deltas = mCommitManager->notifyFullLayerCommit(msg.layerId, msg.commitNr);

	mRenderer->updateLayer(msg);

	for(auto& dt : deltas)
		mRenderer->applyDelta(dt);
}


void MainComHandler::handleLayerControl_sendFrame( proto::v1_0::to_client::LayerControl_SendFrame& msg )
{
	mRenderer->updateFrame(msg);
}



shared_ptr<MainComHandler> MainComHandler::Create( ClientApp& app, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<ServerInfo>& svr_info)
{
	shared_ptr<MainComHandler> ptr(new MainComHandler(app, port, svr_info));
	ptr->init();
	return ptr;
}

void MainComHandler::requestStdTileName(TNID tile_id)
{
	if(mRequestedStdTiles.count(tile_id) != 0)
		return;
	mRequestedStdTiles.insert(tile_id);

	proto::v1_0::to_srv::Request_StdTileName req;

	req.tileId = tile_id;

	mPort->send(net::make_message(req));
}

void MainComHandler::handleAnswer_StdTileNameRequest( proto::v1_0::to_client::Answer_StdTileNameRequest& answ )
{
	mTileMapper->identifyStdTile(answ.tileName, answ.tileId);
}

shared_ptr<ComInterface> MainComHandler::getComInterface()
{
	return std::static_pointer_cast<MainComHandler>(shared_from_this());
}

void MainComHandler::notifyRender( sf::RenderTarget& target )
{
	mRenderer->render(target);
}

void MainComHandler::notifyKeyevent( const sf::Event& event )
{
	typedef proto::v1_0::to_srv::Control_KeyEvent KeyEvent;

	KeyEvent ke;
	ke.type = (event.type == sf::Event::KeyPressed? KeyEvent::Evt_KeyDown : KeyEvent::Evt_KeyUp );
	ke.data.key = proto::v1_0::KeyMapper::Inst().toCode(event.key.code);
	ke.data.modifier = (event.key.alt? TNKM_ALT : 0)
					| (event.key.control? TNKM_CONTROL : 0)
					| (event.key.shift? TNKM_SHIFT : 0)
					| (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)? TNKM_ALTGR : 0);

	mPort->send(net::make_message(ke));
}












}}}