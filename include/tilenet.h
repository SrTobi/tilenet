#pragma once
#ifndef _TILENET_H
#define _TILENET_H


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TILENET_
#define TILENET_MAKE_HANDLE(_name) typedef struct { tilenet_idtype id; } _name;

typedef unsigned long tilenet_idtype;







#ifdef __cplusplus
}
#endif

#endif