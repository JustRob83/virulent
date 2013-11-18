/*! \file pwmdetail.h
 *
 */


#ifndef _PWMDETAIL_H
#define _PWMDETAIL_H

#include "ch.h"
#include "hal.h"

#define RED_PERIOD	1504
#define GREEN_PERIOD	875
#define BLUE_PERIOD	586

extern PWMConfig pwmcfgRed;
extern PWMConfig pwmcfgGreen;
extern PWMConfig pwmcfgBlue;

#endif
