#pragma once
#ifndef _TILENET_DEBUG_H
#define _TILENET_DEBUG_H


#include "tilenet.h"


#ifdef __cplusplus
extern "C" {
#endif

TNAPI TNERROR dtilenet_available();
TNAPI TNERROR dtilenet_reset_error_state(TNERROR code);




#ifdef __cplusplus
}
#endif



#endif