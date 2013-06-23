#include <iostream>
#include <cstring>
#include <string>
#include <memory>
#include <vector>
#include "tilenet.h"


#define EVENT_BUF_LENGTH 100
#define COL_WHITE 0xFFFFFFFF


class StdTile
{
public:
	StdTile(const std::wstring& name)
	{
		tilenet_stdtile(name.c_str(), &id);
	}

	TNTILE operator ()() const
	{
		return (*this)(COL_WHITE);
	}

	TNTILE operator ()(TNCOLOR color) const
	{
		TNTILE tile;
		tile.type = TN_STD_TILE;
		tile.data.stdset.id = id;
		tile.data.stdset.color = color;
		tile.data.stdset.modifier = 0;

		return tile;
	}

private:
	TNID id;
};

class Layer
{
public:
	Layer(unsigned int w, unsigned int h)
		: m_width(w), m_height(h)
		, m_content(w * h), m_updated(w * h, 0)
	{
		tilenet_create_tilelayer(&m_handle, w, h, TNSTDRATIO, TNSTDRATIO, 0);
	}

	const TNTILE& tile(unsigned int x, unsigned int y) const
	{
		return tile(y * m_width + x);
	}

	TNTILE& tile(unsigned int x, unsigned int y)
	{
		return tile(y * m_width + x);
	}

	const TNTILE& tile(unsigned int pos) const
	{
		return m_content[pos];
	}

	TNTILE& tile(unsigned int pos)
	{
		m_updated[pos] = 1;
		return m_content[pos];
	}

	void flush()
	{
		tilenet_update_tilelayer(m_handle, m_content.data(), m_updated.data());
		memset(m_updated.data(), 0, m_width * m_height * sizeof(TNTILE));
	}

	void attachTo(TNPARTICIPANT p)
	{
		tilenet_attach_layer(p, m_handle);
	}

	unsigned int width() const { return m_width; }
	unsigned int height() const { return m_height; }

private:
	const unsigned int m_width, m_height;
	TNLAYER m_handle;
	std::vector<TNTILE> m_content;
	std::vector<TNBOOL> m_updated;
};



#define LAYER_W	10
#define LAYER_H 10

TNSERVER srv;
StdTile a_tile(L"test-a");
Layer* layer;
TNTILE* layerContent;


void init_testlayer()
{
	layer = new Layer(10, 10);

	
	layer->tile(0,0) = a_tile();
	layer->tile(1,0) = a_tile(TNMAKE_COLOR(0xff, 0, 0xff, 0));
	layer->tile(9,9) = a_tile(TNMAKE_COLOR(0xff, 0xff, 0, 0));
	layer->flush();
}

unsigned int pos = 0;

void update_layer()
{
	layer->tile(pos++ % (90) + 10) = a_tile(TNMAKE_COLOR(0xff, rand() % 0xff, rand() % 0xff, rand() % 0xff));
	layer->flush();
}

void do_event(TNEVENT& e)
{
	/*switch(e.type)
	{
	case TNEV_CONNECT:
		std::cout << "Player connected!\n";

		layer->attachTo(e.participant);
		break;
	case TNEV_DISCONNECT:
		std::cout << "Player disconnected!\n";
	case TNEV_KEYDOWN:
		{
			TNKEYCODE key = e.data.keyevent.key;
			const wchar_t* name = L"";
			tilenet_keyname(key, &name);
			std::wcout << "Keydown[" << std::wstring(name) << "]\n";


			TNKEYCODE escape_code = 0;
			tilenet_keycode(L"esc", &escape_code);
			if(escape_code == key)
			{
				tilenet_kick(e.participant, L"");
				tilenet_exit(0);
				exit(0);
			}


		}break;
	default:
		break;
	}*/
}



int main()
{
	TNSVRCONFIG config;
	config.name = L"test";
	config.info = L"simple tilenet test";
	config.pkg = L"test-pack";
	config.pkgi = L"tn.org.samples.test";
	config.options = 0;

	tilenet_set_service_thread_count(1);
	/*init_testlayer();


	tilenet_create_server(&srv, &config);
	tilenet_add_local_acceptor(0, srv);


	while(true)
	{
		TNEVENT events[EVENT_BUF_LENGTH];

		size_t fetched;
		size_t timeout = 100;
		tilenet_fetch_events(srv, events, EVENT_BUF_LENGTH, &fetched, &timeout);

		if(fetched)
		{
			for(size_t i = 0; i < fetched; ++i)
			{
				do_event(events[i]);
			}
		}else{
			update_layer();
		}

	}
	*/



	std::cin.get();
	return 0;
}
