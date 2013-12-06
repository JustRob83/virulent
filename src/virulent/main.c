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

/*
 * STM32F0-DISCOVERY Board Assignments
 */
#ifdef BOARD_ST_STM32F0_DISCOVERY

#define GPIO_TEAMID0            GPIOA_PIN11
#define GPIO_TEAMID1            GPIOA_PIN12

#define GPIO_SW_RESET           GPIOA_PIN4
#define GPIO_5V_ENABLE          GPIOA_PIN1
#define GPIO_MOD_FREQ		GPIOA_PIN3
#define GPIO_RX                 GPIOA_PIN2
#define GPIO_TRIGGER            GPIOA_BUTTON

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
#define BLUETEAM                0
#define GREENTEAM               1
#define REDTEAM                 2
#define TRANSMIT_TIME           1000      //Transmit time in milliseconds.
#define TRANSMIT_LOCKOUT        4000      //Transmit lockout time in milliseconds.

/*
 * Interrupt callback function prototypes
 */
static void transmit_unlock(GPTDriver *gptp);
static void transmit_stop(GPTDriver *gptp);
static void extReset(EXTDriver *extp, expchannel_t channel);
static void extPLLR(EXTDriver *extp, expchannel_t channel);
static void extPLLG(EXTDriver *extp, expchannel_t channel);
static void extPLLB(EXTDriver *extp, expchannel_t channel);

/*
 * Helper function prototypes
 */
void Red_LED(void);
void Green_LED(void);
void Blue_LED(void);
void Red_Team(void);
void Green_Team(void);
void Blue_Team(void);

/*
 * Constants
 */
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

/*
 * Static variables.
 */
static int DefaultTeam = 0;             //
static int CurrentTeam = 0;


/*
 * GPIO_NSW_RESET interrupt handler. Switch to the default team.
 */
static void extReset(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  CurrentTeam = DefaultTeam;

  //Enable default team LED, Interrupts and PWM period
  if(CurrentTeam == REDTEAM)
    Red_Team();
  else if(CurrentTeam == GREENTEAM)
    Green_Team();
  else  //Blue Team
    Blue_Team();

  chSysUnlockFromIsr();
}

/*
 * PLLR interrupt callback. Switch to Red Team.
 */
static void extPLLR(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  Red_Team();

  chSysUnlockFromIsr();
}

/*
 * PLLG interrupt callback. Switch to Green Team.
 */
static void extPLLG(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  Green_Team();

  chSysUnlockFromIsr();
}

/*
 * PLLB interrupt callback. Switch to Blue Team.
 */
static void extPLLB(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  Blue_Team();

  chSysUnlockFromIsr();
}

/*
 * GPT3 interrupt callback. Reenable transmit interrupts.
 */
static void transmit_unlock(GPTDriver *gptp) {

  (void) gptp;

  extChannelEnableI(&EXTD1, 0);	//Reenable transmit interrupt

  chSysUnlockFromIsr();
}

/*
 * GPT1 interrupt callback. Stop transmit burst.
 */
static void transmit_stop(GPTDriver *gptp) {

  (void) gptp;

  //Stop transmitting
  pwmDisableChannel(&PWMD2, 3);
  palSetPad(GPIOA, GPIO_RX);

  chSysUnlockFromIsr();
}

/*
 * GPIO_TRIGGER interrupt callback. Start transmit burst, disable additional
 * transmit interrupts for TRANSMIT_LOCKOUT.
 */
static void extTransmit(EXTDriver *extp, expchannel_t channel) {

  (void)extp;
  (void)channel;

  //Start transmitting
  pwmEnableChannel(&PWMD2, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 5000));
  palClearPad(GPIOA, GPIO_RX);

 //Disable Transmit interrupt and start timer to reenable
  extChannelDisableI(&EXTD1, 0);              //Disable transmit interrupt
  gptStartOneShotI(&GPTD1, TRANSMIT_TIME);    //Start timer for transmit
  gptStartOneShotI(&GPTD3, TRANSMIT_LOCKOUT); //Start timer for transmit unlock


  chSysUnlockFromIsr();
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extReset},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extTransmit},
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

  //Start external interrupt subsystem
  extStart(&EXTD1, &extcfg);

  //Disable 5V_ENABLE for startup sequence
  palClearPad(GPIOA, GPIO_5V_ENABLE);

  //Configure pin for 5V Enable
  palSetPadMode(GPIOA, GPIO_5V_ENABLE, PAL_MODE_OUTPUT_PUSHPULL);       //Red Team LED

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
  palSetPadMode(GPIOA, GPIO_RX, PAL_MODE_OUTPUT_PUSHPULL);

  //Read Default Team Pins, set DefaultTeam
  DefaultTeam = (palReadPad(GPIOA, GPIO_TEAMID0) + palReadPad(GPIOA, GPIO_TEAMID1));
  CurrentTeam = DefaultTeam;

  //Set Initial Team
  if(CurrentTeam == REDTEAM) {
    pwmStart(&PWMD2, &pwmcfgRed);
    Red_Team();
  }
  else if(CurrentTeam == GREENTEAM) {
    pwmStart(&PWMD2, &pwmcfgGreen);
    Green_Team();
  }
  else {        //Blue Team
    pwmStart(&PWMD2, &pwmcfgBlue);
    Blue_Team();
  }

  //Enable 5V_ENABLE
  palSetPad(GPIOA, GPIO_5V_ENABLE);

  //Enable GPT's
  gptStart(&GPTD1, &gpt1cfg);
  gptStart(&GPTD3, &gpt3cfg);

  //Idle loop. Wait for interrupts.
  while (TRUE)
    chThdSleepMilliseconds(500);
}

/*
 * Turn on Red LED, turn off Green, Blue LED
 */
void Red_LED(void) {

  palClearPad(GPIOA, GPIO_RED_LED);
  palSetPad(GPIOA, GPIO_GREEN_LED);
  palSetPad(GPIOA, GPIO_BLUE_LED);
}

/*
 * Turn on Green LED, turn off Red, Blue LED
 */
void Green_LED(void) {

  palClearPad(GPIOA, GPIO_GREEN_LED);
  palSetPad(GPIOA, GPIO_RED_LED);
  palSetPad(GPIOA, GPIO_BLUE_LED);
}

/*
 * Turn on Blue LED, turn off Green, Red LED
 */
void Blue_LED(void) {

  palClearPad(GPIOA, GPIO_BLUE_LED);
  palSetPad(GPIOA, GPIO_RED_LED);
  palSetPad(GPIOA, GPIO_GREEN_LED);
}

/*
 * Disable PLLR interrupt, enable PLLG, PLLB interrupts, switch to Red LED
 */
void Red_Team(void) {

  //Disable further PLLR interrupts
  extChannelDisableI(&EXTD1, PLLR_EXTCHAN);
  //Enable PLLG, PLLB interrupts
  extChannelEnableI(&EXTD1, PLLG_EXTCHAN);
  extChannelEnableI(&EXTD1, PLLB_EXTCHAN);
  //Change PWM frequency to PLLR Frequency
  pwmChangePeriodI(&PWMD2, RED_PERIOD);
  //Turn on Red LED, turn off Green, Blue LED's
  Red_LED();
}

/*
 * Disable PLLG interrupt, enable PLLR, PLLB interrupts, switch to Green LED
 */
void Green_Team(void) {

  //Disable further PLLG interrupts
  extChannelDisableI(&EXTD1, PLLG_EXTCHAN);
  //Enable PLLR, PLLB interrupts
  extChannelEnableI(&EXTD1, PLLR_EXTCHAN);
  extChannelEnableI(&EXTD1, PLLB_EXTCHAN);
  //Change PWM frequency to PLLG Frequency
  pwmChangePeriodI(&PWMD2, GREEN_PERIOD);
  //Turn on Green LED, turn off Red, Blue LED's
  Green_LED();
}

/*
 * Disable PLLB interrupt, enable PLLG, PLLR interrupts, switch to Blue LED
 */
void Blue_Team(void) {

  //Disable further PLLB interrupts
  extChannelDisableI(&EXTD1, PLLB_EXTCHAN);
  //Enable PLLG, PLLR interrupts
  extChannelEnableI(&EXTD1, PLLG_EXTCHAN);
  extChannelEnableI(&EXTD1, PLLR_EXTCHAN);
  //Change PWM frequency to PLLB Frequency
  pwmChangePeriodI(&PWMD2, BLUE_PERIOD);
  //Turn on Blue LED, turn off Red, Green LED's
  Blue_LED();
}
