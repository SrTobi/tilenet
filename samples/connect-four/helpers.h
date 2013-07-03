#ifndef _HELPERS_H
#define _HELPERS_H

#include <tilenet.h>
#include "4con.h"

typedef struct {

	TNLAYER handle;

	TNTILE* content;
	TNBOOL* changes;

	unsigned int w, h;

} Layer;


#define COLOR_WHITE (0xFFFFFFFF)
#define COLOR_LGREY (0xBBBBBBBB)


Layer* create_layer(unsigned int w, unsigned int h);
void destroy_layer(Layer* layer);

size_t pos_index(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void clear_layer(Layer* layer);
void set_tile(Layer* layer, unsigned int x, unsigned int y, TNTILE* tile);
void set_stdtile(Layer* layer, unsigned int x, unsigned int y, TNID tileid, TNCOLOR color);
void set_chartile(Layer* layer, unsigned int x, unsigned int y, wchar_t ch, TNCOLOR color);
void print_chars(Layer* layer, unsigned int x, unsigned int y, TNCOLOR color, const wchar_t* txt, ...);
void flush_layer(Layer* layer);


#endif