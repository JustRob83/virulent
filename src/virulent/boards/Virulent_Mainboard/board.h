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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for Virulent Mainboard.
 */

/*
 * Board identifier.
 */
#define BOARD_VIRULENT_MAINBOARD
#define BOARD_NAME                  "Virulent Mainboard V1.0"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 * NOTE: HSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0
#endif

#define STM32_LSEDRV                (3 << 3)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                0
#endif

#define STM32_HSE_BYPASS

/*
 * MCU type as defined in the ST header file stm32f0xx.h.
 */
#define STM32F0XX

/*
 * IO pins assignments.
 */
#define GPIO_NSW_RESET              0
#define GPIO_5V_ENABLE              1
#define GPIO_RX                     2
#define GPIO_MOD_FREQ               3
#define GPIO_TRIGGER                4
#define GPIO_BLUE_LED               5
#define GPIO_RED_LED                6
#define GPIO_GREEN_LED              7
#define GPIOA_PIN8                   8
#define GPIO_TEAMID0                9
#define GPIO_TEAMID1                10
#define GPIO_SWDAT                  13
#define GPIO_SWCLK                  14
#define GPIO_SWO                    15

#define GPIOB_PIN0                  0
#define GPIOB_PIN1                  1
#define GPIOB_PIN3                  3
#define GPIOB_PIN4                  4
#define GPIO_RED_PLL                5
#define GPIO_GREEN_PLL              6
#define GPIO_BLUE_PLL               7

#define GPIOF_OSC_IN                0
#define GPIOF_OSC_OUT               1

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_10M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_40M(n)           (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 *
 * PA0  - BUTTON                    (input floating).
 * PA1  - PIN1                      (input pullup).
 * PA2  - PIN2                      (input pullup).
 * PA3  - PIN3                      (input pullup).
 * PA4  - PIN4                      (input pullup).
 * PA5  - PIN5                      (input pullup).
 * PA6  - PIN6                      (input pullup).
 * PA7  - PIN7                      (input pullup).
 * PA8  - PIN8                      (input pullup).
 * PA9  - PIN9                      (input pullup).
 * PA10 - PIN10                     (input pullup).
 * PA13 - SWDAT                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - PIN15                     (input pullup).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIO_NSW_RESET) |      \
                                     PIN_MODE_OUTPUT(GPIO_5V_ENABLE) |     \
                                     PIN_MODE_OUTPUT(GPIO_RX) |            \
                                     PIN_MODE_ALTERNATE(GPIO_MOD_FREQ) |   \
                                     PIN_MODE_INPUT(GPIO_TRIGGER) |       \
                                     PIN_MODE_OUTPUT(GPIO_BLUE_LED) |     \
                                     PIN_MODE_OUTPUT(GPIO_RED_LED) |      \
                                     PIN_MODE_OUTPUT(GPIO_GREEN_LED) |    \
                                     PIN_MODE_INPUT(GPIOA_PIN8) |          \
                                     PIN_MODE_INPUT(GPIO_TEAMID0) |        \
                                     PIN_MODE_INPUT(GPIO_TEAMID1) |        \
                                     PIN_MODE_ALTERNATE(GPIO_SWDAT) |      \
                                     PIN_MODE_ALTERNATE(GPIO_SWCLK) |      \
                                     PIN_MODE_INPUT(GPIO_SWO))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIO_NSW_RESET) |  \
                                     PIN_OTYPE_PUSHPULL(GPIO_5V_ENABLE) |  \
                                     PIN_OTYPE_PUSHPULL(GPIO_RX) |         \
                                     PIN_OTYPE_PUSHPULL(GPIO_MOD_FREQ) |   \
                                     PIN_OTYPE_PUSHPULL(GPIO_TRIGGER) |   \
                                     PIN_OTYPE_PUSHPULL(GPIO_BLUE_LED) |  \
                                     PIN_OTYPE_PUSHPULL(GPIO_RED_LED) |   \
                                     PIN_OTYPE_PUSHPULL(GPIO_GREEN_LED) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8) |      \
                                     PIN_OTYPE_PUSHPULL(GPIO_TEAMID0) |    \
                                     PIN_OTYPE_PUSHPULL(GPIO_TEAMID1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIO_SWDAT) |      \
                                     PIN_OTYPE_PUSHPULL(GPIO_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIO_SWO))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_2M(GPIO_NSW_RESET) |       \
                                     PIN_OSPEED_2M(GPIO_5V_ENABLE) |       \
                                     PIN_OSPEED_2M(GPIO_RX) |              \
                                     PIN_OSPEED_2M(GPIO_MOD_FREQ) |        \
                                     PIN_OSPEED_2M(GPIO_TRIGGER) |        \
                                     PIN_OSPEED_2M(GPIO_BLUE_LED) |       \
                                     PIN_OSPEED_2M(GPIO_RED_LED) |        \
                                     PIN_OSPEED_2M(GPIO_GREEN_LED) |      \
                                     PIN_OSPEED_2M(GPIOA_PIN8) |           \
                                     PIN_OSPEED_2M(GPIO_TEAMID0) |         \
                                     PIN_OSPEED_2M(GPIO_TEAMID1) |         \
                                     PIN_OSPEED_40M(GPIO_SWDAT) |          \
                                     PIN_OSPEED_40M(GPIO_SWCLK) |          \
                                     PIN_OSPEED_40M(GPIO_SWO))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIO_NSW_RESET) |  \
                                     PIN_PUPDR_FLOATING(GPIO_5V_ENABLE) |  \
                                     PIN_PUPDR_FLOATING(GPIO_RX) |         \
                                     PIN_PUPDR_PULLDOWN(GPIO_MOD_FREQ) |   \
                                     PIN_PUPDR_FLOATING(GPIO_TRIGGER) |   \
                                     PIN_PUPDR_PULLUP(GPIO_BLUE_LED) |    \
                                     PIN_PUPDR_PULLUP(GPIO_RED_LED) |     \
                                     PIN_PUPDR_PULLUP(GPIO_GREEN_LED) |   \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN8) |        \
                                     PIN_PUPDR_PULLDOWN(GPIO_TEAMID0) |    \
                                     PIN_PUPDR_PULLDOWN(GPIO_TEAMID1) |    \
                                     PIN_PUPDR_PULLUP(GPIO_SWDAT) |        \
                                     PIN_PUPDR_PULLDOWN(GPIO_SWCLK) |      \
                                     PIN_PUPDR_PULLUP(GPIO_SWO))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIO_NSW_RESET) |        \
                                     PIN_ODR_LOW(GPIO_5V_ENABLE) |        \
                                     PIN_ODR_HIGH(GPIO_RX) |             \
                                     PIN_ODR_LOW(GPIO_MOD_FREQ) |        \
                                     PIN_ODR_HIGH(GPIO_TRIGGER) |       \
                                     PIN_ODR_HIGH(GPIO_BLUE_LED) |      \
                                     PIN_ODR_HIGH(GPIO_RED_LED) |       \
                                     PIN_ODR_HIGH(GPIO_GREEN_LED) |     \
                                     PIN_ODR_HIGH(GPIOA_PIN8) |          \
                                     PIN_ODR_HIGH(GPIO_TEAMID0) |        \
                                     PIN_ODR_HIGH(GPIO_TEAMID1) |        \
                                     PIN_ODR_HIGH(GPIO_SWDAT) |          \
                                     PIN_ODR_HIGH(GPIO_SWCLK) |          \
                                     PIN_ODR_HIGH(GPIO_SWO))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIO_NSW_RESET, 0) |    \
                                     PIN_AFIO_AF(GPIO_5V_ENABLE, 0) |    \
                                     PIN_AFIO_AF(GPIO_RX, 0) |           \
                                     PIN_AFIO_AF(GPIO_MOD_FREQ, 2) |     \
                                     PIN_AFIO_AF(GPIO_TRIGGER, 0) |     \
                                     PIN_AFIO_AF(GPIO_BLUE_LED, 0) |    \
                                     PIN_AFIO_AF(GPIO_RED_LED, 0) |     \
                                     PIN_AFIO_AF(GPIO_GREEN_LED, 0))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0) |        \
                                     PIN_AFIO_AF(GPIO_TEAMID0, 0) |      \
                                     PIN_AFIO_AF(GPIO_TEAMID1, 0) |      \
                                     PIN_AFIO_AF(GPIO_SWDAT, 0) |        \
                                     PIN_AFIO_AF(GPIO_SWCLK, 0) |        \
                                     PIN_AFIO_AF(GPIO_SWO, 0))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0                      (input pullup).
 * PB1  - PIN1                      (input pullup).
 * PB3  - PIN3                      (input pullup).
 * PB4  - PIN4                      (input pullup).
 * PB5  - PIN5                      (input pullup).
 * PB6  - PIN6                      (input pullup).
 * PB7  - PIN7                      (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN4) |           \
                                     PIN_MODE_INPUT(GPIO_RED_PLL) |         \
                                     PIN_MODE_INPUT(GPIO_GREEN_PLL) |       \
                                     PIN_MODE_INPUT(GPIO_BLUE_PLL))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIO_RED_PLL) |     \
                                     PIN_OTYPE_PUSHPULL(GPIO_GREEN_PLL) |   \
                                     PIN_OTYPE_PUSHPULL(GPIO_BLUE_PLL))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_2M(GPIOB_PIN0) |            \
                                     PIN_OSPEED_2M(GPIOB_PIN1) |            \
                                     PIN_OSPEED_40M(GPIOB_PIN3) |           \
                                     PIN_OSPEED_40M(GPIOB_PIN4) |           \
                                     PIN_OSPEED_2M(GPIO_RED_PLL) |          \
                                     PIN_OSPEED_2M(GPIO_GREEN_PLL) |        \
                                     PIN_OSPEED_2M(GPIO_BLUE_PLL))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN4) |         \
                                     PIN_PUPDR_FLOATING(GPIO_RED_PLL) |     \
                                     PIN_PUPDR_FLOATING(GPIO_GREEN_PLL) |   \
                                     PIN_PUPDR_FLOATING(GPIO_BLUE_PLL))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN4) |             \
                                     PIN_ODR_LOW(GPIO_RED_PLL) |            \
                                     PIN_ODR_LOW(GPIO_GREEN_PLL) |          \
                                     PIN_ODR_LOW(GPIO_BLUE_PLL))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIO_RED_PLL, 0) |         \
                                     PIN_AFIO_AF(GPIO_GREEN_PLL, 0) |       \
                                     PIN_AFIO_AF(GPIO_BLUE_PLL, 0))

/*
 * GPIOF setup:
 *
 * PF0  - OSC_IN                    (input floating).
 * PF1  - OSC_OUT                   (input floating).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOF_OSC_OUT))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_OSC_OUT))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_2M(GPIOF_OSC_IN) |          \
                                     PIN_OSPEED_2M(GPIOF_OSC_OUT))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_OSC_OUT))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOF_OSC_OUT))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOF_OSC_OUT, 0))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
