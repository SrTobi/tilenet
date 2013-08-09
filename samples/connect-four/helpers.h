#ifndef _HELPERS_H
#define _HELPERS_H

#include <tilenet.h>
#include "con4.h"

typedef struct {

	TNLAYER handle;

	TNTILE* content;
	TNBOOL* changes;

	unsigned int w, h;

} Layer;


#define COLOR_WHITE (0xFFFFFFFF)
#define COLOR_LGREY (0xBBBBBBBB)
#define COLOR_RED	(0xFFFF0000)
#define COLOR_GREEN	(0xFF00FF00)
#define COLOR_BLUE	(0xFF0000FF)


Layer* create_layer(unsigned int w, unsigned int h, const wchar_t* aspect);
void destroy_layer(Layer* layer);

size_t pos_index(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void clear_layer(Layer* layer);
void set_tile(Layer* layer, unsigned int x, unsigned int y, TNTILE* tile);
void set_nulltile(Layer* layer, unsigned int x, unsigned int y);
void set_stdtile(Layer* layer, unsigned int x, unsigned int y, TNID tileid, TNCOLOR color);
void set_chartile(Layer* layer, unsigned int x, unsigned int y, wchar_t ch, TNCOLOR color);
void print_chars(Layer* layer, unsigned int x, unsigned int y, TNCOLOR color, const wchar_t* txt, ...);
void flush_layer(Layer* layer);

TNID get_tileid(const wchar_t* name);

#endif