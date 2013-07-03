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


Layer* create_layer(unsigned int w, unsigned int h);
void destroy_layer(Layer* layer);

void set_tile(Layer* layer, unsigned int x, unsigned int y, TNTILE* tile);
void set_stdtile(Layer* layer, unsigned int x, unsigned int y, TNID tileid, TNCOLOR color);
void set_chartile(Layer* layer, unsigned int x, unsigned int y, wchar_t ch, TNCOLOR color);
void print(Layer* layer, unsigned int x, unsigned y, const wchar_t* txt, TNCOLOR color);




#endif