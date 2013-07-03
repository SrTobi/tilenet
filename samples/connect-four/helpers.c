#include "helpers.h"




Layer* create_layer( unsigned int w, unsigned int h )
{
	Layer* layer = (Layer*)malloc(sizeof(Layer));

	layer->content = (TNTILE*)malloc(sizeof(TNTILE) * w * h);
	layer->changes = (TNBOOL*)malloc(sizeof(TNTILE) * w * h);
	layer->w = w;
	layer->h = h;

	tilenet_create_tilelayer(&(layer->handle), w, h, TNSTDRATIO, TNSTDRATIO, 0);

	return layer;
}

void destroy_layer( Layer* layer )
{
	tilenet_destroy(layer->handle);
	free(layer->content);
	free(layer->changes);
	free(layer);
}

void set_tile( Layer* layer, unsigned int x, unsigned int y, TNTILE* tile )
{

}

void set_stdtile( Layer* layer, unsigned int x, unsigned int y, TNID tileid, TNCOLOR color )
{

}

void set_chartile( Layer* layer, unsigned int x, unsigned int y, wchar_t ch, TNCOLOR color )
{

}

void print( Layer* layer, unsigned int x, unsigned y, const wchar_t* txt, TNCOLOR color )
{

}
