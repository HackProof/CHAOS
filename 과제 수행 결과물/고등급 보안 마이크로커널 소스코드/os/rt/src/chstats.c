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
 * @file    chstats.c
 * @brief   Statistics module code.
 *
 * @addtogroup statistics
 * @details Statistics services.
 * @{
 */

#include "ch.h"

#if (CH_DBG_STATISTICS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

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

ucnt_t kernel_stats_get_n_irq(kernel_stats_t * stat) {
  return stat->n_irq;
}
ucnt_t kernel_stats_get_n_ctxswc(kernel_stats_t * stat) {
  return stat->n_ctxswc;
}
time_measurement_t * kernel_stats_get_m_crit_thd_ptr(kernel_stats_t * stat) {
  return &(stat->m_crit_thd);
}
time_measurement_t * kernel_stats_get_m_crit_isr_ptr(kernel_stats_t * stat) {
  return &(stat->m_crit_isr);
}

void kernel_stats_set_n_irq(kernel_stats_t * stat, ucnt_t n_irq) {
  stat->n_irq = n_irq;
}
void kernel_stats_set_n_ctxswc(kernel_stats_t * stat, ucnt_t n_ctxswc) {
  stat->n_ctxswc = n_ctxswc;
}
void kernel_stats_set_m_crit_thd(kernel_stats_t * stat, time_measurement_t m_crit_thd) {
  stat->m_crit_thd = m_crit_thd;
}
void kernel_stats_set_m_crit_isr(kernel_stats_t * stat, time_measurement_t m_crit_isr) {
  stat->m_crit_isr = m_crit_isr;
}


/**
 * @brief   Initializes the statistics module.
 *
 * @init
 */
void _stats_init(void) {
  ch.kernel_stats.n_irq = (ucnt_t)0;
  ch.kernel_stats.n_ctxswc = (ucnt_t)0;
  chTMObjectInit(&ch.kernel_stats.m_crit_thd);
  chTMObjectInit(&ch.kernel_stats.m_crit_isr);

  // kernel_stats_set_n_irq(ch_get_kernel_stats_ptr(), (ucnt_t)0);
  // kernel_stats_set_n_ctxswc(ch_get_kernel_stats_ptr(), (ucnt_t)0);
  // chTMObjectInit(kernel_stats_get_m_crit_thd_ptr(ch_get_kernel_stats_ptr()));
  // chTMObjectInit(kernel_stats_get_m_crit_isr_ptr(ch_get_kernel_stats_ptr()));
}

/**
 * @brief   Increases the IRQ counter.
 */
void _stats_increase_irq(void) {

  port_lock_from_isr();
  ch.kernel_stats.n_irq++;
  // kernel_stats_set_n_irq(ch_get_kernel_stats_ptr(), kernel_stats_get_n_irq(ch_get_kernel_stats_ptr()) + 1);
  port_unlock_from_isr();
}

/**
 * @brief   Updates context switch related statistics.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
void _stats_ctxswc(thread_t *ntp, thread_t *otp) {
  ch.kernel_stats.n_ctxswc++;
  chTMChainMeasurementToX(&otp->stats, &ntp->stats);

  // kernel_stats_set_n_ctxswc(ch_get_kernel_stats_ptr(), kernel_stats_get_n_ctxswc(ch_get_kernel_stats_ptr()) + 1);
  // chTMChainMeasurementToX(thread_get_stats_ptr(otp), thread_get_stats_ptr(ntp)); // TODO
}

/**
 * @brief   Starts the measurement of a thread critical zone.
 */
void _stats_start_measure_crit_thd(void) {

  chTMStartMeasurementX(&ch.kernel_stats.m_crit_thd);
  // chTMStartMeasurementX(kernel_stats_get_m_crit_thd_ptr(ch_get_kernel_stats_ptr()));
}

/**
 * @brief   Stops the measurement of a thread critical zone.
 */
void _stats_stop_measure_crit_thd(void) {

  chTMStopMeasurementX(&ch.kernel_stats.m_crit_thd);
  // chTMStopMeasurementX(kernel_stats_get_m_crit_thd_ptr(ch_get_kernel_stats_ptr()));
}

/**
 * @brief   Starts the measurement of an ISR critical zone.
 */
void _stats_start_measure_crit_isr(void) {

  chTMStartMeasurementX(&ch.kernel_stats.m_crit_isr);
  // chTMStartMeasurementX(kernel_stats_get_m_crit_isr_ptr(ch_get_kernel_stats_ptr()));
}

/**
 * @brief   Stops the measurement of an ISR critical zone.
 */
void _stats_stop_measure_crit_isr(void) {

  chTMStopMeasurementX(&ch.kernel_stats.m_crit_isr);
  // chTMStopMeasurementX(kernel_stats_get_m_crit_isr_ptr(ch_get_kernel_stats_ptr()));
}

#endif /* CH_DBG_STATISTICS == TRUE */

/** @} */
