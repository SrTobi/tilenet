#include <iostream>
#include <cstring>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
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
		tilenet_create_tilelayer(&m_handle, w, h, TNSTDRATIO, TNSTDRATIO, L"", 0);
	}

	~Layer()
	{
		tilenet_destroy(m_handle);
	}

	void print(unsigned int x, unsigned int y, const std::wstring& txt, TNCOLOR color = COL_WHITE)
	{
		unsigned int start_x = x;
		for(auto c : txt)
		{
			if(c == L'\n')
			{
				x = start_x;
				++y;

				if(y >= m_height)
					return;

			}else if(x < m_width)
			{
				TNTILE t;
				t.type = TN_CHAR_TILE;
				t.data.charset.ch = c;
				t.data.charset.color = color;
				t.data.charset.modifier = 0;
				tile(x,y) = t;
				++x;
			}
		}
	}

	const TNTILE& tile(unsigned int x, unsigned int y) const
	{
		return tile(TNPOS_INDEX(x, y, m_width));
	}

	TNTILE& tile(unsigned int x, unsigned int y)
	{
		return tile(TNPOS_INDEX(x, y, m_width));
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
		std::fill(m_updated.begin(), m_updated.end(), 0);
	}

	void attachTo(TNPARTICIPANT p)
	{
		tilenet_attach_layer(p, m_handle);
	}

	unsigned int width() const { return m_width; }
	unsigned int height() const { return m_height;}

	TNLAYER handle() const { return m_handle; }

private:
	const unsigned int m_width, m_height;
	TNLAYER m_handle;
	std::vector<TNTILE> m_content;
	std::vector<TNBOOL> m_updated;
};

class Frame
{
public:
	Frame()
	{
		tilenet_create_frame(&m_handle, L"", 0);
	}

	~Frame()
	{
		tilenet_destroy(m_handle);
	}


	void attachTo(TNPARTICIPANT p)
	{
		tilenet_attach_layer(p, m_handle);
	}

	void addChild(Layer* layer, unsigned int x, unsigned int y, TNCOLOR color = COL_WHITE)
	{
		layers.push_back(layer->handle());

		TNVIEW view;

		view.color = color;
		view.flags = 0;

		view.outter_x = x;
		view.outter_y = y;
		view.outter_w = layer->width();
		view.outter_h = layer->height();
		
		view.outter_xr = TNSTDRATIO;
		view.outter_yr = TNSTDRATIO;
		view.outter_wr = TNSTDRATIO;
		view.outter_hr = TNSTDRATIO;

		view.inner_x = 0;
		view.inner_y = 0;
		view.inner_w = layer->width();
		view.inner_h = layer->height();

		view.inner_xr = TNSTDRATIO;
		view.inner_yr = TNSTDRATIO;
		view.inner_wr = TNSTDRATIO;
		view.inner_hr = TNSTDRATIO;

		std::vector<TNVIEW*> viewstack(layers.size(), nullptr);
		viewstack.back() = &view;

		tilenet_update_frame(m_handle, layers.data(), viewstack.data(), layers.size());
	}

private:
	std::vector<TNLAYER> layers;
	TNLAYER m_handle;
};

#define LAYER_W	10
#define LAYER_H 10

TNSERVER srv;
StdTile a_tile(L"test-a");
Layer* layer1;
Layer* layer2;
Frame* frame;
TNTILE* layerContent;

TNACCEPTOR listenAcceptor;

void init_testlayer()
{
	layer1 = new Layer(10, 10);

	
	layer1->tile(0,0) = a_tile();
	layer1->tile(1,0) = a_tile(TNMAKE_COLOR(0xff, 0, 0xff, 0));
	layer1->tile(9,9) = a_tile(TNMAKE_COLOR(0xff, 0xff, 0, 0));
	layer1->print(2, 5, L"Hello\nwie\ngehts?");
	layer1->flush();


	layer2 = new Layer(10, 10);


	layer2->tile(0,0) = a_tile();
	layer2->tile(1,0) = a_tile(TNMAKE_COLOR(0xff, 0, 0xff, 0));
	layer2->tile(9,9) = a_tile(TNMAKE_COLOR(0xff, 0xff, 0, 0));
	layer2->flush();

	frame = new Frame();
	frame->addChild(layer1, 0, 0);
	frame->addChild(layer2, 15, 0, 0xffff8888);
}

unsigned int pos = 0;

void update_layer()
{
	layer1->tile(pos++ % (90) + 10) = a_tile(TNMAKE_COLOR(0xff, rand() % 0xff, rand() % 0xff, rand() % 0xff));
	layer1->flush();

	layer2->tile(rand() % 100) = a_tile(TNMAKE_COLOR(0xff, 0xaa, 0xaa, rand() % 0xff));
	layer2->flush();
}

void do_event(TNEVENT& e)
{
	switch(e.type)
	{
	case TNEV_CONNECT:
		std::cout << "Player connected!\n";

		frame->attachTo(e.participant);
		break;
	case TNEV_DISCONNECT:
		std::cout << "Player disconnected!\n";
		break;
	case TNEV_TXT:
		std::wcout << L"Text event[" << e.data.txtevent.ch << "]";
		layer1->print(3, 0, std::wstring(1, e.data.txtevent.ch));
		layer1->flush();
		break;

	case TNEV_KEYDOWN:
		{
			TNKEYCODE key = e.data.keyevent.key;
			const wchar_t* name = L"";
			tilenet_keyname(key, &name);
			std::wcout << "Keydown[" << std::wstring(name) << "]\n";


			TNKEYCODE enter_code = 0;
			tilenet_keycode(L"enter", &enter_code);
			if(enter_code == key)
			{
				std::wifstream file("ip.txt");

				if(file.is_open())
				{
					std::wstring str;
					file >> std::noskipws;
					std::getline(file, str);
					str = L"@!!!link:" + str;
					tilenet_kick(e.participant, str.c_str());
				}
			}


			TNKEYCODE escape_code = 0;
			tilenet_keycode(L"esc", &escape_code);
			if(escape_code == key)
			{
				delete layer1;
				delete layer2;
				delete frame;


				tilenet_kick(e.participant, L"I don't like you!!!");
				tilenet_exit(0);
				exit(0);
			}


		}break;
	default:
		break;
	}
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
	init_testlayer();


	tilenet_create_server(&srv, &config);
	tilenet_add_local_acceptor(0, srv);
	
	if(tilenet_add_listen_acceptor(&listenAcceptor, srv, 2165, 16) > TNOK)
	{
		std::cout << L"Acceptor could not be created!\n";
	}

	size_t timeout = 2000;
	while(true)
	{
		TNEVENT events[EVENT_BUF_LENGTH];

		size_t fetched = 0;
		if(timeout > 0)
			tilenet_fetch_events(srv, events, EVENT_BUF_LENGTH, &fetched, &timeout);

		if(fetched)
		{
			for(size_t i = 0; i < fetched; ++i)
			{
				do_event(events[i]);
			}
		}else{
			timeout = 2000;
			update_layer();
		}

	}



	std::cin.get();

	tilenet_exit(0);
	return 0;
}
