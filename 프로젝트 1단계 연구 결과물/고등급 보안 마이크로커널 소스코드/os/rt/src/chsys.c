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
 * @file    chsys.c
 * @brief   System related code.
 *
 * @addtogroup system
 * @details System related APIs and services:
 *          - Initialization.
 *          - Locks.
 *          - Interrupt Handling.
 *          - Power Management.
 *          - Abnormal Termination.
 *          - Realtime counter.
 *          .
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   Idle thread working area.
 */
THD_WORKING_AREA(ch_idle_thread_wa, PORT_IDLE_THREAD_STACK_SIZE);
#endif

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   This function implements the idle thread infinite loop.
 * @details The function puts the processor in the lowest power mode capable
 *          to serve interrupts.<br>
 *          The priority is internally set to the minimum system value so
 *          that this thread is executed only if there are no other ready
 *          threads in the system.
 *
 * @param[in] p         the thread parameter, unused in this scenario
 */
static void _idle_thread(void *p) {

  (void)p;

  while (true) {
    /*lint -save -e522 [2.2] Apparently no side effects because it contains
      an asm instruction.*/
    port_wait_for_interrupt();
    /*lint -restore*/
    CH_CFG_IDLE_LOOP_HOOK();
  }
}
#endif /* CH_CFG_NO_IDLE_THREAD == FALSE */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

// void _secure_init(void) {
//   ch.secure.verification = FALSE;
// }


/**
 * @brief   ChibiOS/RT initialization.
 * @details After executing this function the current instructions stream
 *          becomes the main thread.
 * @pre     Interrupts must disabled before invoking this function.
 * @post    The main thread is created with priority @p NORMALPRIO and
 *          interrupts are enabled.
 *
 * @special
 */
void chSysInit(void) {
  _audit_init();  // CHAOS
  _fia_init();    // CHAOS

  _scheduler_init();
  _vt_init();
  _trace_init();


#if CH_DBG_SYSTEM_STATE_CHECK == TRUE
  ch.dbg.isr_cnt  = (cnt_t)0;
  ch.dbg.lock_cnt = (cnt_t)0;
#endif
#if CH_CFG_USE_TM == TRUE
  _tm_init();
#endif
#if CH_CFG_USE_MEMCORE == TRUE
  _core_init();
#endif
#if CH_CFG_USE_HEAP == TRUE
  _heap_init();
#endif
#if CH_CFG_USE_FACTORY == TRUE
  _factory_init();
#endif
#if CH_DBG_STATISTICS == TRUE
  _stats_init();
#endif

#if CH_CFG_NO_IDLE_THREAD == FALSE
  /* Now this instructions flow becomes the main thread.*/
#if CH_CFG_USE_REGISTRY == TRUE
  setCurrp(_thread_init(&ch.mainthread, (const char *)&ch_debug, NORMALPRIO()));
#else
  setCurrp(_thread_init(&ch.mainthread, "main", NORMALPRIO()));
#endif
#else
  /* Now this instructions flow becomes the idle thread.*/
  setCurrp(_thread_init(&ch.mainthread, "idle", IDLEPRIO()));
#endif

#if CH_DBG_ENABLE_STACK_CHECK == TRUE
  {
    /* Setting up the base address of the static main thread stack, the
       symbol must be provided externally.*/
    extern stkalign_t __main_thread_stack_base__;
    getCurrp()->wabase = &__main_thread_stack_base__;
  }
#elif CH_CFG_USE_DYNAMIC == TRUE
  getCurrp()->wabase = NULL;
#endif

  /* Setting up the caller as current thread.*/
  getCurrp()->state = CH_STATE_CURRENT();

  /* Port layer initialization last because it depend on some of the
     initializations performed before.*/
  port_init();

#if CH_DBG_STATISTICS == TRUE
  /* Starting measurement for this thread.*/
  chTMStartMeasurementX(&getCurrp()->stats);
#endif

  /* Initialization hook.*/
  CH_CFG_SYSTEM_INIT_HOOK();

  /* It is alive now.*/
  chSysEnable();

#if CH_CFG_NO_IDLE_THREAD == FALSE
  {
    static const thread_descriptor_t idle_descriptor = {
      "idle",
      THD_WORKING_AREA_BASE(ch_idle_thread_wa),
      THD_WORKING_AREA_END(ch_idle_thread_wa),
      (tprio_t)1,
      _idle_thread,
      NULL
    };

    /* This thread has the lowest priority in the system, its role is just to
       serve interrupts in its context while keeping the lowest energy saving
       mode compatible with the system status.*/
    (void) chThdCreate(secret_kValue, &idle_descriptor);
  }
#endif

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected, for example because a programming
 *          error in the application code that triggers an assertion while
 *          in debug mode.
 * @note    Can be invoked from any system state.
 *
 * @param[in] reason        pointer to an error string
 *
 * @special
 */
void chSysHalt(const char *reason) {
  port_disable();

  /* Logging the event.*/
  _trace_halt(reason);

  /* Pointing to the passed message.*/
  ch.dbg.panic_msg = reason;

  /* Halt hook code, usually empty.*/
  CH_CFG_SYSTEM_HALT_HOOK(reason);

  auditbox_try_update_buffer(__func__, true); // CHAOS audit

  /* Harmless infinite loop.*/
  while (true) {
  }
}

/**
 * @brief   System integrity check.
 * @details Performs an integrity check of the important ChibiOS/RT data
 *          structures.
 * @note    The appropriate action in case of failure is to halt the system
 *          before releasing the critical zone.
 * @note    If the system is corrupted then one possible outcome of this
 *          function is an exception caused by @p NULL or corrupted pointers
 *          in list elements. Exception vectors must be monitored as well.
 * @note    This function is not used internally, it is up to the
 *          application to define if and where to perform system
 *          checking.
 * @note    Performing all tests at once can be a slow operation and can
 *          degrade the system response time. It is suggested to execute
 *          one test at time and release the critical zone in between tests.
 *
 * @param[in] testmask  Each bit in this mask is associated to a test to be
 *                      performed.
 * @return              The test result.
 * @retval false        The test succeeded.
 * @retval true         Test failed.
 *
 * @iclass
 */
bool chSysIntegrityCheckI(unsigned testmask) {
  cnt_t n;

  chDbgCheckClassI();

  /* Ready List integrity check.*/
  if ((testmask & CH_INTEGRITY_RLIST) != 0U) {
    thread_t *tp;

    /* Scanning the ready list forward.*/
    n = (cnt_t)0;
    tp = ch.rlist.queue.next;
    while (tp != (thread_t *)&ch.rlist.queue) {
      n++;
      tp = tp->queue.next;
    }

    /* Scanning the ready list backward.*/
    tp = ch.rlist.queue.prev;
    while (tp != (thread_t *)&ch.rlist.queue) {
      n--;
      tp = tp->queue.prev;
    }

    /* The number of elements must match.*/
    if (n != (cnt_t)0) {
      return true;
    }
  }

  /* Timers list integrity check.*/
  if ((testmask & CH_INTEGRITY_VTLIST) != 0U) {
    virtual_timer_t * vtp;

    /* Scanning the timers list forward.*/
    n = (cnt_t)0;
    vtp = ch.vtlist.next;
    while (vtp != (virtual_timer_t *)&ch.vtlist) {
      n++;
      vtp = vtp->next;
    }

    /* Scanning the timers list backward.*/
    vtp = ch.vtlist.prev;
    while (vtp != (virtual_timer_t *)&ch.vtlist) {
      n--;
      vtp = vtp->prev;
    }

    /* The number of elements must match.*/
    if (n != (cnt_t)0) {
      return true;
    }
  }

#if CH_CFG_USE_REGISTRY == TRUE
  if ((testmask & CH_INTEGRITY_REGISTRY) != 0U) {
    thread_t *tp;

    /* Scanning the ready list forward.*/
    n = (cnt_t)0;
    tp = ch.rlist.newer;
    while (tp != (thread_t *)&ch.rlist) {
      n++;
      tp = tp->newer;
    }

    /* Scanning the ready list backward.*/
    tp = ch.rlist.older;
    while (tp != (thread_t *)&ch.rlist) {
      n--;
      tp = tp->older;
    }

    /* The number of elements must match.*/
    if (n != (cnt_t)0) {
      return true;
    }
  }
#endif /* CH_CFG_USE_REGISTRY == TRUE */

#if defined(PORT_INTEGRITY_CHECK)
  if ((testmask & CH_INTEGRITY_PORT) != 0U) {
    PORT_INTEGRITY_CHECK();
  }
#endif

  return false;
}

/**
 * @brief   Handles time ticks for round robin preemption and timer increments.
 * @details Decrements the remaining time quantum of the running thread
 *          and preempts it when the quantum is used up. Increments system
 *          time and manages the timers.
 * @note    The frequency of the timer determines the system tick granularity
 *          and, together with the @p CH_CFG_TIME_QUANTUM macro, the round robin
 *          interval.
 *
 * @iclass
 */
void chSysTimerHandlerI(void) {

  chDbgCheckClassI();

#if CH_CFG_TIME_QUANTUM > 0
  /* Running thread has not used up quantum yet? */
  if (getCurrp()->ticks > (tslices_t)0) {
    /* Decrement remaining quantum.*/
    getCurrp()->ticks--;
  }
#endif
#if CH_DBG_THREADS_PROFILING == TRUE
  getCurrp()->time++;
#endif
  chVTDoTickI();
  CH_CFG_SYSTEM_TICK_HOOK();
}

/**
 * @brief   Returns the execution status and enters a critical zone.
 * @details This functions enters into a critical zone and can be called
 *          from any context. Because its flexibility it is less efficient
 *          than @p chSysLock() which is preferable when the calling context
 *          is known.
 * @post    The system is in a critical zone.
 *
 * @return              The previous system status, the encoding of this
 *                      status word is architecture-dependent and opaque.
 *
 * @xclass
 */
syssts_t chSysGetStatusAndLockX(void) {

  syssts_t sts = port_get_irq_status();
  if (port_irq_enabled(sts)) {
    if (port_is_isr_context()) {
      chSysLockFromISR();
    }
    else {
      chSysLock();
    }
  }

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
  return sts;
}

/**
 * @brief   Restores the specified execution status and leaves a critical zone.
 * @note    A call to @p chSchRescheduleS() is automatically performed
 *          if exiting the critical zone and if not in ISR context.
 *
 * @param[in] sts       the system status to be restored.
 *
 * @xclass
 */
void chSysRestoreStatusX(syssts_t sts) {

  if (port_irq_enabled(sts)) {
    if (port_is_isr_context()) {
      chSysUnlockFromISR();
    }
    else {
      chSchRescheduleS();
      chSysUnlock();
    }
  }
}

#if (PORT_SUPPORTS_RT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Realtime window test.
 * @details This function verifies if the current realtime counter value
 *          lies within the specified range or not. The test takes care
 *          of the realtime counter wrapping to zero on overflow.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @param[in] cnt       the counter value to be tested
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
bool chSysIsCounterWithinX(rtcnt_t cnt, rtcnt_t start, rtcnt_t end) {

  return (bool)((cnt - start) < (end - start));
}

/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @param[in] cycles    number of cycles
 *
 * @xclass
 */
void chSysPolledDelayX(rtcnt_t cycles) {
  rtcnt_t start = chSysGetRealtimeCounterX();
  rtcnt_t end  = start + cycles;

  while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end)) {
  }
}
#endif /* PORT_SUPPORTS_RT == TRUE */

/*===========================================================================*/
/* Module inline functions.  (previously)                                    */
/*===========================================================================*/

/**
 * @brief   Raises the system interrupt priority mask to the maximum level.
 * @details All the maskable interrupt sources are disabled regardless their
 *          hardware priority.
 * @note    Do not invoke this API from within a kernel lock.
 *
 * @special
 */
void chSysDisable(void) {

  port_disable();
  _dbg_check_disable();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Raises the system interrupt priority mask to system level.
 * @details The interrupt sources that should not be able to preempt the kernel
 *          are disabled, interrupt sources with higher priority are still
 *          enabled.
 * @note    Do not invoke this API from within a kernel lock.
 * @note    This API is no replacement for @p chSysLock(), the @p chSysLock()
 *          could do more than just disable the interrupts.
 *
 * @special
 */
void chSysSuspend(void) {

  port_suspend();
  _dbg_check_suspend();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Lowers the system interrupt priority mask to user level.
 * @details All the interrupt sources are enabled.
 * @note    Do not invoke this API from within a kernel lock.
 * @note    This API is no replacement for @p chSysUnlock(), the
 *          @p chSysUnlock() could do more than just enable the interrupts.
 *
 * @special
 */
void chSysEnable(void) {

  _dbg_check_enable();
  port_enable();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Enters the kernel lock state.
 *
 * @special
 */
void chSysLock(void) {

  port_lock();
  _stats_start_measure_crit_thd();
  _dbg_check_lock();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Leaves the kernel lock state.
 *
 * @special
 */
void chSysUnlock(void) {

  _dbg_check_unlock();
  _stats_stop_measure_crit_thd();

  /* The following condition can be triggered by the use of i-class functions
     in a critical section not followed by a chSchResceduleS(), this means
     that the current thread has a lower priority than the next thread in
     the ready list.*/
  chDbgAssert((ch.rlist.queue.next == (thread_t *)&ch.rlist.queue) ||
              (ch.rlist.current->prio >= ch.rlist.queue.next->prio),
              "priority order violation");

  port_unlock();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Enters the kernel lock state from within an interrupt handler.
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API before invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 *
 * @special
 */
void chSysLockFromISR(void) {

  port_lock_from_isr();
  _stats_start_measure_crit_isr();
  _dbg_check_lock_from_isr();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Leaves the kernel lock state from within an interrupt handler.
 *
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API after invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 *
 * @special
 */
void chSysUnlockFromISR(void) {

  _dbg_check_unlock_from_isr();
  _stats_stop_measure_crit_isr();
  port_unlock_from_isr();
  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Unconditionally enters the kernel lock state.
 * @note    Can be called without previous knowledge of the current lock state.
 *          The final state is "s-locked".
 *
 * @special
 */
void chSysUnconditionalLock(void) {

  if (port_irq_enabled(port_get_irq_status())) {
    chSysLock();
  }
  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Unconditionally leaves the kernel lock state.
 * @note    Can be called without previous knowledge of the current lock state.
 *          The final state is "normal".
 *
 * @special
 */
void chSysUnconditionalUnlock(void) {

  if (!port_irq_enabled(port_get_irq_status())) {
    chSysUnlock();
  }
  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   Returns a pointer to the idle thread.
 * @pre     In order to use this function the option @p CH_CFG_NO_IDLE_THREAD
 *          must be disabled.
 * @note    The reference counter of the idle thread is not incremented but
 *          it is not strictly required being the idle thread a static
 *          object.
 *
 * @return              Pointer to the idle thread.
 *
 * @xclass
 */
thread_t *chSysGetIdleThreadX(void) {
  auditbox_try_update_buffer(__func__, true); // CHAOS audit
  return ch.rlist.queue.prev;
}
#endif /* CH_CFG_NO_IDLE_THREAD == FALSE */

/** @} */
