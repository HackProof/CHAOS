/*

*/

#include "hal.h"
#include "rt_test_root.h"

/**
 * @file    rt_test_sequence_011.c
 * @brief   Test Sequence 011 code.
 *
 * @page rt_test_sequence_011 [11] Audit
 *
 * File: @ref rt_test_sequence_011.c
 *
 * <h2>Description</h2>
 * audit 기능 테스트
 *
 * <h2>Test Cases</h2>
 * - @subpage rt_test_011_001
 * .
 */

extern auditbox_t ch_auditbox; // ch_audit.c 전역 변수 참조
extern auditspace_t ch_auditspace; // ch_audit.c 전역 변수 참조

extern ch_FIA_t ch_fia; // ch_fia.c 전역 변수 참조


/****************************************************************************
 * Shared code.
 ****************************************************************************/



/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page rt_test_011_001 [11.1] FAU (Audit)
 *
 * <h2>Description</h2>
 * FAU - ch_auditbox
 *
 * <h2>Test Steps</h2>
 * - [11.1.1] ch_auditbox
 * .
 */

static void rt_test_011_001_execute(void) {
  // uint32_t i;
  

  // thread_t *tp;
  // msg_t msg;
  // const char * last_auditbox_buffer_event;
  // const char * last_auditspace_buffer_event;

  /* [11.1.1] ch_auditbox.*/
  test_set_step(1);
  {
    test_assert(auditbox_check_write_authorize(getCurrp()) == true,
                "there is no write authorization for this thread");
    // test_println("1단계 성공!");
  }

  /* [11.1.2] ch_auditbox.*/
  test_set_step(2);
  {
    thread_t * idle_thread = chSysGetIdleThreadX();
    test_assert(strcmp(ch_auditbox.buffer.event, "chSysGetIdleThreadX") == 0,
                ch_auditbox.buffer.event);
    // test_println("2단계 성공!");
  }

    /* [11.1.3] ch_auditbox.*/
  test_set_step(3);
  {
    tprio_t prio = chThdGetPriorityX(); // 내부적으로 chThdGetSelfX() 호출 
    test_assert(strcmp(ch_auditbox.buffer.event, "chThdGetSelfX") == 0,
                ch_auditbox.buffer.event);
    // test_println("3단계 성공!");
  }

    /* [11.1.4] ch_auditbox.*/
  test_set_step(4);
  {
    chThdSleep(100);
    test_assert(strcmp(ch_auditbox.buffer.event, "chThdSleep") == 0,
                ch_auditbox.buffer.event);
    // test_println("4단계 성공!");
  }
  
}

static const testcase_t rt_test_011_001 = {
  "FAU - ch_auditbox",
  NULL,
  NULL,
  rt_test_011_001_execute
};


/**
 * @page rt_test_011_001 [11.2] FAU - ch_auditspace
 *
 * <h2>Description</h2>
 * FAU - ch_auditspace
 *
 * <h2>Test Steps</h2>
 * - [11.2.1] ch_auditbox
 * .
 */

static void rt_test_011_002_execute(void) {

  // // 잔챙이 test
  // test_print("Current thread ID: ");
  // test_printn(thread_get_id_thd(getCurrp()));
  // test_println("");

  // test_print("ch_auditspace.re_thread: ");
  // test_printn(ch_auditbox.re_thread);
  // test_println("");

  // test_print("AUDIT_ORDER_MAX: ");
  // test_printn(AUDIT_ORDER_MAX);
  // test_println("");

  // test_print("ch_auditspace.storage_start: ");
  // test_printn(ch_auditspace.storage_start);
  // test_println("");

  // test_print("ch_auditspace.storage_end: ");
  // test_printn(ch_auditspace.storage_end);
  // test_println("");

  // test_print("ch_auditspace.order: ");
  // test_printn(ch_auditspace.order);
  // test_println("");

/* [11.2.1] ch_auditspace.*/
  test_set_step(1);
  {
    // The printed values will be different
    test_println(ch_auditbox.buffer.event); // chSysUnlock
    auditspace_read_data_from_auditbox();
    test_println(ch_auditspace.space[ch_auditspace.storage_start].event); // 기록이 안됨
    // test_println("1단계 성공!");
  }

  /* [11.2.2] ch_auditspace.*/
  test_set_step(2);
  {
    // The printed values will be the same
    test_println(ch_auditbox.buffer.event); // chSysUnlock
    auditspace_copy_buffer_from_auditbox();
    test_println(ch_auditspace.space[ch_auditspace.storage_start].event);
    // test_println("2단계 성공!");
  }

  /* [11.2.3] ch_auditspace.*/
  test_set_step(3);
  {
    test_assert(ch_auditspace.storage_start \
      == (ch_auditspace.order % ch_auditspace.storage_end),
      "unexpected storage_start at ch_auditspace");
    // test_println("3단계 성공!");
  }

  // // 잔챙이 test
  // test_print("ch_auditspace.storage_start: ");
  // test_printn(ch_auditspace.storage_start);
  // test_println("");

  // test_print("ch_auditspace.storage_end: ");
  // test_printn(ch_auditspace.storage_end);
  // test_println("");

  // test_print("ch_auditspace.order: ");
  // test_printn(ch_auditspace.order);
  // test_println("");

  // for (i = 0; i < ch_auditbox.wr_threads_count; ++i) {
  //   test_printn(ch_auditbox.wr_threads[i]);
  //   test_println("");
  // }

  // for (i = 0; i <= ch_auditspace.storage_start; ++i) {
  //   test_print("ch_auditspace[");
  //   test_printn(i);
  //   test_print("] : {id: ");
  //   test_printn(ch_auditspace.space[i].id);
  //   test_print(", event: ");
  //   test_print(ch_auditspace.space[i].event);
  //   test_println("}");
  //   test_println(ch_auditbox.buffer.event);
  // }
}

static const testcase_t rt_test_011_002 = {
  "FAU - ch_auditspace",
  NULL,
  NULL,
  rt_test_011_002_execute
};

/**
 * @page rt_test_011_002 [11.3] FIA
 *
 * <h2>Description</h2>
 * FIA기능 테스트
 *
 * <h2>Test Steps</h2>
 * - [11.3.1] FIA
 * .
 */

static void rt_test_011_003_execute(void)
{
  uint32_t initial_count = 0;
  // test_print("list_count: ");
  // test_printn(ch_fia.list_count);
  // test_println("");

  /* [11.3.1] */
  test_set_step(1);
  {
    initial_count = ch_fia.list_count;
    fia_register_process_id(0);
    test_assert(ch_fia.list_count == (initial_count + 1),
                "unexpected ch_fia.list_count");
    // test_println("1단계 성공!");
  }

    /* [11.3.2] */
  test_set_step(2);
  {
    fia_register_process_id(0);
    test_assert(ch_fia.list_count == (initial_count + 2),
                "unexpected ch_fia.list_count");
    // test_println("2단계 성공!");
  }

    /* [11.3.3] */
  test_set_step(3);
  {
    fia_register_process_id(0);
    test_assert(ch_fia.list_count == (initial_count + 3),
                "unexpected ch_fia.list_count");
    // test_println("3단계 성공!");
  }

    /* [11.3.4] */
  test_set_step(4);
  {
    fia_register_process_id(0);
    test_assert(ch_fia.list_count == (initial_count + 4),
                "unexpected ch_fia.list_count");
    // test_println("4단계 성공!");
  }
}

static const testcase_t rt_test_011_003 = {
    "FIA test",
    NULL,
    NULL,
    rt_test_011_003_execute
};


/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t *const rt_test_sequence_011_array[] = {
    &rt_test_011_001,
    &rt_test_011_002,
    &rt_test_011_003,
    NULL};

/**
 * @brief   Synchronous Messages.
 */
const testsequence_t rt_test_sequence_011 = {
  "FAU and FIA",
  rt_test_sequence_011_array
};