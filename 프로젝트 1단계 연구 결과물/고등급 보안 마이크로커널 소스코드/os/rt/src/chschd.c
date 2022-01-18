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
 * @file    chschd.c
 * @brief   Scheduler code.
 *
 * @addtogroup scheduler
 * @details This module provides the default portable scheduler code.
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   System data structures.
 */
ch_system_t ch;

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
/* Macro functions.                                                          */
/*===========================================================================*/

/**
 * @name    Wakeup status codes
 * @{
 */
msg_t MSG_OK(void) {
  return (msg_t)0;
}
msg_t MSG_TIMEOUT(void) {
  return (msg_t)-1;
}
msg_t MSG_RESET(void) {
  return (msg_t)-2;
}

/** @} */

/**
 * @name    Priority constants
 * @{
 */
tprio_t NOPRIO(void) {
  return (tprio_t)0;
}
tprio_t IDLEPRIO(void) {
  return (tprio_t)1;
}
tprio_t LOWPRIO(void) {
  return (tprio_t)2;
}
tprio_t NORMALPRIO(void) {
  return (tprio_t)128;
}
tprio_t HIGHPRIO(void) {
  return (tprio_t)255;
}

/** @} */

/**
 * @name    Thread states
 * @{
 */

tstate_t CH_STATE_READY(void) {
  return (tstate_t)0;
}
tstate_t CH_STATE_CURRENT(void) {
  return (tstate_t)1;
}
tstate_t CH_STATE_WTSTART(void) {
  return (tstate_t)2;
}
tstate_t CH_STATE_SUSPENDED(void) {
  return (tstate_t)3;
}
tstate_t CH_STATE_QUEUED(void) {
  return (tstate_t)4;
}
tstate_t CH_STATE_WTSEM(void) {
  return (tstate_t)5;
}
tstate_t CH_STATE_WTMTX(void) {
  return (tstate_t)6;
}
tstate_t CH_STATE_WTCOND(void) {
  return (tstate_t)7;
}
tstate_t CH_STATE_SLEEPING(void) {
  return (tstate_t)8;
}
tstate_t CH_STATE_WTEXIT(void) {
  return (tstate_t)9;
}
tstate_t CH_STATE_WTOREVT(void) {
  return (tstate_t)10;
}
tstate_t CH_STATE_WTANDEVT(void) {
  return (tstate_t)11;
}
tstate_t CH_STATE_SNDMSGQ(void) {
  return (tstate_t)12;
}
tstate_t CH_STATE_SNDMSG(void) {
  return (tstate_t)13;
}
tstate_t CH_STATE_WTMSG(void) {
  return (tstate_t)14;
}
tstate_t CH_STATE_FINAL(void) {
  return (tstate_t)15;
}

/** @} */

/**
 * @name    Thread flags and attributes
 * @{
 */

tmode_t CH_FLAG_MODE_MASK(void) {
  return (tmode_t)3U;
}
tmode_t CH_FLAG_MODE_STATIC(void) {
  return (tmode_t)0U;
}
tmode_t CH_FLAG_MODE_HEAP(void) {
  return (tmode_t)1U;
}
tmode_t CH_FLAG_MODE_MPOOL(void) {
  return (tmode_t)2U;
}
tmode_t CH_FLAG_TERMINATE(void) {
  return (tmode_t)4U;
}
/** @} */

tprio_t firstprio(ready_list_t *rlp) {
  return thread_get_prio(tqueue_get_next(rlist_get_queue_ptr(rlp)));
}

thread_t *getCurrp(void) {
  return ch.rlist.current;
}

void setCurrp(thread_t *newthread) {
  ch.rlist.current = newthread;
}

/*===========================================================================*/
/* Encapsulation functions.                                                  */
/*===========================================================================*/

// struct ch_threads_list

thread_t *tlist_get_next(threads_list_t *tlp) {
  return tlp->next;
}

void tlist_set_next(threads_list_t *tlp, thread_t *next) {
  tlp->next = next;
}

/** @} */

// struct ch_threads_queue

thread_t *tqueue_get_next(threads_queue_t *tqp) {
  return tqp->next;
}

void tqueue_set_next(threads_queue_t *tqp, thread_t *next) {
  tqp->next = next;
}

thread_t *tqueue_get_prev(threads_queue_t *tqp) {
  return tqp->prev;
}

void tqueue_set_prev(threads_queue_t *tqp, thread_t *prev) {
  tqp->prev = prev;
}

/** @} */

// struct ch_thread

threads_queue_t *thread_get_queue_ptr(thread_t *tp) {
  return &(tp->queue);
}

void thread_set_queue(thread_t *tp, threads_queue_t queue) {
  tp->queue = queue;
}

tprio_t thread_get_prio(thread_t *tp) {
  return tp->prio;
}

void thread_set_prio(thread_t *tp, tprio_t prio) {
  tp->prio = prio;
}

thread_t *thread_get_newer(thread_t *tp) {
  return tp->newer;
}

void thread_set_newer(thread_t *tp, thread_t *newer) {
  tp->newer = newer;
}

thread_t *thread_get_older(thread_t *tp) {
  return tp->older;
}

void thread_set_older(thread_t *tp, thread_t *older) {
  tp->newer = older;
}

const char *thread_get_name(thread_t *tp) {
  return tp->name;
}

void thread_set_name(thread_t *tp, const char *name) {
  tp->name = name;
}

stkalign_t *thread_get_wabase(thread_t *tp) {
  return tp->wabase;
}

void thread_set_wabase(thread_t *tp, stkalign_t *wabase) {
  tp->wabase = wabase;
}

tstate_t thread_get_state(thread_t *tp) {
  return tp->state;
}

void thread_set_state(thread_t *tp, tstate_t state) {
  tp->state = state;
}

tmode_t thread_get_flags(thread_t *tp) {
  return tp->flags;
}

void thread_set_flags(thread_t *tp, tmode_t flags) {
  tp->flags = flags;
}

trefs_t thread_get_refs(thread_t *tp) {
  return tp->refs;
}

void thread_set_refs(thread_t *tp, trefs_t refs) {
  tp->refs = refs;
}
#if (CH_CFG_TIME_QUANTUM > 0) || defined(__DOXYGEN__)
tslices_t thread_get_ticks(thread_t *tp) {
  return tp->ticks;
}

void thread_set_ticks(thread_t *tp, tslices_t ticks) {
  tp->ticks = ticks;
}
#endif
msg_t thread_get_rdymsg(thread_t *tp) {
  return tp->u.rdymsg;
}

void thread_set_rdymsg(thread_t *tp, msg_t msg) {
  tp->u.rdymsg = msg;
}

msg_t thread_get_exitcode(thread_t *tp) {
  return tp->u.exitcode;
}

void thread_set_exitcode(thread_t *tp, msg_t msg) {
  tp->u.exitcode = msg;
}

msg_t *thread_get_wtobjp(thread_t *tp) {
  return tp->u.wtobjp;
}

void thread_set_wtobjp(thread_t *tp, msg_t *wtobjp) {
  tp->u.wtobjp = wtobjp;
}

thread_reference_t *thread_get_wttrp(thread_t *tp) {
  return tp->u.wttrp;
}

void thread_set_wttrp(thread_t *tp, thread_reference_t *wttrp) {
  tp->u.wttrp = wttrp;
}

msg_t thread_get_sentmsg(thread_t *tp) {
  return tp->u.sentmsg;
}

void thread_set_sentmsg(thread_t *tp, msg_t msg) {
  tp->u.sentmsg = msg;
}

struct ch_semaphore *thread_get_wtsemp(thread_t *tp) {
  return tp->u.wtsemp;
}

void thread_set_wtsemp(thread_t *tp, struct ch_semaphore *wtsemp) {
  tp->u.wtsemp = wtsemp;
}

struct ch_mutex *thread_get_wtmtxp(thread_t *tp) {
  return tp->u.wtmtxp;
}

void thread_set_wtmtxp(thread_t *tp, struct ch_mutex *wtmtxp) {
  tp->u.wtmtxp = wtmtxp;
}

eventmask_t thread_get_ewmask(thread_t *tp) {
  return tp->u.ewmask;
}

void thread_set_ewmask(thread_t *tp, eventmask_t ewmask) {
  tp->u.ewmask = ewmask;
}

threads_list_t *thread_get_waiting_ptr(thread_t *tp) {
  return &(tp->waiting);
}

void thread_set_waiting(thread_t *tp, threads_list_t waiting) {
  tp->waiting = waiting;
}

threads_queue_t *thread_get_msgqueue_ptr(thread_t *tp) {
  return &(tp->msgqueue);
}

void thread_set_msgqueue(thread_t *tp, threads_queue_t msgqueue) {
  tp->msgqueue = msgqueue;
}

eventmask_t thread_get_epending(thread_t *tp) {
  return tp->epending;
}

void thread_set_epending(thread_t *tp, eventmask_t epending) {
  tp->epending = epending;
}

struct ch_mutex *thread_get_mtxlist(thread_t *tp) {
  return tp->mtxlist;
}

void thread_set_mtxlist(thread_t *tp, struct ch_mutex *mtxlist) {
  tp->mtxlist = mtxlist;
}

tprio_t thread_get_realprio(thread_t *tp) {
  return tp->realprio;
}

void thread_set_realprio(thread_t *tp, tprio_t realprio) {
  tp->realprio = realprio;
}

void *thread_get_mpool(thread_t *tp) {
  return tp->mpool;
}

void thread_set_mpool(thread_t *tp, void *mpool) {
  tp->mpool = mpool;
}

#if (CH_DBG_STATISTICS == TRUE) || defined(__DOXYGEN__)
time_measurement_t *thread_get_stats_ptr(thread_t *tp) {
  return &(tp->stats);
}

void thread_set_stats(thread_t *tp, time_measurement_t stats) {
  tp->stats = stats;
}
#endif

// CHAOS에서 추가됨
stkalign_t * thread_get_start_addr(thread_t *tp) { // TODO: 자료형 확인
  return tp->start_addr;
}

void thread_set_start_addr(thread_t *tp, stkalign_t *start_addr) {
  tp->start_addr = start_addr;
}

stkalign_t * thread_get_end_addr(thread_t *tp) {
  return tp->end_addr;
}

void thread_set_end_addr(thread_t *tp, stkalign_t *end_addr) {
  tp->end_addr = end_addr;
}

ID_t thread_get_id_thd(thread_t *tp) {
  return tp->id_thd;
}

void thread_set_id_thd(thread_t *tp, ID_t id_thd) {
  tp->id_thd = id_thd;
}

SecretValue_t thread_get_pass_thd(thread_t *tp) {
  return tp->pass_thd;
}

void thread_set_pass_thd(thread_t *tp, SecretValue_t pass_thd) {
  tp->pass_thd = pass_thd;
}





/** @} */

// struct ch_ready_list

threads_queue_t *rlist_get_queue_ptr(ready_list_t *rlist) {
  return &(rlist->queue);
}

void rlist_set_queue(ready_list_t *rlist, threads_queue_t queue) {
  rlist->queue = queue;
}

tprio_t rlist_get_prio(ready_list_t *rlist) {
  return rlist->prio;
}

void rlist_set_prio(ready_list_t *rlist, tprio_t prio) {
  rlist->prio = prio;
}

thread_t *rlist_get_newer(ready_list_t *rlist) {
  return rlist->newer;
}

void rlist_set_newer(ready_list_t *rlist, thread_t *newer) {
  rlist->newer = newer;
}

thread_t *rlist_get_older(ready_list_t *rlist) {
  return rlist->older;
}

void rlist_set_older(ready_list_t *rlist, thread_t *older) {
  rlist->older = older;
}

thread_t *rlist_get_current(ready_list_t *rlist) {
  return rlist->current;
}

void rlist_set_current(ready_list_t *rlist, thread_t *current) {
  rlist->current = current;
}

/** @} */

  // functions for struct system_debug_t
  const char * dbg_get_panic_msg(system_debug_t *dbg) {
    return dbg->panic_msg;
  }
  void dbg_set_panic_msg(system_debug_t *dbg, const char * volatile panic_msg) {
    dbg->panic_msg = panic_msg;
  }

#if (CH_DBG_SYSTEM_STATE_CHECK == TRUE) || defined(__DOXYGEN__)
  cnt_t dbg_get_isr_cnt(system_debug_t *dbg) {
    return dbg->isr_cnt;
  }
  void dbg_set_isr_cnt(system_debug_t *dbg, cnt_t isr_cnt) {
    dbg->isr_cnt = isr_cnt;
  }

  cnt_t dbg_get_lock_cnt(system_debug_t *dbg) {
    return dbg->lock_cnt;
  }
  void dbg_set_lock_cnt(system_debug_t *dbg, cnt_t lock_cnt){
    dbg->lock_cnt = lock_cnt;
  }
#endif

#if (CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_DISABLED) || defined(__DOXYGEN__)
  ch_trace_buffer_t * dbg_get_trace_buffer_ptr(system_debug_t *dbg) {
    return &(dbg->trace_buffer);
  }
  void dbg_set_trace_buffer(system_debug_t *dbg, ch_trace_buffer_t trace_buffer) {
    dbg->trace_buffer = trace_buffer;
  }
#endif

// struct ch_system_t -> ch
ready_list_t *ch_get_rlist_ptr(void) {
  return &(ch.rlist);
}

virtual_timers_list_t *ch_get_vtlist_ptr(void) {
  return &(ch.vtlist);
}

system_debug_t *ch_get_dbg_ptr(void) {
  return &(ch.dbg); 
}

thread_t *ch_get_mainthread_ptr(void) {
  return &(ch.mainthread);
}

tm_calibration_t *ch_get_tm_ptr(void) {
  return &(ch.tm);
}
#if (CH_DBG_STATISTICS == TRUE) || defined(__DOXYGEN__)
kernel_stats_t *ch_get_kernel_stats_ptr(void) {
  return &(ch.kernel_stats);
}
#endif

/** @} */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Scheduler initialization.
 *
 * @notapi
 */
void _scheduler_init(void) {

  queue_init(rlist_get_queue_ptr(ch_get_rlist_ptr()));
  // ch.rlist.prio = NOPRIO();
  rlist_set_prio(ch_get_rlist_ptr(), NOPRIO());
#if CH_CFG_USE_REGISTRY == TRUE
  // ch.rlist.newer = (thread_t *)&ch.rlist;
  // ch.rlist.older = (thread_t *)&ch.rlist;
  rlist_set_newer(ch_get_rlist_ptr(), (thread_t *)ch_get_rlist_ptr());
  rlist_set_older(ch_get_rlist_ptr(), (thread_t *)ch_get_rlist_ptr());
#endif
}

#if (CH_CFG_OPTIMIZE_SPEED == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   Inserts a thread into a priority ordered queue.
 * @note    The insertion is done by scanning the list from the highest
 *          priority toward the lowest.
 *
 * @param[in] tp        the pointer to the thread to be inserted in the list
 * @param[in] tqp       the pointer to the threads list header
 *
 * @notapi
 */
void queue_prio_insert(thread_t *tp, threads_queue_t *tqp) {

  thread_t *cp = (thread_t *)tqp;
  do {
    cp = tqueue_get_next(thread_get_queue_ptr(cp));
  } while ((cp != (thread_t *)tqp) && (thread_get_prio(cp) >= thread_get_prio(tp)));
  // tp->queue.next             = cp;
  // tp->queue.prev             = cp->queue.prev;
  // tp->queue.prev->queue.next = tp;
  // cp->queue.prev             = tp;
  tqueue_set_next(thread_get_queue_ptr(tp), cp);
  tqueue_set_prev(thread_get_queue_ptr(tp), tqueue_get_prev(thread_get_queue_ptr(cp)));
  tqueue_set_next(thread_get_queue_ptr(tqueue_get_prev(thread_get_queue_ptr(tp))), tp);
  tqueue_set_prev(thread_get_queue_ptr(cp), tp);
}

/**
 * @brief   Inserts a thread into a queue.
 *
 * @param[in] tp        the pointer to the thread to be inserted in the list
 * @param[in] tqp       the pointer to the threads list header
 *
 * @notapi
 */
void queue_insert(thread_t *tp, threads_queue_t *tqp) {

  // tp->queue.next             = (thread_t *)tqp;
  // tp->queue.prev             = tqp->prev;
  // tp->queue.prev->queue.next = tp;
  // tqp->prev                  = tp;

  tqueue_set_next(thread_get_queue_ptr(tp), (thread_t *)tqp);
  tqueue_set_prev(thread_get_queue_ptr(tp), tqueue_get_prev(tqp));
  tqueue_set_next(thread_get_queue_ptr(tqueue_get_prev(thread_get_queue_ptr(tp))), tp);
  tqueue_set_prev(tqp, tp);
}

/**
 * @brief   Removes the first-out thread from a queue and returns it.
 * @note    If the queue is priority ordered then this function returns the
 *          thread with the highest priority.
 *
 * @param[in] tqp       the pointer to the threads list header
 * @return              The removed thread pointer.
 *
 * @notapi
 */
thread_t *queue_fifo_remove(threads_queue_t *tqp) {
  thread_t *tp = tqueue_get_next(tqp);
  printf("queue_fifo_remove tp->name: %s\n", tp->name);
  // tqp->next             = tp->queue.next;
  // tqp->next->queue.prev = (thread_t *)tqp;
  tqueue_set_next(tqp, tqueue_get_next(thread_get_queue_ptr(tp)));
  tqueue_set_prev(thread_get_queue_ptr(tqueue_get_next(tqp)), (thread_t *)tqp);
  return tp;
}

/**
 * @brief   Removes the last-out thread from a queue and returns it.
 * @note    If the queue is priority ordered then this function returns the
 *          thread with the lowest priority.
 *
 * @param[in] tqp   the pointer to the threads list header
 * @return          The removed thread pointer.
 *
 * @notapi
 */
thread_t *queue_lifo_remove(threads_queue_t *tqp) {
  thread_t *tp = tqp->prev;
  //thread_t *tp = tqueue_get_prev(tqp);
  // tqp->prev             = tp->queue.prev;
  // tqp->prev->queue.next = (thread_t *)tqp;
  tqueue_set_prev(tqp, tqueue_get_prev(thread_get_queue_ptr(tp)));
  tqueue_set_next(thread_get_queue_ptr(tqueue_get_prev(tqp)), (thread_t *)tqp);
  return tp;
}

/**
 * @brief   Removes a thread from a queue and returns it.
 * @details The thread is removed from the queue regardless of its relative
 *          position and regardless the used insertion method.
 *
 * @param[in] tp        the pointer to the thread to be removed from the queue
 * @return              The removed thread pointer.
 *
 * @notapi
 */
thread_t *queue_dequeue(thread_t *tp) {

  // tp->queue.prev->queue.next = tp->queue.next;
  // tp->queue.next->queue.prev = tp->queue.prev;
  tqueue_set_next(thread_get_queue_ptr(tqueue_get_prev(thread_get_queue_ptr(tp))), tqueue_get_next(thread_get_queue_ptr(tp)));
  tqueue_set_prev(thread_get_queue_ptr(tqueue_get_next(thread_get_queue_ptr(tp))), tqueue_get_prev(thread_get_queue_ptr(tp)));
  return tp;
}

/**
 * @brief   Pushes a thread_t on top of a stack list.
 *
 * @param[in] tp    the pointer to the thread to be inserted in the list
 * @param[in] tlp   the pointer to the threads list header
 *
 * @notapi
 */
void list_insert(thread_t *tp, threads_list_t *tlp) {

  // tp->queue.next = tlp->next;
  // tlp->next      = tp;

  tqueue_set_next(thread_get_queue_ptr(tp), tlist_get_next(tlp));
  tlist_set_next(tlp, tp)
}

/**
 * @brief   Pops a thread from the top of a stack list and returns it.
 * @pre     The list must be non-empty before calling this function.
 *
 * @param[in] tlp       the pointer to the threads list header
 * @return              The removed thread pointer.
 *
 * @notapi
 */
thread_t *list_remove(threads_list_t *tlp) {

  // thread_t *tp = tlp->next;
  // tlp->next = tp->queue.next;

  thread_t *tp = tlist_get_next(tlp);
  tlist_set_next(tlp, tqueue_get_next(thread_get_queue_ptr(tp)));
  return tp;
}
#endif /* CH_CFG_OPTIMIZE_SPEED */

/**
 * @brief   Inserts a thread in the Ready List placing it behind its peers.
 * @details The thread is positioned behind all threads with higher or equal
 *          priority.
 * @pre     The thread must not be already inserted in any list through its
 *          @p next and @p prev or list corruption would occur.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] tp        the thread to be made ready
 * @return              The thread pointer.
 *
 * @iclass
 */
thread_t *chSchReadyI(SecretValue_t kValue, thread_t *tp) {

  if (kValue == get_kernel_secret()) {
    //printf("chSchReadyI Start\n");
    thread_t *cp;

    chDbgCheckClassI();
    chDbgCheck(tp != NULL);
    chDbgAssert((thread_get_state(tp) != CH_STATE_READY()) &&
                (thread_get_state(tp) != CH_STATE_FINAL()),
                "invalid state");

    //tp->state = CH_STATE_READY();
    thread_set_state(tp, CH_STATE_READY());
    //cp = (thread_t *)&ch.rlist.queue;
    cp = (thread_t *)rlist_get_queue_ptr(ch_get_rlist_ptr());
    do {
      //cp = cp->queue.next;
      cp = tqueue_get_next(thread_get_queue_ptr(cp));
    //} while (cp->prio >= tp->prio);
    } while ((thread_get_prio(cp) >= thread_get_prio(tp)));
    /* Insertion on prev.*/

    // tp->queue.next             = cp;
    // tp->queue.prev             = cp->queue.prev;
    // tp->queue.prev->queue.next = tp;
    // cp->queue.prev             = tp;
    tqueue_set_next(thread_get_queue_ptr(tp), cp);
    tqueue_set_prev(thread_get_queue_ptr(tp), tqueue_get_prev(thread_get_queue_ptr(cp)));
    tqueue_set_next(thread_get_queue_ptr(tqueue_get_prev(thread_get_queue_ptr(tp))), tp);
    tqueue_set_prev(thread_get_queue_ptr(cp), tp);
    //printf("chSchReadyI end\n");
    return tp; 
  }
  else {
    //printf("chSchReadyI false\n");
    return tp;
  }
}

/**
 * @brief   Inserts a thread in the Ready List placing it ahead its peers.
 * @details The thread is positioned ahead all threads with higher or equal
 *          priority.
 * @pre     The thread must not be already inserted in any list through its
 *          @p next and @p prev or list corruption would occur.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] tp        the thread to be made ready
 * @return              The thread pointer.
 *
 * @iclass
 */
thread_t *chSchReadyAheadI(SecretValue_t kValue, thread_t *tp) {

  if (kValue == get_kernel_secret()) {
    thread_t *cp;

    chDbgCheckClassI();
    chDbgCheck(tp != NULL);
    chDbgAssert((thread_get_state(tp) != CH_STATE_READY()) &&
                (thread_get_state(tp) != CH_STATE_FINAL()),
                "invalid state");

    //tp->state = CH_STATE_READY();
    thread_set_state(tp, CH_STATE_READY());
    cp = (thread_t *)&ch.rlist.queue;
    do {
      //cp = cp->queue.next;
      cp = tqueue_get_next(thread_get_queue_ptr(cp));
    //} while (cp->prio > tp->prio);
    } while ((thread_get_prio(cp) > thread_get_prio(tp)));
    /* Insertion on prev.*/
    //tp->queue.next             = cp;
    //tp->queue.prev             = cp->queue.prev;
    //tp->queue.prev->queue.next = tp;
    //cp->queue.prev             = tp;
    tqueue_set_next(thread_get_queue_ptr(tp), cp);
    tqueue_set_prev(thread_get_queue_ptr(tp), tqueue_get_prev(thread_get_queue_ptr(cp)));
    tqueue_set_next(thread_get_queue_ptr(tqueue_get_prev(thread_get_queue_ptr(tp))), tp);
    tqueue_set_prev(thread_get_queue_ptr(cp), tp);

    return tp;
  }
  else {
    return tp;
  }
}

/**
 * @brief   Puts the current thread to sleep into the specified state.
 * @details The thread goes into a sleeping state. The possible
 *          @ref thread_states are defined into @p threads.h.
 *
 * @param[in] newstate  the new thread state
 *
 * @sclass
 */
void chSchGoSleepS(tstate_t newstate) {
  thread_t *otp = getCurrp();

  chDbgCheckClassS();
  /* New state.*/
  //otp->state = newstate;
  thread_set_state(otp, newstate);
#if CH_CFG_TIME_QUANTUM > 0
  /* The thread is renouncing its remaining time slices so it will have a new
     time quantum when it will wakeup.*/
  //otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
  thread_set_ticks(otp, (tslices_t)CH_CFG_TIME_QUANTUM);
#endif

  /* Next thread in ready list becomes current.*/
  //setCurrp(queue_fifo_remove(rlist_get_queue_ptr(ch_get_rlist_ptr())));
  setCurrp(queue_fifo_remove(&ch.rlist.queue));
  thread_set_state(getCurrp(), CH_STATE_CURRENT());

  /* Handling idle-enter hook.*/
  if (thread_get_prio(getCurrp()) == IDLEPRIO()) {
    CH_CFG_IDLE_ENTER_HOOK();
  }
  
  /* Swap operation as tail call.*/
  chSysSwitch(getCurrp(), otp);
}

/*
 * Timeout wakeup callback.
 */
static void wakeup(void *p) {
  thread_t *tp = (thread_t *)p;

  chSysLockFromISR();
  switch (thread_get_state(tp)) {
  case (tstate_t)0:
    /* Handling the special case where the thread has been made ready by
       another thread with higher priority.*/
    chSysUnlockFromISR();
    return;
  case (tstate_t)3:
    *tp->u.wttrp = NULL;
    break;
#if CH_CFG_USE_SEMAPHORES == TRUE
  case (tstate_t)5:
    chSemFastSignalI(tp->u.wtsemp);
#endif
    /* Falls through.*/
  case (tstate_t)4:
    /* Falls through.*/
#if (CH_CFG_USE_CONDVARS == TRUE) && (CH_CFG_USE_CONDVARS_TIMEOUT == TRUE)
  case (tstate_t)7:
#endif
    /* States requiring dequeuing.*/
    (void) queue_dequeue(tp);
    break;
  default:
    /* Any other state, nothing to do.*/
    break;
  }
  thread_set_rdymsg(tp, MSG_TIMEOUT());
  //tp->u.rdymsg = MSG_TIMEOUT();
  (void) chSchReadyI(secret_kValue, tp);
  chSysUnlockFromISR();
}

/**
 * @brief   Puts the current thread to sleep into the specified state with
 *          timeout specification.
 * @details The thread goes into a sleeping state, if it is not awakened
 *          explicitly within the specified timeout then it is forcibly
 *          awakened with a @p MSG_TIMEOUT low level message. The possible
 *          @ref thread_states are defined into @p threads.h.
 *
 * @param[in] newstate  the new thread state
 * @param[in] timeout   the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state, this is equivalent to invoking
 *                        @p chSchGoSleepS() but, of course, less efficient.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @return              The wakeup message.
 * @retval MSG_TIMEOUT  if a timeout occurs.
 *
 * @sclass
 */
msg_t chSchGoSleepTimeoutS(tstate_t newstate, sysinterval_t timeout) {

  chDbgCheckClassS();

  if (TIME_INFINITE != timeout) {
    virtual_timer_t vt;

    chVTDoSetI(&vt, timeout, wakeup, getCurrp());
    chSchGoSleepS(newstate);
    if (chVTIsArmedI(&vt)) {
      chVTDoResetI(&vt);
    }
  }
  else {
    chSchGoSleepS(newstate);
  }

  return thread_get_rdymsg(getCurrp());
}

/**
 * @brief   Wakes up a thread.
 * @details The thread is inserted into the ready list or immediately made
 *          running depending on its relative priority compared to the current
 *          thread.
 * @pre     The thread must not be already inserted in any list through its
 *          @p next and @p prev or list corruption would occur.
 * @note    It is equivalent to a @p chSchReadyI() followed by a
 *          @p chSchRescheduleS() but much more efficient.
 * @note    The function assumes that the current thread has the highest
 *          priority.
 *
 * @param[in] ntp       the thread to be made ready
 * @param[in] msg       the wakeup message
 *
 * @sclass
 */
void chSchWakeupS(SecretValue_t kValue, thread_t *ntp, msg_t msg) {
  //printf("chSchWakeupS start\n");
  thread_t *otp = getCurrp();
  if (kValue == get_kernel_secret()) {
    chDbgCheckClassS();

    chDbgAssert((tqueue_get_next(rlist_get_queue_ptr(ch_get_rlist_ptr())) == (thread_t *)rlist_get_queue_ptr(ch_get_rlist_ptr())) ||
                (thread_get_prio(rlist_get_current(ch_get_rlist_ptr())) >= thread_get_prio(tqueue_get_next(rlist_get_queue_ptr(ch_get_rlist_ptr())))),
                "priority order violation");

    /* Storing the message to be retrieved by the target thread when it will
       restart execution.*/
    //ntp->u.rdymsg = msg;
    thread_set_rdymsg(ntp, msg);
    /* If the waken thread has a not-greater priority than the current
       one then it is just inserted in the ready list else it made
       running immediately and the invoking thread goes in the ready
       list instead.*/
    if (thread_get_prio(ntp) <= thread_get_prio(otp)) {
      (void) chSchReadyI(secret_kValue, ntp);
    }
    else {
      otp = chSchReadyI(secret_kValue, otp);
      /* Handling idle-leave hook.*/
      if (thread_get_prio(otp) == IDLEPRIO()) {
        CH_CFG_IDLE_LEAVE_HOOK();
      }

      /* The extracted thread is marked as current.*/
      setCurrp(ntp);
      thread_set_state(ntp, CH_STATE_CURRENT());
      /* Swap operation as tail call.*/
      chSysSwitch(ntp, otp);
    }    
  }
  //printf("chSchWakeupS - end\n");
}

/**
 * @brief   Performs a reschedule if a higher priority thread is runnable.
 * @details If a thread with a higher priority than the current thread is in
 *          the ready list then make the higher priority thread running.
 *
 * @sclass
 */
void chSchRescheduleS(void) {

  chDbgCheckClassS();

  if (chSchIsRescRequiredI()) {
    chSchDoRescheduleAhead();
  }

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

#if !defined(CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED)
/**
 * @brief   Evaluates if preemption is required.
 * @details The decision is taken by comparing the relative priorities and
 *          depending on the state of the round robin timeout counter.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @retval true         if there is a thread that must go in running state
 *                      immediately.
 * @retval false        if preemption is not required.
 *
 * @special
 */
bool chSchIsPreemptionRequired(void) {
  //tprio_t p1 = firstprio(&ch.rlist.queue);
  tprio_t p1 = firstprio(ch_get_rlist_ptr());
  tprio_t p2 = thread_get_prio(getCurrp());

#if CH_CFG_TIME_QUANTUM > 0
  /* If the running thread has not reached its time quantum, reschedule only
     if the first thread on the ready queue has a higher priority.
     Otherwise, if the running thread has used up its time quantum, reschedule
     if the first thread on the ready queue has equal or higher priority.*/
  return (getCurrp()->ticks > (tslices_t)0) ? (p1 > p2) : (p1 >= p2);
#else
  /* If the round robin preemption feature is not enabled then performs a
     simpler comparison.*/
  return p1 > p2;
#endif
}
#endif /* !defined(CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED) */

/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind all
 *          threads having the same priority. The thread regains its time
 *          quantum.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @special
 */
void chSchDoRescheduleBehind(void) {
  thread_t *otp = getCurrp();

  /* Picks the first thread from the ready queue and makes it current.*/
  setCurrp(queue_fifo_remove(rlist_get_queue_ptr(ch_get_rlist_ptr())));
  thread_set_state(getCurrp(), CH_STATE_CURRENT());

  /* Handling idle-leave hook.*/
  if (thread_get_prio(otp) == IDLEPRIO()) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

#if CH_CFG_TIME_QUANTUM > 0
  /* It went behind peers so it gets a new time quantum.*/
  thread_set_ticks(otp, (tslices_t)CH_CFG_TIME_QUANTUM);
  //otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif

  /* Placing in ready list behind peers.*/
  otp = chSchReadyI(secret_kValue, otp);

  /* Swap operation as tail call.*/
  chSysSwitch(getCurrp(), otp);
}

/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list ahead of all
 *          threads having the same priority.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @special
 */
void chSchDoRescheduleAhead(void) {
  thread_t *otp = getCurrp();

  /* Picks the first thread from the ready queue and makes it current.*/
  setCurrp(queue_fifo_remove(rlist_get_queue_ptr(ch_get_rlist_ptr())));
  thread_set_state(getCurrp(), CH_STATE_CURRENT());

  /* Handling idle-leave hook.*/
  if (thread_get_prio(otp) == IDLEPRIO()) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

  /* Placing in ready list ahead of peers.*/
  otp = chSchReadyAheadI(secret_kValue, otp);

  /* Swap operation as tail call.*/
  chSysSwitch(getCurrp(), otp);
}

#if !defined(CH_SCH_DO_RESCHEDULE_HOOKED)
/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind or
 *          ahead of all threads having the same priority depending on
 *          if it used its whole time slice.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @special
 */
void chSchDoReschedule(void) {
  thread_t *otp = getCurrp();

  /* Picks the first thread from the ready queue and makes it current.*/
  setCurrp(queue_fifo_remove(rlist_get_queue_ptr(ch_get_rlist_ptr())));
  thread_set_state(getCurrp(), CH_STATE_CURRENT());

  /* Handling idle-leave hook.*/
  if (thread_get_prio(otp) == IDLEPRIO()) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

#if CH_CFG_TIME_QUANTUM > 0
  /* If CH_CFG_TIME_QUANTUM is enabled then there are two different scenarios
     to handle on preemption: time quantum elapsed or not.*/
  if (thread_get_ticks(getCurrp()) == (tslices_t)0) {

    /* The thread consumed its time quantum so it is enqueued behind threads
       with same priority level, however, it acquires a new time quantum.*/
    otp = chSchReadyI(secret_kValue, otp);

    /* The thread being swapped out receives a new time quantum.*/
    thread_set_ticks(otp, (tslices_t)CH_CFG_TIME_QUANTUM);
  }
  else {
    /* The thread didn't consume all its time quantum so it is put ahead of
       threads with equal priority and does not acquire a new time quantum.*/
    otp = chSchReadyAheadI(secret_kValue, otp);
  }
#else /* !(CH_CFG_TIME_QUANTUM > 0) */
  /* If the round-robin mechanism is disabled then the thread goes always
     ahead of its peers.*/
  otp = chSchReadyAheadI(secret_kValue, otp);
#endif /* !(CH_CFG_TIME_QUANTUM > 0) */

  /* Swap operation as tail call.*/
  chSysSwitch(getCurrp(), otp);
}
#endif /*!defined(CH_SCH_DO_RESCHEDULE_HOOKED) */

/** @} */
