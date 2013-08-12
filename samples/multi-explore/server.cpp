#include "server.hpp"



tiley::StdTile PlainTile(std::wstring(L"plain"));
tiley::StdTile PlayerTile(std::wstring(L"player"));
tiley::StdTile TreeTile(std::wstring(L"tree"));
tiley::StdTile WaterTile(std::wstring(L"water"));
tiley::StdTile EmptyTile(std::wstring(L"empty"));


Participant::Participant(Map& map, const std::shared_ptr<tiley::Participant>& p )
	: mMap(map)
	, mLayer(tiley::Rect(21, 21))
	, mHandle(p)
{
	mLayer.autoFree(false);

	mPosition = mMap.freeRandPos();
	mMap.togglePlayer(mPosition);
	p->attach(&mLayer);

	draw();
}


Participant::~Participant()
{

}

bool Participant::move( const tiley::Point& dir )
{
	auto newP = mPosition + dir;

	if(mMap.tile(newP) == Map::Plain)
	{
		mMap.togglePlayer(mPosition);
		mPosition = newP;
		mMap.togglePlayer(mPosition);
		draw();

		return true;
	}
	return false;
}

void Participant::someone_moved( const tiley::Point& from, const tiley::Point& to )
{
	tiley::Point offset(mLayer.size()/2);

	tiley::Context<TNTILE> ctx(mLayer);
	ctx.set(PlainTile(), from - position() + offset);
	ctx.set(PlayerTile(), to - position() + offset);
}

void Participant::draw()
{
	auto size = mLayer.size();

	tiley::Point offset(size/2);

	tiley::Context<TNTILE> ctx(mLayer);

	for(int y = 0; y < size.h; ++y)
	{
		for(int x = 0; x < size.w; ++x)
		{
			tiley::Point p(x,y);
			auto type = mMap.tile(mPosition + (p - offset));
			TNTILE tile;

			switch (type)
			{
			case Map::Empty:
				tile = EmptyTile();
				break;
			case Map::Plain:
				tile = PlainTile();
				break;
			case Map::Tree:
				tile = TreeTile();
				break;
			case Map::Water:
				tile = WaterTile();
				break;
			case Map::Player:
				tile = PlayerTile();
				break;
			default:
				break;
			}
			ctx.set(tile, p);
		}
	}
}

const tiley::Point& Participant::position() const
{
	return mPosition;
}






MEServer::MEServer(unsigned short port )
	: mParticipants(128, nullptr)
	, mMap(tiley::Rect(100, 100))
{
	tiley::ServerConfig<> config(L"multi-explore");
	config.description(L"Explore a simple map with friends!");
	config.package(L"multi-explore-sample-package");
	config.interface(L"tn.org.samples.multi-explore");

	mServer.reset(new tiley::Server<>(config));
	mListenAcceptor.reset(new tiley::ListenAcceptor(*mServer, port));
}

MEServer::~MEServer()
{
	mListenAcceptor.reset();
	mServer.reset();
}

void MEServer::run()
{
	while(true)
	{
		TNEVENT evt;
		while(mServer->fetch(evt))
		{
			handle_events(evt);
		}
	}
}

void MEServer::handle_events( TNEVENT& evt )
{
	TNPARTICIPANT index = TNEXTRACTID(evt.participant);
	switch(evt.type)
	{
	case TNEV_CONNECT:
		assert(!mParticipants[index]);
		mParticipants[index] = new Participant(mMap, mServer->participant(evt.participant));

		break;


	case TNEV_DISCONNECT:
		assert(mParticipants[index]);
		delete mParticipants[index];
		mParticipants[index] = nullptr;
		break;

	case TNEV_TXT:
		{
			wchar_t c = evt.data.txtevent.ch;
			tiley::Point dir;

			switch(c)
			{
			case L'w':	dir.y = -1; break;
			case L's':	dir.y = 1; break;
			case L'a':	dir.x = -1; break;
			case L'd':	dir.x = 1; break;
			default:
				return;
			}
			auto& ply = mParticipants[index];
			auto from = ply->position();

			if(ply->move(dir))
			{
				auto to = ply->position();

				for(auto& p : mParticipants)
				{
					if(p && p != ply)
						p->someone_moved(from, to);
				}
			}

		}
		break;

	default:
		break;
	}
}
