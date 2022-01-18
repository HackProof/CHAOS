/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chsys.h
 * @brief   System related macros and structures.
 *
 * @addtogroup system
 * @{
 */

#ifndef CHSYS_H
#define CHSYS_H

/*lint -sem(chSysHalt, r_no)*/

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Masks of executable integrity checks.
 * @{
 */
#define CH_INTEGRITY_RLIST                  1U
#define CH_INTEGRITY_VTLIST                 2U
#define CH_INTEGRITY_REGISTRY               4U
#define CH_INTEGRITY_PORT                   8U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    ISRs abstraction macros
 */
/**
 * @brief   Priority level validation macro.
 * @details This macro determines if the passed value is a valid priority
 *          level for the underlying architecture.
 *
 * @param[in] prio      the priority level
 * @return              Priority range result.
 * @retval false        if the priority is invalid or if the architecture
 *                      does not support priorities.
 * @retval true         if the priority is valid.
 */
#if defined(PORT_IRQ_IS_VALID_PRIORITY) || defined(__DOXYGEN__)
#define CH_IRQ_IS_VALID_PRIORITY(prio)                                      \
  PORT_IRQ_IS_VALID_PRIORITY(prio)
#else
#define CH_IRQ_IS_VALID_PRIORITY(prio) false
#endif

/**
 * @brief   Priority level validation macro.
 * @details This macro determines if the passed value is a valid priority
 *          level that cannot preempt the kernel critical zone.
 *
 * @param[in] prio      the priority level
 * @return              Priority range result.
 * @retval false        if the priority is invalid or if the architecture
 *                      does not support priorities.
 * @retval true         if the priority is valid.
 */
#if defined(PORT_IRQ_IS_VALID_KERNEL_PRIORITY) || defined(__DOXYGEN__)
#define CH_IRQ_IS_VALID_KERNEL_PRIORITY(prio)                               \
  PORT_IRQ_IS_VALID_KERNEL_PRIORITY(prio)
#else
#define CH_IRQ_IS_VALID_KERNEL_PRIORITY(prio) false
#endif

/**
 * @brief   IRQ handler enter code.
 * @note    Usually IRQ handlers functions are also declared naked.
 * @note    On some architectures this macro can be empty.
 *
 * @special
 */
#define CH_IRQ_PROLOGUE()                                                   \
  PORT_IRQ_PROLOGUE();                                                      \
  CH_CFG_IRQ_PROLOGUE_HOOK();                                               \
  _stats_increase_irq();                                                    \
  _trace_isr_enter(__func__);                                               \
  _dbg_check_enter_isr()

/**
 * @brief   IRQ handler exit code.
 * @note    Usually IRQ handlers function are also declared naked.
 * @note    This macro usually performs the final reschedule by using
 *          @p chSchIsPreemptionRequired() and @p chSchDoReschedule().
 *
 * @special
 */
#define CH_IRQ_EPILOGUE()                                                   \
  _dbg_check_leave_isr();                                                   \
  _trace_isr_leave(__func__);                                               \
  CH_CFG_IRQ_EPILOGUE_HOOK();                                               \
  PORT_IRQ_EPILOGUE()

/**
 * @brief   Standard normal IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 *
 * @special
 */
#define CH_IRQ_HANDLER(id) PORT_IRQ_HANDLER(id)
/** @} */

/**
 * @name    Fast ISRs abstraction macros
 */
/**
 * @brief   Standard fast IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 * @note    Not all architectures support fast interrupts.
 *
 * @special
 */
#define CH_FAST_IRQ_HANDLER(id) PORT_FAST_IRQ_HANDLER(id)
/** @} */

/**
 * @name    Time conversion utilities for the realtime counter
 * @{
 */
/**
 * @brief   Seconds to realtime counter.
 * @details Converts from seconds to realtime counter cycles.
 * @note    The macro assumes that @p freq >= @p 1.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] sec       number of seconds
 * @return              The number of cycles.
 *
 * @api
 */
#define S2RTC(freq, sec) ((freq) * (sec))

/**
 * @brief   Milliseconds to realtime counter.
 * @details Converts from milliseconds to realtime counter cycles.
 * @note    The result is rounded upward to the next millisecond boundary.
 * @note    The macro assumes that @p freq >= @p 1000.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] msec      number of milliseconds
 * @return              The number of cycles.
 *
 * @api
 */
#define MS2RTC(freq, msec) (rtcnt_t)((((freq) + 999UL) / 1000UL) * (msec))

/**
 * @brief   Microseconds to realtime counter.
 * @details Converts from microseconds to realtime counter cycles.
 * @note    The result is rounded upward to the next microsecond boundary.
 * @note    The macro assumes that @p freq >= @p 1000000.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] usec      number of microseconds
 * @return              The number of cycles.
 *
 * @api
 */
#define US2RTC(freq, usec) (rtcnt_t)((((freq) + 999999UL) / 1000000UL) * (usec))

/**
 * @brief   Realtime counter cycles to seconds.
 * @details Converts from realtime counter cycles number to seconds.
 * @note    The result is rounded up to the next second boundary.
 * @note    The macro assumes that @p freq >= @p 1.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] n         number of cycles
 * @return              The number of seconds.
 *
 * @api
 */
#define RTC2S(freq, n) ((((n) - 1UL) / (freq)) + 1UL)

/**
 * @brief   Realtime counter cycles to milliseconds.
 * @details Converts from realtime counter cycles number to milliseconds.
 * @note    The result is rounded up to the next millisecond boundary.
 * @note    The macro assumes that @p freq >= @p 1000.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] n         number of cycles
 * @return              The number of milliseconds.
 *
 * @api
 */
#define RTC2MS(freq, n) ((((n) - 1UL) / ((freq) / 1000UL)) + 1UL)

/**
 * @brief   Realtime counter cycles to microseconds.
 * @details Converts from realtime counter cycles number to microseconds.
 * @note    The result is rounded up to the next microsecond boundary.
 * @note    The macro assumes that @p freq >= @p 1000000.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] n         number of cycles
 * @return              The number of microseconds.
 *
 * @api
 */
#define RTC2US(freq, n) ((((n) - 1UL) / ((freq) / 1000000UL)) + 1UL)
/** @} */

/**
 * @brief   Returns the current value of the system real time counter.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @return              The value of the system realtime counter of
 *                      type rtcnt_t.
 *
 * @xclass
 */
#if (PORT_SUPPORTS_RT == TRUE) || defined(__DOXYGEN__)
#define chSysGetRealtimeCounterX() (rtcnt_t)port_rt_get_counter_value()
#endif

/**
 * @brief   Performs a context switch.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 *
 * @special
 */
// CHAOS ... 보류?
// CHAOS ... audit 부분 추가 (2021-10-13); auditspace_process() 추가했다 삭제함
#define chSysSwitch(ntp, otp) {                                             \
  _trace_switch(ntp, otp);                                                  \
  _stats_ctxswc(ntp, otp);                                                  \
  CH_CFG_CONTEXT_SWITCH_HOOK(ntp, otp);                                     \
  port_switch(ntp, otp);                                                    \
  auditspace_process();                                                     \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern stkalign_t ch_idle_thread_wa[];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  // void _secure_init(void);
  void chSysInit(void);
  bool chSysIntegrityCheckI(unsigned testmask);
  void chSysTimerHandlerI(void);
  syssts_t chSysGetStatusAndLockX(void);
  void chSysRestoreStatusX(syssts_t sts);
#if PORT_SUPPORTS_RT == TRUE
  bool chSysIsCounterWithinX(rtcnt_t cnt, rtcnt_t start, rtcnt_t end);
  void chSysPolledDelayX(rtcnt_t cycles);
#endif

  /*===========================================================================*/
  /* Module inline functions.                                                  */
  /*===========================================================================*/

  void chSysDisable(void);
  void chSysSuspend(void);
  void chSysEnable(void);
  void chSysLock(void);
  void chSysUnlock(void);
  void chSysLockFromISR(void);
  void chSysUnlockFromISR(void);
  void chSysUnconditionalLock(void);
  void chSysUnconditionalUnlock(void);
#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
thread_t *chSysGetIdleThreadX(void);
#endif /* CH_CFG_NO_IDLE_THREAD == FALSE */
#ifdef __cplusplus
}
#endif

#endif /* CHSYS_H */

/** @} */
