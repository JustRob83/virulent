/*! \file pwmdetail.c
 *
 */

#include "ch.h"
#include "hal.h"

#include "pwm_lld.h"

#define PWM_CH2ONLY	{PWM_OUTPUT_DISABLED, NULL},	\
					{PWM_OUTPUT_ACTIVE_HIGH, NULL},	\
					{PWM_OUTPUT_DISABLED, NULL},	\
					{PWM_OUTPUT_DISABLED, NULL},



PWMConfig pwmcfg2 = {
		1000000, /* 1Mhz PWM clock frequency */
		500,   /* PWM period 0.5 millisecond */
		NULL,    /* No callback */
		/* Channel 2 enabled */
		{
				PWM_CH2ONLY
		},
		0,
		0,
};
PWMConfig pwmcfg3 = {
		1000000, /* 1Mhz PWM clock frequency */
		333,   /* PWM period 0.333 millisecond */
		NULL,    /* No callback */
		/* Channel 2 enabled */
		{
				PWM_CH2ONLY
		},
		0,
		0,
};
PWMConfig pwmcfg5 = {
		1000000, /* 1Mhz PWM clock frequency */
		200,   /* PWM period 0.2 millisecond */
		NULL,    /* No callback */
		/* Channel 2 enabled */
		{
				PWM_CH2ONLY
		},
		0,
		0,
};
