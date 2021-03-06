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
 * @file    SIMIA32/chcore.c
 * @brief   Simulator on IA32 port code.
 *
 * @addtogroup SIMIA32_GCC_CORE
 * @{
 */

#if defined(WIN32)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

bool port_isr_context_flag;
syssts_t port_irq_sts;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
__attribute__((used))
static void __dummy(thread_t *ntp, thread_t *otp) {
  (void)ntp; (void)otp;

  asm volatile (
#if defined(WIN32)
                ".globl @port_switch@8                          \n\t"
                "@port_switch@8:"
#elif defined(__APPLE__)
                ".globl _port_switch                            \n\t"
                "_port_switch:"
#else
                ".globl port_switch                             \n\t"
                "port_switch:"
#endif
                "push    %ebp                                   \n\t"
                "push    %esi                                   \n\t"
                "push    %edi                                   \n\t"
                "push    %ebx                                   \n\t"
                "movl    %esp, 12(%edx)                         \n\t"
                "movl    12(%ecx), %esp                         \n\t"
                "pop     %ebx                                   \n\t"
                "pop     %edi                                   \n\t"
                "pop     %esi                                   \n\t"
                "pop     %ebp                                   \n\t"
                "ret");
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
__attribute__((cdecl, noreturn))
void _port_thread_start(msg_t (*pf)(void *), void *p) {

  chSysUnlock();
  pf(p);
  chThdExit(secret_kValue, 0);
  while(1);
}


/**
 * @brief   Returns the current value of the realtime counter.
 *
 * @return              The realtime counter value.
 */
rtcnt_t port_rt_get_counter_value(void) {
#if defined(WIN32)
  LARGE_INTEGER n;

  QueryPerformanceCounter(&n);

  return (rtcnt_t)(n.QuadPart / 1000LL);
#else
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((rtcnt_t)tv.tv_sec * (rtcnt_t)1000000) + (rtcnt_t)tv.tv_usec;
#endif
}

/** @} */
