#include "includes.h"
#include "service/netservice.h"
#include "service/pending.h"
#include "service/participant.h"

#include "server/layer.h"
#include "server/display.h"
#include "server/server.h"

namespace service {

NetService::NetService(const TilenetConfig& config, server::TilenetServer* server)
	:	mConfig(config),
		mServer(server),
		mAcceptor(mIOService),
		mRunning(false),
		mPointSwapBuffer(&mPointBuffer)
{
	SLOG(Construct, L"Construct NetService");
}

NetService::~NetService()
{
}



bool NetService::init()
{
	SLOG(Info, L"Init Netservice...");
	if(mConfig.useServerNetwork)
	{
		// Init Mainserver or connect
	}else{
		mServerPort = mConfig.portBind;
	}

	bool randomPort = false;
	if(mServerPort == 0)
		randomPort = true;

	//TODO Hier muessen fehler abgefangen werden und neue versuche gewagt werden

	asio::tcp::endpoint endpoint(asio::tcp::v6(), mServerPort);
	mAcceptor.open(asio::tcp::v6());
	error_code err;

	// Nun maximal 10 mal versuchen den Port zu binden
	for (int i = 0; i < 10; ++i)
	{
		if(randomPort)
			mServerPort = rand() % USHRT_MAX;
		endpoint.port(mServerPort);

		SLOG(Info, L"Bind TcpListener to port " << mServerPort);
		mAcceptor.bind(endpoint, err);

		if(!err)
			break;
	}

	if(err)
	{
		SLOG(Error, L"Not able to bind TcpLisener on port " << mServerPort << L" [" << err << L"]");
		throw tn::TnException(L"Can not bind!");
	}

	//HACK: Sollte eingestellt werden können
	mAcceptor.listen(100);
	SLOG(Info, L"TcpListener is listening to port " << port());

	return true;
}


void NetService::start()
{
	if(mRunning)
		return;

	init();

	SLOG(Info, L"Start Netservice in second thread!");
	mServiceThread = boost::thread(boost::bind(&NetService::run, this));
}

void NetService::run()
{
	mRunning = true;
	
	// Starte asyncrone operationen
	startAsyncAccept();

	mIOService.run();
}



void NetService::startAsyncAccept()
{
	socket_ptr sock(new asio::tcp::socket(mIOService));
	mAcceptor.async_accept(*sock,
							boost::bind(&NetService::handleAsyncAccept, this,
										sock, asio::placeholders::error)
								);
}


void NetService::handleAsyncAccept(socket_ptr socket, const error_code& err)
{
	if(!err)
	{
		// Einfach einen neuen Pending einfügen
		SLOG(Info, L"New connection accepted! add to pending...");
		auto pending = make_shared<PendingConnection>(socket, this);

		pending->sendWelcomeHandshake();
		pending->startReceiving();
	}

	startAsyncAccept();
}


uint NetService::participantCount() const
{
	syncronize<const decltype(mParticipants)> participants(mParticipants);
	return participants->size();
}


shared_ptr<Participant> NetService::participant(uint p)
{
	syncronize<decltype(mParticipants)> participants(mParticipants);
	shared_ptr<Participant> ret;

	participants->get(p, ret);

	return ret;
}

shared_ptr<Participant> NetService::createParticipant(const socket_ptr& socket)
{
	syncronize<decltype(mParticipants)> participants(mParticipants);
	int id = participants->nextId();
	shared_ptr<Participant> p = make_shared<Participant>(socket, this, id);

	participants->add(p);

	mServer->injectEvent_NewParticipant(p);

	return p;
}

shared_ptr<Participant> NetService::eraseParticipant(uint p)
{
	syncronize<decltype(mParticipants)> participants(mParticipants);
	shared_ptr<Participant> ret;
	participants->get(p, ret);

	if(!ret)
		return shared_ptr<Participant>();

	ret->closeSocket();

	participants->erase(p);
	return ret;
}


void NetService::start_LayerPropertiesTransmittion_broadcast(const shared_ptr<server::Layer>& layer)
{
	handleLayerPropertieTransmittion(layer, shared_ptr<asio::deadline_timer>());
}

void NetService::start_LayerTransmittion_broadcast(	const shared_ptr<server::Layer>& layer,
													ptime::duration wait)
{
	auto timer = make_shared<asio::deadline_timer>(mIOService, wait);
	timer->async_wait(boost::bind(&NetService::handleLayerTransmittion, this, layer, timer));
}

void NetService::start_FullLayerTransmittion_broadcast(	const shared_ptr<server::Layer>& layer,
														ptime::duration wait)
{
	auto timer = make_shared<asio::deadline_timer>(mIOService, wait);
	timer->async_wait(boost::bind(&NetService::handleFullLayerTransmittion, this, layer, timer));
}


void NetService::start_fullLayerTransmittion_to_participant(const shared_ptr<server::Layer>& layer,
															const shared_ptr<Participant>& partitipant,
															ptime::duration wait /*= ptime::seconds(0)*/)
{
	auto timer = make_shared<asio::deadline_timer>(mIOService, wait);
	timer->async_wait(boost::bind(&NetService::handleFullLayerTransmittion_to_participant, this, layer, partitipant, timer));
}

void NetService::handleLayerPropertieTransmittion(	const shared_ptr<server::Layer>& layer,
													const shared_ptr<asio::deadline_timer>& timer)
{
	net::OBuffer pack(net::msg::ToClient::SendLayerProperties);
	layer->gatherProperties(pack);

	sendToDisplay(layer->holdingDisplay(), pack);
}


void NetService::handleLayerTransmittion(	const shared_ptr<server::Layer>& layer,
											const shared_ptr<asio::deadline_timer>& timer)
{
	tool::PointComparator comparator;
	mPointSwapBuffer = layer->swapBuffer(mPointSwapBuffer);

	// das duerfte eigentlich nicht passieren, aber wenn brauchen wir nichts zu machen
	if(!mPointSwapBuffer->size())
		return;

	sf::Clock clock;
	clock.restart();
	
	SLOG(Info, L"Before sorting: " << mPointSwapBuffer->size());

	//TODO:
	// Bei size >> width*height * 2 sollte einfach das ganze layer uebertragen werden!

	Rect size = layer->size();
	if(mPointSwapBuffer->size() * 3 > uint(size.area()) * 2)
	{
		handleFullLayerTransmittion(layer, shared_ptr<asio::deadline_timer>());
		return;
	}



	// Sortieren, damit alles besser verschickt werden kann
	std::sort(mPointSwapBuffer->begin(), mPointSwapBuffer->end(), comparator);
	std::cout << "Sortieren: " << clock.getElapsedTime().asMilliseconds() << std::endl; 
	clock.restart();
	auto endIt = std::unique(mPointSwapBuffer->begin(), mPointSwapBuffer->end());
	
	
	std::cout << "Zusammenschieben: " << clock.getElapsedTime().asMilliseconds() << std::endl; 

	nuint count = endIt - mPointSwapBuffer->begin();
	//SLOG(Info, L"After sorting: " << count);

	//TODO:
	// Bei cout >= width*height sollte einfach das ganze layer uebertragen werden!

	net::OBuffer pack(net::msg::ToClient::SendUpdatedLayerContent);
	net::Protocol<net::msg::ToClient::SendUpdatedLayerContent>::Inject(pack, layer->id(), count);
	
	
	clock.restart();
	const server::Layer::TileField& tf = layer->tileField();
	Point startPoint(0,0);
	Point* last = &startPoint;

	for(auto it = mPointSwapBuffer->begin();
		it != endIt;
		++it)
	{
		Point& p = *it;
		if(p.x >= 0 && p.y >= 0 && p.x < size.w && p.y < size.h)
		{
			if(p.y == last->y)
			{
				pack << nuint(p.x - last->x);
			}else{
				pack << nuint( (size.w - last->x) + (p.y - last->y - 1)*size.w + p.x);
			}
			const server::Layer::Tile& t = tf.at(p.x).at(p.y);
			pack << t.tile << t.color;

			last = &p;
		}
	}
	std::cout << "Verpacken: " << clock.getElapsedTime().asMilliseconds() << std::endl; 
	clock.restart();
	
	mPointSwapBuffer->clear();
	std::cout << "Loeschen: " << clock.getElapsedTime().asMilliseconds() << std::endl; 
	clock.restart();

	// Und absenden
	sendToDisplay(layer->holdingDisplay(), pack);
	std::cout << "Senden: " << clock.getElapsedTime().asMilliseconds() << std::endl; 
	clock.restart();
}

void NetService::handleFullLayerTransmittion(	const shared_ptr<server::Layer>& layer,
												const shared_ptr<asio::deadline_timer>& timer)
{
	net::OBuffer pack(net::msg::ToClient::SendFullLayerContent);

	buildFullLayerTransmittionPackage(layer, pack);

	sendToDisplay(layer->holdingDisplay(), pack);

}

void NetService::handleFullLayerTransmittion_to_participant(const shared_ptr<server::Layer>& layer,
															const shared_ptr<Participant>& participant,
															const shared_ptr<asio::deadline_timer>& timer )
{
	net::OBuffer pack(net::msg::ToClient::SendFullLayerContent);

	buildFullLayerTransmittionPackage(layer, pack);

	net::send(participant->socket(), pack);
}


void NetService::fullDisplayTransmittion(const shared_ptr<server::Display>& display)
{

	if(!display)
		return;

	shared_ptr<asio::deadline_timer> dummy_timer_ptr;

	server::AbstractLayer* end = display->end();
	for(server::AbstractLayer* it = display->topLayer();
		it != end;
		it = it->below())
	{
		server::Layer* layer = it->toLayer();

		net::OBuffer pack(net::msg::ToClient::SendLayerProperties);
		layer->gatherProperties(pack);
		sendToDisplay(display.get(), pack);

		start_FullLayerTransmittion_broadcast(layer->shared_from_derived_this());
	}
}

void NetService::fullDisplayTransmittion_to_partitipant(const shared_ptr<Participant>& partitipant)
{
	socket_ptr socket = partitipant->socket();
	server::Display* display = partitipant->getDisplay();

	if(!display)
		return;

	shared_ptr<asio::deadline_timer> dummy_timer_ptr;

	server::AbstractLayer* end = display->end();
	for(server::AbstractLayer* it = display->topLayer();
		it != end;
		it = it->below())
	{
		server::Layer* layer = it->toLayer();

		net::OBuffer pack(net::msg::ToClient::SendLayerProperties);
		layer->gatherProperties(pack);
		net::send(socket, pack);

		start_fullLayerTransmittion_to_participant(layer->shared_from_derived_this(), partitipant);
	}

		

}

void NetService::buildFullLayerTransmittionPackage( const shared_ptr<server::Layer>& layer, net::OBuffer& pack)
{
	typedef server::Layer::TileField TF;
	const TF& tf = layer->tileField();

	Rect size = layer->size();

	net::Protocol<net::msg::ToClient::SendFullLayerContent>::Inject(pack, layer->id(), size.area());

	for(auto itx = tf.begin();
		itx != tf.end();
		++itx)
	{
		for(auto ity = itx->begin();
			ity != itx->end();
			++ity)
		{
			pack << ity->tile << ity->color;
		}
	}
}


void NetService::sendToDisplay(server::Display* dispaly, const net::OBuffer& pack)
{
	std::vector< shared_ptr<service::Participant> > participants;
	dispaly->getParticipants(participants);

	for(auto it = participants.begin();
		it != participants.end();
		++it)
	{
		if(*it)
		{
			net::send((*it)->socket(), pack);
		}
	}
}


}
