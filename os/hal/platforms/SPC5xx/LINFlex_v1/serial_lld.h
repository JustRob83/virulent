/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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

/**
 * @file    SPC5xx/LINFlex_v1/serial_lld.h
 * @brief   SPC5xx low level serial driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

#include "spc5_linflex.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Serial driver allowable modes
 * @{
 */
#define SD_MODE_8BITS_PARITY_NONE           (SPC5_UARTCR_WL)
#define SD_MODE_8BITS_PARITY_EVEN           (SPC5_UARTCR_WL |               \
                                             SPC5_UARTCR_PCE)
#define SD_MODE_8BITS_PARITY_ODD            (SPC5_UARTCR_WL |               \
                                             SPC5_UARTCR_PCE |              \
                                             SPC5_UARTCR_OP)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   LINFlex-0 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-0 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX0) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX0            FALSE
#endif

/**
 * @brief   LINFlex-1 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-1 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX1) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX1            FALSE
#endif

/**
 * @brief   LINFlex-2 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-2 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX2) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX2            FALSE
#endif

/**
 * @brief   LINFlex-3 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-3 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX3) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX3            FALSE
#endif

/**
 * @brief   LINFlex-0 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_PRIORITY       8
#endif

/**
 * @brief   LINFlex-1 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_PRIORITY       8
#endif

/**
 * @brief   LINFlex-2 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX2_PRIORITY       8
#endif

/**
 * @brief   LINFlex-3 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX3_PRIORITY       8
#endif

/**
 * @brief   LINFlex-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX2_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX2_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX3_START_PCTL     (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SERIAL_LINFLEX3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX3_STOP_PCTL      (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if SPC5_SERIAL_USE_LINFLEX0 && !SPC5_HAS_LINFLEX0
#error "LINFlex-0 not present in the selected device"
#endif

#if SPC5_SERIAL_USE_LINFLEX1 && !SPC5_HAS_LINFLEX1
#error "LINFlex-1 not present in the selected device"
#endif

#if SPC5_SERIAL_USE_LINFLEX2 && !SPC5_HAS_LINFLEX2
#error "LINFlex-2 not present in the selected device"
#endif

#if SPC5_SERIAL_USE_LINFLEX3 && !SPC5_HAS_LINFLEX3
#error "LINFlex-3 not present in the selected device"
#endif

#if !SPC5_SERIAL_USE_LINFLEX0 && !SPC5_SERIAL_USE_LINFLEX1 &&               \
    !SPC5_SERIAL_USE_LINFLEX2 && !SPC5_SERIAL_USE_LINFLEX3
#error "SERIAL driver activated but no LINFlex peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Generic Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief Bit rate.
   */
  uint32_t                  speed;
  /**
   * @brief Mode flags.
   */
  uint8_t                   mode;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  InputQueue                iqueue;                                         \
  /* Output queue.*/                                                        \
  OutputQueue               oqueue;                                         \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the volatile LINFlex registers block.*/                     \
  volatile struct spc5_linflex *linflexp;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPC5_SERIAL_USE_LINFLEX0 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if SPC5_SERIAL_USE_LINFLEX1 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if SPC5_SERIAL_USE_LINFLEX2 && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if SPC5_SERIAL_USE_LINFLEX3 && !defined(__DOXYGEN__)
extern SerialDriver SD4;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
