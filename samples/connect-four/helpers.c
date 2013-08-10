#include "helpers.h"

#include <stdarg.h>


Layer* create_layer( unsigned int w, unsigned int h, const wchar_t* aspect)
{
	Layer* layer = (Layer*)malloc(sizeof(Layer));

	layer->content = (TNTILE*)malloc(sizeof(TNTILE) * w * h);
	layer->changes = (TNBOOL*)malloc(sizeof(TNTILE) * w * h);
	layer->w = w;
	layer->h = h;

	tilenet_create_tilelayer(&(layer->handle), w, h, TNSTDRATIO, TNSTDRATIO, aspect, 0);
	clear_layer(layer);

	return layer;
}

void destroy_layer( Layer* layer )
{
	tilenet_destroy(layer->handle);
	free(layer->content);
	free(layer->changes);
	free(layer);
}

void clear_layer( Layer* layer)
{
	memset(layer->changes, 1, sizeof(TNBOOL) * layer->w * layer->h);
	memset(layer->content, 0, sizeof(TNTILE) * layer->w * layer->h);
}


size_t pos_index( unsigned int x, unsigned int y, unsigned int w, unsigned int h )
{
	assert(	0 <= x && x < w &&
			0 <= y && y < h);
	return TNPOS_INDEX(x, y, w);
}


void set_tile( Layer* layer, unsigned int x, unsigned int y, TNTILE* tile )
{
	assert(layer);

	if( 0 <= x && x < layer->w &&
		0 <= y && y < layer->h)
	{
		size_t idx = pos_index(x, y, layer->w, layer->h);
		layer->changes[idx] = 1;
		layer->content[idx] = *tile;
	}
}

void set_nulltile( Layer* layer, unsigned int x, unsigned int y )
{
	TNTILE tile;
	memset(&tile, 0, sizeof(tile));
	set_tile(layer, x, y, &tile);
}


void set_stdtile( Layer* layer, unsigned int x, unsigned int y, TNID tileid, TNCOLOR color )
{
	TNTILE tile;
	tile.type = TN_STD_TILE;
	tile.data.stdset.id = tileid;
	tile.data.stdset.color = color;
	tile.data.stdset.modifier = 0;

	set_tile(layer, x, y, &tile);
}

void set_chartile( Layer* layer, unsigned int x, unsigned int y, wchar_t ch, TNCOLOR color )
{
	TNTILE tile;
	tile.type = TN_CHAR_TILE;
	tile.data.charset.ch = ch;
	tile.data.charset.color = color;
	tile.data.charset.modifier = 0;

	set_tile(layer, x, y, &tile);
}

void print_chars( Layer* layer, unsigned int x, unsigned int y, TNCOLOR color, const wchar_t* txt, ...)
{
#define PRINT_CHARS_BUF_SIZE 1024
	wchar_t buf[PRINT_CHARS_BUF_SIZE];

	unsigned int cur_x = x;
	unsigned int cur_y = y;

	int len;
	int idx;

	va_list args;
	va_start(args, txt);

	len = vswprintf(buf, PRINT_CHARS_BUF_SIZE, txt, args);


	// print to layer
	for(idx = 0; idx < len; ++idx)
	{
		wchar_t c = buf[idx];

		assert(c);
		switch(c)
		{
		case L'\n':
			++cur_y;
			cur_x = x;
			break;

		case L'\t':
			cur_x += 4;
			break;

		default:
			set_chartile(layer, cur_x, cur_y, c, color);
			++cur_x;
			break;
		}
	}


	va_end(args);
}

void flush_layer( Layer* layer )
{
	tilenet_update_tilelayer(layer->handle, layer->content, layer->changes);
}

TNID get_tileid( const wchar_t* name )
{
	TNID id;

	tilenet_stdtile(name, &id);

	return id;
}
