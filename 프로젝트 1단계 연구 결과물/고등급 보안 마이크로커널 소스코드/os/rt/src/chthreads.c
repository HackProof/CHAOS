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
 * @file    chthreads.c
 * @brief   Threads code.
 *
 * @addtogroup threads
 * @details Threads related APIs and services.
 *          <h2>Operation mode</h2>
 *          A thread is an abstraction of an independent instructions flow.
 *          In ChibiOS/RT a thread is represented by a "C" function owning
 *          a processor context, state informations and a dedicated stack
 *          area. In this scenario static variables are shared among all
 *          threads while automatic variables are local to the thread.<br>
 *          Operations defined for threads:
 *          - <b>Create</b>, a thread is started on the specified thread
 *            function. This operation is available in multiple variants,
 *            both static and dynamic.
 *          - <b>Exit</b>, a thread terminates by returning from its top
 *            level function or invoking a specific API, the thread can
 *            return a value that can be retrieved by other threads.
 *          - <b>Wait</b>, a thread waits for the termination of another
 *            thread and retrieves its return value.
 *          - <b>Resume</b>, a thread created in suspended state is started.
 *          - <b>Sleep</b>, the execution of a thread is suspended for the
 *            specified amount of time or the specified future absolute time
 *            is reached.
 *          - <b>SetPriority</b>, a thread changes its own priority level.
 *          - <b>Yield</b>, a thread voluntarily renounces to its time slot.
 *          .
 * @{
 */

#include "ch.h"

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
const char *td_get_name(const thread_descriptor_t *td) {
  return td->name;
}

stkalign_t *td_get_wbase(const thread_descriptor_t *td) {
  return td->wbase;
}

stkalign_t *td_get_wend(const thread_descriptor_t *td) {
  return td->wend;
}

tprio_t td_get_prio(const thread_descriptor_t *td) {
  return td->prio;
}

tfunc_t td_get_funcp(const thread_descriptor_t *td) {
  return td->funcp;
}

void *td_get_arg(const thread_descriptor_t *td) {
  return td->arg;
}

/**
 * @brief   Initializes a thread structure.
 * @note    This is an internal functions, do not use it in application code.
 *
 * @param[in] tp        pointer to the thread
 * @param[in] name      thread name
 * @param[in] prio      the priority level for the new thread
 * @return              The same thread pointer passed as parameter.
 *
 * @notapi
 */
thread_t *_thread_init(thread_t *tp, const char *name, tprio_t prio) {
  // thread_set_prio(tp, prio);
  // thread_set_state(tp, CH_STATE_WTSTART());
  // thread_set_flags(tp, CH_FLAG_MODE_STATIC());
  
  tp->prio      = prio;
  tp->state     = CH_STATE_WTSTART();
  tp->flags     = CH_FLAG_MODE_STATIC();
  
#if CH_CFG_TIME_QUANTUM > 0
  // thread_set_ticks(tp, (tslices_t)CH_CFG_TIME_QUANTUM);
  tp->ticks     = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif
#if CH_CFG_USE_MUTEXES == TRUE
  // thread_set_realprio(tp, prio);
  // thread_set_mtxlist(tp, NULL);
  tp->realprio  = prio;
  tp->mtxlist   = NULL;
#endif
#if CH_CFG_USE_EVENTS == TRUE
  // thread_set_epending(tp, (eventmask_t)0);
  tp->epending  = (eventmask_t)0;
#endif
#if CH_DBG_THREADS_PROFILING == TRUE
  // thread_set_time(tp, (systime_t)0);
  tp->time      = (systime_t)0;
#endif
#if CH_CFG_USE_REGISTRY == TRUE
  // thread_set_refs(tp, (trefs_t)1);
  // thread_set_name(tp, name);
  tp->refs      = (trefs_t)1;
  tp->name      = name;
  REG_INSERT(tp);
#else
  (void)name;
#endif
#if CH_CFG_USE_WAITEXIT == TRUE
  // list_init(thread_get_waiting_ptr(tp));
  list_init(&tp->waiting);
#endif
#if CH_CFG_USE_MESSAGES == TRUE
  // queue_init(thread_get_msgqueue_ptr(tp));
  queue_init(&tp->msgqueue);
#endif
#if CH_DBG_STATISTICS == TRUE
  // chTMObjectInit(thread_get_stats_ptr(tp));
  chTMObjectInit(&tp->stats);
#endif
  CH_CFG_THREAD_INIT_HOOK(tp);
  return tp;
}

#if (CH_DBG_FILL_THREADS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Memory fill utility.
 *
 * @param[in] startp    first address to fill
 * @param[in] endp      last address to fill +1
 * @param[in] v         filler value
 *
 * @notapi
 */
void _thread_memfill(uint8_t *startp, uint8_t *endp, uint8_t v) {

  while (startp < endp) {
    *startp++ = v;
  }
}
#endif /* CH_DBG_FILL_THREADS */

/**
 * @brief   Creates a new thread into a static memory area.
 * @details The new thread is initialized but not inserted in the ready list,
 *          the initial state is @p CH_STATE_WTSTART.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @post    The initialized thread can be subsequently started by invoking
 *          @p chThdStart(), @p chThdStartI() or @p chSchWakeupS()
 *          depending on the execution context.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @note    Threads created using this function do not obey to the
 *          @p CH_DBG_FILL_THREADS debug option because it would keep
 *          the kernel locked for too much time.
 *
 * @param[out] tdp      pointer to the thread descriptor
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @iclass
 */
thread_t *chThdCreateSuspendedI(SecretValue_t kSecret, const thread_descriptor_t *tdp) {
  //printf("chThdCreateSuspendedI start\n");
  thread_t *tp;
  IDSV_bool_t registered_idsv;
  registered_idsv = fia_register_process_id(kSecret);
  auditbox_register_id(registered_idsv.id);
	// //printf("chThdCreateSuspendedI - bool: %d\n", get_IDQueue_bool_bool(&registered_idsv));
  
  if (registered_idsv.bv) { // TODO: get, set (snd registeredIDSV)
    chDbgCheckClassI();
    chDbgCheck(tdp != NULL);
    // chDbgCheck(MEM_IS_ALIGNED(td_get_wbase(tdp), PORT_WORKING_AREA_ALIGN) &&
    //       MEM_IS_ALIGNED(td_get_wend(tdp), PORT_STACK_ALIGN) &&
    //       (td_get_wend(tdp) > td_get_wbase(tdp)) &&
    //       (((size_t)td_get_wend(tdp) - (size_t)td_get_wbase(tdp)) >= THD_WORKING_AREA_SIZE(0)));
    // chDbgCheck((td_get_prio(tdp) <= HIGHPRIO()) && (td_get_funcp(tdp) != NULL));
    chDbgCheck(MEM_IS_ALIGNED(tdp->wbase, PORT_WORKING_AREA_ALIGN) &&
            MEM_IS_ALIGNED(tdp->wend, PORT_STACK_ALIGN) &&
            (tdp->wend > tdp->wbase) &&
            (((size_t)tdp->wend - (size_t)tdp->wbase) >= THD_WORKING_AREA_SIZE(0)));
    chDbgCheck((tdp->prio <= HIGHPRIO()) && (tdp->funcp != NULL));

    /* The thread structure is laid out in the upper part of the thread
    workspace. The thread position structure is aligned to the required
    stack alignment because it represents the stack top.*/

    // make_thread 부분
    tp = (thread_t *)((uint8_t *)tdp->wend -
                  MEM_ALIGN_NEXT(sizeof (thread_t), PORT_STACK_ALIGN));
    
    // TODO: 자료형 확인
    thread_set_start_addr(tp, tdp->wend - sizeof (thread_t)); 
    thread_set_end_addr(tp, tdp->wend);
    
    // set_idsv_thread 부분
    thread_set_id_thd(tp, registered_idsv.id);
    thread_set_pass_thd(tp, registered_idsv.sv);

    #if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE)
    /* Stack boundary.*/
    // thread_set_wabase(tp, td_get_wbase(tdp));
    tp->wabase = tdp->wbase;
    #endif

    /* Setting up the port-dependent part of the working area.*/
    // PORT_SETUP_CONTEXT(tp, td_get_wbase(tdp), tp, td_get_funcp(tdp), td_get_arg(tdp));
    PORT_SETUP_CONTEXT(tp, tdp->wbase, tp, tdp->funcp, tdp->arg);

    /* The driver object is initialized but not started.*/
    // return _thread_init(tp, td_get_name(tdp), td_get_prio(tdp));
    return _thread_init(tp, tdp->name, tdp->prio);
  }
  return tp; // null thread (TODO 확인)
}

/**
 * @brief   Creates a new thread into a static memory area.
 * @details The new thread is initialized but not inserted in the ready list,
 *          the initial state is @p CH_STATE_WTSTART.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @post    The initialized thread can be subsequently started by invoking
 *          @p chThdStart(), @p chThdStartI() or @p chSchWakeupS()
 *          depending on the execution context.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 *
 * @param[out] tdp      pointer to the thread descriptor
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @api
 */
thread_t *chThdCreateSuspended(SecretValue_t kSecret, const thread_descriptor_t *tdp) {
  thread_t *tp;
  // IDQueue_bool_t registered_idsv;
  // registered_idsv = register_process_id(kSecret); // TODO 명세 확인
  //if (get_IDQueue_bool_bool(&registered_idsv) == TRUE) {
#if CH_CFG_USE_REGISTRY == TRUE
  // chDbgAssert(chRegFindThreadByWorkingArea(td_get_wbase(tdp)) == NULL,
  //             "working area in use");
  chDbgAssert(chRegFindThreadByWorkingArea(tdp->wbase) == NULL,
            "working area in use");
#endif

#if CH_DBG_FILL_THREADS == TRUE
  // _thread_memfill((uint8_t *)td_get_wbase(tdp),
  //                 (uint8_t *)td_get_wend(tdp),
  //                 CH_DBG_STACK_FILL_VALUE);
  _thread_memfill((uint8_t *)tdp->wbase,
                  (uint8_t *)tdp->wend,
                  CH_DBG_STACK_FILL_VALUE);
#endif

  chSysLock();
  tp = chThdCreateSuspendedI(kSecret, tdp);
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true);
  return tp;
	//}
  //return tp;
}

/**
 * @brief   Creates a new thread into a static memory area.
 * @details The new thread is initialized and make ready to execute.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @post    The initialized thread can be subsequently started by invoking
 *          @p chThdStart(), @p chThdStartI() or @p chSchWakeupS()
 *          depending on the execution context.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @note    Threads created using this function do not obey to the
 *          @p CH_DBG_FILL_THREADS debug option because it would keep
 *          the kernel locked for too much time.
 *
 * @param[out] tdp      pointer to the thread descriptor
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @iclass
 */
thread_t *chThdCreateI(SecretValue_t kValue, const thread_descriptor_t *tdp) {
  
  return chSchReadyI(kValue, chThdCreateSuspendedI(kValue, tdp));
}

/**
 * @brief   Creates a new thread into a static memory area.
 * @details The new thread is initialized and make ready to execute.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 *
 * @param[out] tdp      pointer to the thread descriptor
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @iclass
 */
thread_t *chThdCreate(SecretValue_t kSecret, const thread_descriptor_t *tdp) {
  //printf("chThdCreate start\n");
  thread_t *tp;

#if (CH_CFG_USE_REGISTRY == TRUE) &&                                        \
    ((CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE))
  // chDbgAssert(chRegFindThreadByWorkingArea(td_get_wbase(tdp)) == NULL,
  //             "working area in use");
  chDbgAssert(chRegFindThreadByWorkingArea(tdp->wbase) == NULL,
              "working area in use");
#endif

#if CH_DBG_FILL_THREADS == TRUE
  // _thread_memfill((uint8_t *)td_get_wbase(tdp),
  //                 (uint8_t *)td_get_wend(tdp),
  //                 CH_DBG_STACK_FILL_VALUE);
  _thread_memfill((uint8_t *)tdp->wbase,
                  (uint8_t *)tdp->wend,
                  CH_DBG_STACK_FILL_VALUE);
#endif

  chSysLock();
  tp = chThdCreateSuspendedI(kSecret, tdp);
  chSchWakeupS(kSecret, tp, MSG_OK());
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
  return tp;    
//}
  //return tp;
}

/**
 * @brief   Creates a new thread into a static memory area.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 *
 * @param[out] wsp      pointer to a working area dedicated to the thread stack
 * @param[in] size      size of the working area
 * @param[in] prio      the priority level for the new thread
 * @param[in] pf        the thread function
 * @param[in] arg       an argument passed to the thread function. It can be
 *                      @p NULL.
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @api
 */
thread_t *chThdCreateStatic(SecretValue_t kValue, SecretValue_t kSecret, 
                            void *wsp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg) {
  thread_t *tp;

  //IDQueue_bool_t registered_idsv = register_process_id(kSecret);
  //if (get_IDQueue_bool_bool(&registered_idsv) == TRUE) {  
  
  IDSV_bool_t registered_idsv;
  registered_idsv = fia_register_process_id(kSecret);
  auditbox_register_id(registered_idsv.id);
  
  if (registered_idsv.bv) { // TODO: get, set (snd registeredIDSV)

    // chDbgCheck((wsp != NULL) &&
    //           MEM_IS_ALIGNED(wsp, PORT_WORKING_AREA_ALIGN) &&
    //           (size >= THD_WORKING_AREA_SIZE(0)) &&
    //           MEM_IS_ALIGNED(size, PORT_STACK_ALIGN) &&
    //           (prio <= HIGHPRIO()) && (pf != NULL));
    chDbgCheck((wsp != NULL) &&
            MEM_IS_ALIGNED(wsp, PORT_WORKING_AREA_ALIGN) &&
            (size >= THD_WORKING_AREA_SIZE(0)) &&
            MEM_IS_ALIGNED(size, PORT_STACK_ALIGN) &&
            (prio <= HIGHPRIO) && (pf != NULL));

  #if (CH_CFG_USE_REGISTRY == TRUE) &&                                        \
      ((CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE))
    chDbgAssert(chRegFindThreadByWorkingArea(wsp) == NULL,
                "working area in use");
  #endif

  #if CH_DBG_FILL_THREADS == TRUE
    _thread_memfill((uint8_t *)wsp,
                    (uint8_t *)wsp + size,
                    CH_DBG_STACK_FILL_VALUE);
  #endif

    chSysLock();

    /* The thread structure is laid out in the upper part of the thread
      workspace. The thread position structure is aligned to the required
      stack alignment because it represents the stack top.*/
    // make_thread 부분
    tp = (thread_t *)((uint8_t *)wsp + size -
                      MEM_ALIGN_NEXT(sizeof (thread_t), PORT_STACK_ALIGN));
    thread_set_start_addr(tp, wsp + size - sizeof (thread_t)); // TODO: 자료형 확인
    thread_set_end_addr(tp, wsp + size); // TODO: 자료형 확인

    // set_idsv_thread 부분
    thread_set_id_thd(tp, registered_idsv.id);
    thread_set_pass_thd(tp, registered_idsv.sv);

  #if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE)
    /* Stack boundary.*/
    // thread_set_wabase(tp, (stkalign_t *)wsp);
    tp->wabase = (stkalign_t *)wsp;
  #endif

    /* Setting up the port-dependent part of the working area.*/
    PORT_SETUP_CONTEXT(tp, wsp, tp, pf, arg);

    tp = _thread_init(tp, "noname", prio);

    /* Starting the thread immediately.*/
    chSchWakeupS(kValue, tp, MSG_OK());
    chSysUnlock();

    // CHAOS audit
    // auditbox_try_update_buffer("chThdCreateStatic", true);
    auditbox_try_update_buffer(__func__, true); // TODO __func__가 잘 동작하는지 확인

    return tp;
  }
  return tp; // TODO: null thread?
}

/**
 * @brief   Resumes a thread created with @p chThdCreateI().
 *
 * @param[in] tp        pointer to the thread
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @api
 */
thread_t *chThdStart(SecretValue_t kValue, thread_t *tp) {

  chSysLock();
  if (kValue == get_kernel_secret()) {
    if (fia_check_id_sv(kValue, thread_get_id_thd(tp), thread_get_pass_thd(tp))) {
      // chDbgAssert(thread_get_state(tp) == CH_STATE_WTSTART(), "wrong state");
      chDbgAssert(tp->state == CH_STATE_WTSTART(), "wrong state");
      chSchWakeupS(kValue, tp, MSG_OK());
    }
  }

  chSysUnlock();
  auditbox_try_update_buffer(__func__, true);
  return tp;
}

#if (CH_CFG_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Adds a reference to a thread object.
 * @pre     The configuration option @p CH_CFG_USE_REGISTRY must be enabled in
 *          order to use this function.
 *
 * @param[in] tp        pointer to the thread
 * @return              The same thread pointer passed as parameter
 *                      representing the new reference.
 *
 * @api
 */
thread_t *chThdAddRef(thread_t *tp) {

  chSysLock();
  // chDbgAssert(thread_get_refs(tp) < (trefs_t)255, "too many references");
  // thread_set_refs(tp, thread_get_refs(tp)+1);
  chDbgAssert(tp->refs < (trefs_t)255, "too many references");
  tp->refs++;
  chSysUnlock();
  auditbox_try_update_buffer(__func__, true);
  return tp;
}

/**
 * @brief   Releases a reference to a thread object.
 * @details If the references counter reaches zero <b>and</b> the thread
 *          is in the @p CH_STATE_FINAL state then the thread's memory is
 *          returned to the proper allocator and the thread is removed
 *          from the registry.<br>
 *          Threads whose counter reaches zero and are still active become
 *          "detached" and will be removed from registry on termination.
 * @pre     The configuration option @p CH_CFG_USE_REGISTRY must be enabled in
 *          order to use this function.
 * @note    Static threads are not affected.
 *
 * @param[in] tp        pointer to the thread
 *
 * @api
 */
void chThdRelease(thread_t *tp) {

  chSysLock();
  chDbgAssert(thread_get_refs(tp) > (trefs_t)0, "not referenced");
  //tp->refs--;
  thread_set_refs(tp, thread_get_refs(tp)-1);
  /* If the references counter reaches zero and the thread is in its
     terminated state then the memory can be returned to the proper
     allocator.*/
  if ((thread_get_refs(tp) == (trefs_t)0) && (thread_get_state(tp) == CH_STATE_FINAL())) {
    REG_REMOVE(tp);
    chSysUnlock();

#if CH_CFG_USE_DYNAMIC == TRUE
    switch (thread_get_flags(tp) & CH_FLAG_MODE_MASK()) {
#if CH_CFG_USE_HEAP == TRUE
    case (tmode_t)1U:
      chHeapFree(chThdGetWorkingAreaX(tp));
      break;
#endif
#if CH_CFG_USE_MEMPOOLS == TRUE
    case (tmode_t)2U:
      chPoolFree(thread_get_mpool(tp), chThdGetWorkingAreaX(tp));
      break;
#endif
    default:
      /* Nothing else to do for static threads.*/
      break;
    }
#endif /* CH_CFG_USE_DYNAMIC == TRUE */
    return;
  }
  chSysUnlock();
  auditbox_try_update_buffer(__func__, true);
}
#endif /* CH_CFG_USE_REGISTRY == TRUE */

/**
 * @brief   Terminates the current thread.
 * @details The thread goes in the @p CH_STATE_FINAL state holding the
 *          specified exit status code, other threads can retrieve the
 *          exit status code by invoking the function @p chThdWait().
 * @post    Eventual code after this function will never be executed,
 *          this function never returns. The compiler has no way to
 *          know this so do not assume that the compiler would remove
 *          the dead code.
 *
 * @param[in] msg       thread exit code
 *
 * @api
 */
//void chThdExit(msg_t msg) {
void chThdExit(SecretValue_t kValue, msg_t msg) {
  chSysLock();
  chThdExitS(secret_kValue, msg);
  /* The thread never returns here.*/

  auditbox_try_update_buffer(__func__, true); // CHAOS audit
}

/**
 * @brief   Terminates the current thread.
 * @details The thread goes in the @p CH_STATE_FINAL state holding the
 *          specified exit status code, other threads can retrieve the
 *          exit status code by invoking the function @p chThdWait().
 * @post    Exiting a non-static thread that does not have references
 *          (detached) causes the thread to remain in the registry.
 *          It can only be removed by performing a registry scan operation.
 * @post    Eventual code after this function will never be executed,
 *          this function never returns. The compiler has no way to
 *          know this so do not assume that the compiler would remove
 *          the dead code.
 *
 * @param[in] msg       thread exit code
 *
 * @sclass
 */
void chThdExitS(SecretValue_t kValue, msg_t msg) {
  thread_t *tp = getCurrp();

  auditbox_remove_id(tp->id_thd); // TODO: check 종료되는 시점이 여기가 마지막인지

  /* Storing exit message.*/
  tp->u.exitcode = msg;

  /* Exit handler hook.*/
  CH_CFG_THREAD_EXIT_HOOK(tp);

#if CH_CFG_USE_WAITEXIT == TRUE
  /* Waking up any waiting thread.*/
  while (list_notempty(thread_get_waiting_ptr(tp))) {
    (void) chSchReadyI(kValue, list_remove(thread_get_waiting_ptr(tp)));
  }
#endif

#if CH_CFG_USE_REGISTRY == TRUE
  /* Static threads with no references are immediately removed from the
     registry because there is no memory to recover.*/
#if CH_CFG_USE_DYNAMIC == TRUE
  if ((thread_get_refs(tp) == (trefs_t)0) &&
      ((thread_get_flags(tp) & CH_FLAG_MODE_MASK()) == CH_FLAG_MODE_STATIC())) {
    REG_REMOVE(tp);
  }
#else
  if (thread_get_refs(tp) == (trefs_t)0) {
    REG_REMOVE(tp);
  }
#endif
#endif

  /* Going into final state.*/
  chSchGoSleepS(CH_STATE_FINAL());

  /* The thread never returns here.*/
  chDbgAssert(false, "zombies apocalypse");

  auditbox_try_update_buffer(__func__, true);
}

#if (CH_CFG_USE_WAITEXIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Blocks the execution of the invoking thread until the specified
 *          thread terminates then the exit code is returned.
 * @details This function waits for the specified thread to terminate then
 *          decrements its reference counter, if the counter reaches zero then
 *          the thread working area is returned to the proper allocator and
 *          the thread is removed from registry.
 * @pre     The configuration option @p CH_CFG_USE_WAITEXIT must be enabled in
 *          order to use this function.
 * @post    Enabling @p chThdWait() requires 2-4 (depending on the
 *          architecture) extra bytes in the @p thread_t structure.
 * @note    If @p CH_CFG_USE_DYNAMIC is not specified this function just waits
 *          for the thread termination, no memory allocators are involved.
 *
 * @param[in] tp        pointer to the thread
 * @return              The exit code from the terminated thread.
 *
 * @api
 */
msg_t chThdWait(SecretValue_t kValue, thread_t *tp) {
  msg_t msg;

  chDbgCheck(tp != NULL);

  chSysLock();

  if (fia_check_id_sv(kValue, thread_get_id_thd(tp), thread_get_pass_thd(tp))) {
    chDbgAssert(tp != getCurrp(), "waiting self");
#if CH_CFG_USE_REGISTRY == TRUE
    chDbgAssert(thread_get_refs(tp) > (trefs_t)0, "no references");
#endif

    if (thread_get_state(tp) != CH_STATE_FINAL()) {
      list_insert(getCurrp(), thread_get_waiting_ptr(tp));
      chSchGoSleepS(CH_STATE_WTEXIT());
    }
    msg = thread_get_exitcode(tp);
    chSysUnlock();

#if CH_CFG_USE_REGISTRY == TRUE
    /* Releasing a reference to the thread.*/
    chThdRelease(tp);
#endif
  }

  auditbox_try_update_buffer(__func__, true); // CHAOS audit

  return msg;
}
#endif /* CH_CFG_USE_WAITEXIT */

/**
 * @brief   Changes the running thread priority level then reschedules if
 *          necessary.
 * @note    The function returns the real thread priority regardless of the
 *          current priority that could be higher than the real priority
 *          because the priority inheritance mechanism.
 *
 * @param[in] newprio   the new priority level of the running thread
 * @return              The old priority level.
 *
 * @api
 */
tprio_t chThdSetPriority(tprio_t newprio) {
  tprio_t oldprio;

  chDbgCheck(newprio <= HIGHPRIO());

  chSysLock();
#if CH_CFG_USE_MUTEXES == TRUE
  oldprio = thread_get_realprio(getCurrp());
  if ((thread_get_prio(getCurrp()) == thread_get_realprio(getCurrp())) || (newprio > thread_get_prio(getCurrp()))) {
    thread_set_prio(getCurrp(), newprio);
  }
  thread_set_realprio(getCurrp(), newprio);
#else
  oldprio = thread_get_prio(getCurrp());
  thread_set_prio(getCurrp(), newprio);
#endif
  chSchRescheduleS();
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true); // CHAOS audit

  return oldprio;
}

/**
 * @brief   Requests a thread termination.
 * @pre     The target thread must be written to invoke periodically
 *          @p chThdShouldTerminate() and terminate cleanly if it returns
 *          @p true.
 * @post    The specified thread will terminate after detecting the termination
 *          condition.
 *
 * @param[in] tp        pointer to the thread
 *
 * @api
 */
void chThdTerminate(SecretValue_t kValue, thread_t *tp) {
  chSysLock();
  if (fia_check_id_sv(kValue, thread_get_id_thd(tp), thread_get_pass_thd(tp))) {
    tp->flags |= CH_FLAG_TERMINATE();
  }
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true);
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @api
 */
void chThdSleep(sysinterval_t time) { // 원래는 SecretValue_t kValue이 있어야 하지만 우선 보류 (TODO)

  chSysLock();
  chThdSleepS(0, time); // 우선 kValue를 0으로 처리
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true); // TODO __func__가 잘 동작하는지 확인
}

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 * @note    The function has no concept of "past", all specifiable times
 *          are in the future, this means that if you call this function
 *          exceeding your calculated intervals then the function will
 *          return in a far future time, not immediately.
 * @see     chThdSleepUntilWindowed()
 *
 * @param[in] time      absolute system time
 *
 * @api
 */
void chThdSleepUntil(systime_t time) { // 원래는 SecretValue_t kValue이 있어야 하지만 우선 보류 (TODO)
  sysinterval_t interval;

  chSysLock();
  interval = chTimeDiffX(chVTGetSystemTimeX(), time);
  if (interval > (sysinterval_t)0) {
    chThdSleepS(0, interval); // 우선 kValue를 0으로 처리
  }
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true);
}

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 * @note    The system time is assumed to be between @p prev and @p time
 *          else the call is assumed to have been called outside the
 *          allowed time interval, in this case no sleep is performed.
 * @see     chThdSleepUntil()
 *
 * @param[in] prev      absolute system time of the previous deadline
 * @param[in] next      absolute system time of the next deadline
 * @return              the @p next parameter
 *
 * @api
 */
systime_t chThdSleepUntilWindowed(systime_t prev, systime_t next) { // 원래는 SecretValue_t kValue이 있어야 하지만 우선 보류 (TODO)
  systime_t time;

  chSysLock();
  time = chVTGetSystemTimeX();
  if (chTimeIsInRangeX(time, prev, next)) {
    chThdSleepS(0, chTimeDiffX(time, next));
  }
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true);
  return next;
}

/**
 * @brief   Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal priority, if any.
 *
 * @api
 */
void chThdYield(void) {

  chSysLock();
  chSchDoYieldS();
  chSysUnlock();
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @return              The wake up message.
 *
 * @sclass
 */
msg_t chThdSuspendS(thread_reference_t *trp) {
  thread_t *tp = chThdGetSelfX();

  chDbgAssert(*trp == NULL, "not NULL");

  *trp = tp;
  thread_set_wttrp(tp, trp);
  //tp->u.wttrp = trp;
  chSchGoSleepS(CH_STATE_SUSPENDED());

  auditbox_try_update_buffer(__func__, true);
  return thread_get_rdymsg(chThdGetSelfX());
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] timeout   the timeout in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE the thread is not enqueued and
 *                        the function returns @p MSG_TIMEOUT as if a timeout
 *                        occurred.
 *                      .
 * @return              The wake up message.
 * @retval MSG_TIMEOUT  if the operation timed out.
 *
 * @sclass
 */
msg_t chThdSuspendTimeoutS(thread_reference_t *trp, sysinterval_t timeout) {
  thread_t *tp = chThdGetSelfX();

  chDbgAssert(*trp == NULL, "not NULL");

  if (TIME_IMMEDIATE == timeout) {
    return MSG_TIMEOUT();
  }

  *trp = tp;
  //tp->u.wttrp = trp;
  thread_set_wttrp(tp, trp);

  auditbox_try_update_buffer(__func__, true);
  return chSchGoSleepTimeoutS(CH_STATE_SUSPENDED(), timeout);
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must not reschedule because it can be called from
 *          ISR context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void chThdResumeI(SecretValue_t kValue, thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {
    thread_t *tp = *trp;

    chDbgAssert(thread_get_state(tp) == CH_STATE_SUSPENDED(), "not CH_STATE_SUSPENDED");

    *trp = NULL;
    //tp->u.rdymsg = msg;
    thread_set_rdymsg(tp, msg);
    (void) chSchReadyI(kValue, tp);
  }

  auditbox_try_update_buffer(__func__, true);
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void chThdResumeS(SecretValue_t kValue, thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {
    thread_t *tp = *trp;

    chDbgAssert(thread_get_state(tp) == CH_STATE_SUSPENDED(), "not CH_STATE_SUSPENDED");

    *trp = NULL;
    chSchWakeupS(kValue, tp, msg);
  }

  auditbox_try_update_buffer(__func__, true); // TODO __func__가 잘 동작하는지 확인
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @api
 */
void chThdResume(SecretValue_t kValue, thread_reference_t *trp, msg_t msg) {

  chSysLock();
  chThdResumeS(kValue, trp, msg);
  chSysUnlock();

  auditbox_try_update_buffer(__func__, true);
}

/**
 * @brief   Enqueues the caller thread on a threads queue object.
 * @details The caller thread is enqueued and put to sleep until it is
 *          dequeued or the specified timeouts expires.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] timeout   the timeout in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE the thread is not enqueued and
 *                        the function returns @p MSG_TIMEOUT as if a timeout
 *                        occurred.
 *                      .
 * @return              The message from @p osalQueueWakeupOneI() or
 *                      @p osalQueueWakeupAllI() functions.
 * @retval MSG_TIMEOUT  if the thread has not been dequeued within the
 *                      specified timeout or if the function has been
 *                      invoked with @p TIME_IMMEDIATE as timeout
 *                      specification.
 *
 * @sclass
 */
msg_t chThdEnqueueTimeoutS(threads_queue_t *tqp, sysinterval_t timeout) {

  if (TIME_IMMEDIATE == timeout) {
    return MSG_TIMEOUT();
  }

  queue_insert(getCurrp(), tqp);

  auditbox_try_update_buffer(__func__, true);
  return chSchGoSleepTimeoutS(CH_STATE_QUEUED(), timeout);
}

/**
 * @brief   Dequeues and wakes up one thread from the threads queue object,
 *          if any.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void chThdDequeueNextI(SecretValue_t kValue, threads_queue_t *tqp, msg_t msg) {

  if (queue_notempty(tqp)) {
    chThdDoDequeueNextI(kValue, tqp, msg);
  }

  auditbox_try_update_buffer(__func__, true);
}

/**
 * @brief   Dequeues and wakes up all threads from the threads queue object.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void chThdDequeueAllI(SecretValue_t kValue, threads_queue_t *tqp, msg_t msg) {

  while (queue_notempty(tqp)) {
    chThdDoDequeueNextI(kValue, tqp, msg);
  }

  auditbox_try_update_buffer(__func__, true);
}


/*===========================================================================*/
/* Module inline functions. (previously)                                     */
/*===========================================================================*/

 /**
  * @brief   Returns a pointer to the current @p thread_t.
  *
  * @return             A pointer to the current thread.
  *
  * @xclass
  */
thread_t *chThdGetSelfX(void) {
  auditbox_try_update_buffer(__func__, true); // CHAOS audit
  return ch.rlist.current; // isn't it same to getCurrp()?
}

/**
 * @brief   Returns the current thread priority.
 * @note    Can be invoked in any context.
 *
 * @return              The current thread priority.
 *
 * @xclass
 */
tprio_t chThdGetPriorityX(void) {
  auditbox_try_update_buffer(__func__, true); // CHAOS audit
  return thread_get_prio(chThdGetSelfX());
}

/** @} */
