/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "pwmdetail.h"
#include "board.h"

//STM32F0-DISCOVERY Board Assignments
#ifdef BOARD_ST_STM32F0_DISCOVERY

#define GPIO_TEAMID0            GPIOA_PIN11
#define GPIO_TEAMID1            GPIOA_PIN12
#define GPIO_MOD_FREQ		GPIOA_PIN3
#define GPIO_TX                 GPIOA_PIN4

#define GPIO_RED_LED		GPIOA_PIN6
#define GPIO_GREEN_LED		GPIOA_PIN7
#define GPIO_BLUE_LED		GPIOA_PIN5

#define GPIO_RED_PLL            GPIOB_PIN5
#define GPIO_GREEN_PLL          GPIOB_PIN6
#define GPIO_BLUE_PLL           GPIOB_PIN7

#endif

#define PLLR_EXTCHAN            5
#define PLLG_EXTCHAN            6
#define PLLB_EXTCHAN            7

static void transmit_unlock(GPTDriver *gptp);
static void transmit_stop(GPTDriver *gptp);
static void extReset(EXTDriver *extp, expchannel_t channel);
static void extPLLR(EXTDriver *extp, expchannel_t channel);
static void extPLLG(EXTDriver *extp, expchannel_t channel);
static void extPLLB(EXTDriver *extp, expchannel_t channel);

static const GPTConfig gpt1cfg = {
  1000,			//1kHz timer frequency
  transmit_stop,
  0
};

static const GPTConfig gpt3cfg = {
  1000,			//1kHz timer frequency
  transmit_unlock,
  0
};

const int BLUETEAM = 0;
const int GREENTEAM = 1;
const int REDTEAM = 2;

const int TRANSMIT_TIME = 1000;         //Transmit time in milliseconds.
const int TRANSMIT_LOCKOUT = 4000;      //Transmit lockout time in milliseconds.

static int DefaultTeam = 0;             //
static int CurrentTeam = 0;


/*
 * Blue LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker1");
  while (TRUE) {
    palClearPad(GPIOC, GPIOC_LED4);
    chThdSleepMilliseconds(300);
    palSetPad(GPIOC, GPIOC_LED4);
    chThdSleepMilliseconds(300);
  }
}

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker2");
  while (TRUE) {
    palClearPad(GPIOC, GPIOC_LED3);
    chThdSleepMilliseconds(10);
    palSetPad(GPIOC, GPIOC_LED3);
    chThdSleepMilliseconds(30);
  }
}

static void extReset(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  CurrentTeam = DefaultTeam;

  //Turn on appropriate team LED
  if(CurrentTeam == REDTEAM) {
    //Disable further PLLR interrupts
    extChannelDisableI(&EXTD1, PLLR_EXTCHAN);
    //Enable PLLG, PLLB interrupts
    extChannelEnableI(&EXTD1, PLLG_EXTCHAN);
    extChannelEnableI(&EXTD1, PLLB_EXTCHAN);
    pwmChangePeriodI(&PWMD2, RED_PERIOD);
    palClearPad(GPIOA, GPIO_RED_LED);
    palSetPad(GPIOA, GPIO_GREEN_LED);
    palSetPad(GPIOA, GPIO_BLUE_LED);
  }
  else if(CurrentTeam == GREENTEAM) {
    //Disable further PLLG interrupts
    extChannelDisableI(&EXTD1, PLLG_EXTCHAN);
    //Enable PLLR, PLLB interrupts
    extChannelEnableI(&EXTD1, PLLR_EXTCHAN);
    extChannelEnableI(&EXTD1, PLLB_EXTCHAN);
    pwmChangePeriodI(&PWMD2, GREEN_PERIOD);
    palClearPad(GPIOA, GPIO_GREEN_LED);
    palSetPad(GPIOA, GPIO_RED_LED);
    palSetPad(GPIOA, GPIO_BLUE_LED);
  }
  else {        //Blue Team
    //Disable further PLLB interrupts
    extChannelDisableI(&EXTD1, PLLB_EXTCHAN);
    //Enable PLLG, PLLR interrupts
    extChannelEnableI(&EXTD1, PLLG_EXTCHAN);
    extChannelEnableI(&EXTD1, PLLR_EXTCHAN);
    pwmChangePeriodI(&PWMD2, BLUE_PERIOD);
    palClearPad(GPIOA, GPIO_BLUE_LED);
    palSetPad(GPIOA, GPIO_RED_LED);
    palSetPad(GPIOA, GPIO_GREEN_LED);
  }

  chSysUnlockFromIsr();
}

static void extPLLR(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  //Disable further PLLR interrupts
  extChannelDisableI(&EXTD1, PLLR_EXTCHAN);
  //Enable PLLG, PLLB interrupts
  extChannelEnableI(&EXTD1, PLLG_EXTCHAN);
  extChannelEnableI(&EXTD1, PLLB_EXTCHAN);

  //Change PWM frequency to PLLR Frequency
  pwmChangePeriodI(&PWMD2, RED_PERIOD);
  //Turn on Red LED, turn off Green, Blue LED's
  palClearPad(GPIOA, GPIO_RED_LED);
  palSetPad(GPIOA, GPIO_GREEN_LED);
  palSetPad(GPIOA, GPIO_BLUE_LED);

  chSysUnlockFromIsr();
}

static void extPLLG(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  //Disable further PLLG interrupts
  extChannelDisableI(&EXTD1, PLLG_EXTCHAN);
  //Enable PLLR, PLLB interrupts
  extChannelEnableI(&EXTD1, PLLR_EXTCHAN);
  extChannelEnableI(&EXTD1, PLLB_EXTCHAN);

  //Change PWM frequency to PLLG Frequency
  pwmChangePeriodI(&PWMD2, GREEN_PERIOD);
  //Turn on Green LED, turn off Red, Blue LED's
  palClearPad(GPIOA, GPIO_GREEN_LED);
  palSetPad(GPIOA, GPIO_RED_LED);
  palSetPad(GPIOA, GPIO_BLUE_LED);

  chSysUnlockFromIsr();
}

static void extPLLB(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  //Disable further PLLB interrupts
  extChannelDisableI(&EXTD1, PLLB_EXTCHAN);
  //Enable PLLG, PLLR interrupts
  extChannelEnableI(&EXTD1, PLLG_EXTCHAN);
  extChannelEnableI(&EXTD1, PLLR_EXTCHAN);

  //Change PWM frequency to PLLB Frequency
  pwmChangePeriodI(&PWMD2, BLUE_PERIOD);
  //Turn on Blue LED, turn off Red, Green LED's
  palClearPad(GPIOA, GPIO_BLUE_LED);
  palSetPad(GPIOA, GPIO_GREEN_LED);
  palSetPad(GPIOA, GPIO_RED_LED);

  chSysUnlockFromIsr();
}

static void transmit_unlock(GPTDriver *gptp) {

  (void) gptp;

  extChannelEnableI(&EXTD1, 0);	//Reenable transmit interrupt

  chSysUnlockFromIsr();
}

static void transmit_stop(GPTDriver *gptp) {

  (void) gptp;

  //Stop transmitting
  pwmDisableChannel(&PWMD2, 3);
  palSetPad(GPIOA, GPIO_TX);

  chSysUnlockFromIsr();
}


static void extTransmit(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  //Start transmitting
  pwmEnableChannel(&PWMD2, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000));
  palClearPad(GPIOA, GPIO_TX);

 //Disable Transmit interrupt and start timer to reenable
  extChannelDisableI(&EXTD1, 0);                   //Disable transmit interrupt
  gptStartOneShotI(&GPTD1, TRANSMIT_TIME*100);    //Start timer for transmit
  gptStartOneShotI(&GPTD3, TRANSMIT_LOCKOUT*100); //Start timer for transmit unlock


  chSysUnlockFromIsr();
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extReset},
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extTransmit},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, extPLLR},
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, extPLLG},
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, extPLLB},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  extStart(&EXTD1, &extcfg);

  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PA9 and PA10 are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1));       /* USART1 TX.       */
  palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(1));      /* USART1 RX.       */

  //Configure Default Team Input Pins
  palSetPadMode(GPIOA, GPIO_TEAMID0, PAL_MODE_INPUT_PULLDOWN);
  palSetPadMode(GPIOA, GPIO_TEAMID1, PAL_MODE_INPUT_PULLDOWN);

  //Configure pins for TeamID LED's
  palSetPadMode(GPIOA, GPIO_RED_LED, PAL_MODE_OUTPUT_PUSHPULL);         //Red Team LED
  palSetPadMode(GPIOA, GPIO_GREEN_LED, PAL_MODE_OUTPUT_PUSHPULL);	//Green Team LED
  palSetPadMode(GPIOA, GPIO_BLUE_LED, PAL_MODE_OUTPUT_PUSHPULL);	//Blue Team LED

  //Configure pins for PLL's
  palSetPadMode(GPIOB, GPIO_RED_PLL, PAL_MODE_INPUT);         //Red Team PLL
  palSetPadMode(GPIOB, GPIO_GREEN_PLL, PAL_MODE_INPUT);       //Green Team PLL
  palSetPadMode(GPIOB, GPIO_BLUE_PLL, PAL_MODE_INPUT);        //Blue Team PLL

  // Configure pin for PWM output (A1: TIM2, channel 4)
  palSetPadMode(GPIOA, GPIO_MOD_FREQ, PAL_MODE_ALTERNATE(2));	//Modulation Freq Out
  //Configure pin for RX Enable
  palSetPadMode(GPIOA, GPIO_TX, PAL_MODE_OUTPUT_PUSHPULL);


  //Read Default Team Pins, set DefaultTeam
  DefaultTeam = (palReadPad(GPIOA, GPIO_TEAMID0) + palReadPad(GPIOA, GPIO_TEAMID1));
  CurrentTeam = DefaultTeam;

  //Set Initial Team
  if(CurrentTeam == REDTEAM) {
    pwmStart(&PWMD2, &pwmcfgRed);
    palClearPad(GPIOA, GPIO_RED_LED);
    palSetPad(GPIOA, GPIO_GREEN_LED);
    palSetPad(GPIOA, GPIO_BLUE_LED);
  }
  else if(CurrentTeam == GREENTEAM) {
    pwmStart(&PWMD2, &pwmcfgGreen);
    palClearPad(GPIOA, GPIO_GREEN_LED);
    palSetPad(GPIOA, GPIO_RED_LED);
    palSetPad(GPIOA, GPIO_BLUE_LED);
  }
  else {        //Blue Team
    pwmStart(&PWMD2, &pwmcfgBlue);
    palClearPad(GPIOA, GPIO_BLUE_LED);
    palSetPad(GPIOA, GPIO_RED_LED);
    palSetPad(GPIOA, GPIO_GREEN_LED);
  }

  //Enable GPT's
  gptStart(&GPTD1, &gpt1cfg);
  gptStart(&GPTD3, &gpt3cfg);

//  //Enable the PWM output on Channel 4 at 50%DC
//  pwmEnableChannel(&PWMD2, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000));

  /*
   * Creates the blinker threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 1.
   */
  while (TRUE) {
    //TODO Create idle loop
    if (palReadPad(GPIOA, GPIOA_BUTTON))
    chThdSleepMilliseconds(500);
  }
}
