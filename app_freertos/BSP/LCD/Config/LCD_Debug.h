/*****************************************************************************
* | File      	:	Debug.h
* | Author      :   Waveshare team
* | Function    :	debug with prntf
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stdio.h"
#include "elog.h"

#define DEV_DEBUG 1
#define DEV_DEBUG_USE_ELOG  1
#if DEV_DEBUG
#if DEV_DEBUG_USE_ELOG
#define Debug(__info, ...) elog_i("LCD", __info,##__VA_ARGS__)
#else
#define Debug(__info, ...) printf("LCD:" __info,##__VA_ARGS__)
#endif
#else
#define Debug(__info, ...)
#endif

#endif

