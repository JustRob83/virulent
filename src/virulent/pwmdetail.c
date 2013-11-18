/*! \file pwmdetail.c
 *
 */

#include "ch.h"
#include "hal.h"
#include "pwmdetail.h"

#include "pwm_lld.h"

#define PWM_CH4ONLY	{						\
					{PWM_OUTPUT_DISABLED, NULL},	\
					{PWM_OUTPUT_DISABLED, NULL},	\
					{PWM_OUTPUT_DISABLED, NULL},	\
					{PWM_OUTPUT_ACTIVE_HIGH, NULL},	\
					}



PWMConfig pwmcfgRed = {
		1000000, /* 1Mhz PWM clock frequency */
		RED_PERIOD,   /* PWM period for Red Team */
		NULL,    /* No callback */
		/* Channel 4 enabled */
		PWM_CH4ONLY,
		0,
		0,
};
PWMConfig pwmcfgGreen = {
		1000000, /* 1Mhz PWM clock frequency */
		GREEN_PERIOD,   /* PWM period for Green Team */
		NULL,    /* No callback */
		/* Channel 4 enabled */
		PWM_CH4ONLY,
		0,
		0,
};
PWMConfig pwmcfgBlue = {
		1000000, /* 1Mhz PWM clock frequency */
		BLUE_PERIOD,   /* PWM period for Blue Team */
		NULL,    /* No callback */
		/* Channel 4 enabled */
		PWM_CH4ONLY,
		0,
		0,
};
