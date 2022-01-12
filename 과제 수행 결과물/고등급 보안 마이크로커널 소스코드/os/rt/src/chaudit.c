/**
 * CHAOS
 */

/**
 * @file    chaudit.c
 * @brief   Audit box
 *
 * @addtogroup audit
 * @details This module provides the default portable audit box code.
 * @{
 */

#include "ch.h"
auditbox_t ch_auditbox;
auditspace_t ch_auditspace;


/*===========================================================================*/
/* Initializers                                                              */
/*===========================================================================*/

void _audit_init() {
  auditbox_init();
  auditspace_init();
}

auditdata_t auditdata_init() { // TODO: 필요한가?
  auditdata_t newdata = {
    0,
    "",
    false
  };
  return newdata;
}

/**
 * text\<open>initialize mailbox\<close>
 * definition init_mailbox::"new_mailbox" where
 * "init_mailbox = \<lparr>buffer = init_share_space, boundary = (0,128),
 *  wr_thread = [], re_thread = 0, flag = False\<rparr>"
 */ 
void auditbox_init() { // 전역변수화함 // TODO: 어디서 호출? 
  auditdata_t newdata;
  newdata = auditdata_init();

  ch_auditbox.buffer = newdata;
  ch_auditbox.memory_boundary_start = 0;
  ch_auditbox.memory_boundary_end = 256;
  // ch_auditbox.wr_threads = {0, }; // TODO: 배열 초기화
  ch_auditbox.re_thread = 0;
  ch_auditbox.wr_threads_count = 0;
  ch_auditbox.flag = false;
}

void auditspace_init() { // storage_init은 이 안에 포함됨
  ch_auditspace.storage_start = 0;
  ch_auditspace.storage_end = AUDIT_ORDER_MAX; // TODO: 원래는 memory size이지만 ...
  ch_auditspace.order = 0;
  // ch_auditspace.space[AUDIT_ORDER_MAX] = {0, };
}
/*===========================================================================*/
/* Auditbox 관련 (일반 스레드 -> buffer 쓰기)                                */
/*===========================================================================*/
void auditbox_try_update_buffer(const char * function_name, bool is_success) { // 명세에 없는 함수
  // CHAOS audit start
  auditdata_t audit_data; // TODO: Q. 항상 new audit을 만들어야 하는가?
  audit_data.id = thread_get_id_thd(getCurrp());
  audit_data.event = function_name;
  audit_data.status = is_success;

  if(auditbox_check_write_authorize(getCurrp())) auditbox_update_buffer(audit_data);
}

// 명세: check_write_authorize + check_validity
// 입력: (1) ch_audit -> 전역변수이므로 생략, (2) 확인할 thread id
// 출력: Success 여부 (true or false)z`
// 내용: 현재 thread가 write 권한이 있는지 확인
// audit_write_data에서 호출 (?)
// auditbox_try_update_buffer에서도 호출
bool auditbox_check_write_authorize(thread_t * tp) {
  for (uint32_t i = 0; i < ch_auditbox.wr_threads_count; ++i) {
    if (ch_auditbox.wr_threads[i] == tp->id_thd) return true;
  }
  return false;

  // return auditbox_check_validity(thread_get_id_thd(thread)); // TODO: 일단은 wr_threads가 update되는 시점이 확정되지 않아 true로 동작 (TODO)
}

// 입력: (1) 비교할 wr_threads, (2) 확인할 thread id
// 출력: Success 여부 (true or false)
// 내용: 현재 thread_id가 target ID_t list에 있는지 없는지 검사
// auditbox_check_write_authorize 에서 호출
// NOTE: 일단 명세랑 다르게 구현함 (굳이 copy할 필요가 없음)
// bool auditbox_check_validity(ID_t thread_id) {
//   return false;
//   // 아래는 명세상 구현 ***************************
//   // bool is_success = false;
//   // for (int i = 0; i < 100; ++i) { // TODO 100은 magic number
//   //   if (wr_threads[i] == thread_id) {
//   //     is_success = true;
//   //     break;
//   //   }
//   // }
//   // return is_success;
// }

// 입력: (1) auditbox (전역변수), (2) new audit data
// 출력: auditbox -> 전역변수 ..?
// auditbox_try_update_buffer에서 호출
void auditbox_update_buffer(auditdata_t audit_data) {
  // 일단 copy 안하고 얕은복사 처리함 (TODO: 문자열은 strcpy 필요함??)
  ch_auditbox.buffer = audit_data;
  ch_auditbox.flag = true;
}

// 입력: ch_auditbox -> 현재 전역변수이므로 생략
// 출력: 스레드 번호 배열
// 내용: auditbox의 wr_thread 배열 복사 (deep copy)
// auditbox_check_write_authorize에서 호출
// ID_t * auditbox_copy_wr_threads() {
//   // TODO 배열이 0일때 [] 반환 -> 알아서?
//   ID_t * copied_wr_threads = auditbox_scan_permit_thread(ch_auditbox.wr_threads);
//   return copied_wr_threads;
// }

// 입력: 스레드 번호 배열
// 출력: 스레드 번호 배열
// Q. 이것을 문맥 교환 전에 왜 주기적으로 수행 ?? (TODO)
// auditbox_copy_wr_threads에서 호출 // TODO 이 함수의 구현 확인
// (1) ready list를 scan하는 것인지
// (2) copy_wr_thread 안에서 현재 mailbox의 wr_thread를 가져오는 것인지
// ID_t * auditbox_scan_permit_thread(ID_t * wr_threads) { // auditbox_copy_wr_threads와 합치기 가능?
//   ID_t copied_wr_threads[100];

//   for (int i = 0; i < 100; ++i) {
//     copied_wr_threads[i] = wr_threads[i];
//   }
//   return copied_wr_threads;
// };

// 명세에는 없는 함수, 스레드 생성시 실행
void auditbox_register_id(ID_t _id) {
  uint32_t i;

  // 중복 방지
  for (i = 0; i < ch_auditbox.wr_threads_count; ++i) {
    if (ch_auditbox.wr_threads[i] == _id) return;
  }

  ch_auditbox.wr_threads[ch_auditbox.wr_threads_count] = _id;
  ch_auditbox.wr_threads_count++;
}

// 명세에는 없는 함수, 스레드 종료시 실행
void auditbox_remove_id(ID_t _id) {
  uint32_t i;
  uint32_t target_index = 0;
  bool is_id_in = false;

  for (i = 0; i < ch_auditbox.wr_threads_count; ++i) {
    if (ch_auditbox.wr_threads[i] == _id) {
      target_index = i;
      is_id_in = true;
      break;
    }
  }

  if (!is_id_in) return; // ID가 배열에 없었음

  for (i = target_index; i + 1 < ch_auditbox.wr_threads_count; ++i) {
    ch_auditbox.wr_threads[i] = ch_auditbox.wr_threads[i + 1];
  }
  ch_auditbox.wr_threads_count--;
}

/*===========================================================================*/
/* Audit thread - Auditbox 관련  (Auditspace)                                */
/*===========================================================================*/

// 명세상 입력: 메일박스 (전역변수), audit_storage (전역변수), audit (전역변수)
// 명세상 출력: audit (전역변수)
void auditspace_process() { // audit_process
  // TODO: debug를 위해 잠시 주석처리함
  if (ch_auditbox.flag) {
    auditspace_increment();
    
    // TODO: 명세에는 없지만 여기서 read data 함
    auditspace_read_data_from_auditbox();
  }
}

// 명세상 입력: audit_storage (전역변수), audit_space (전역변수)
// 명세상 출력: audit_space (전역변수)
// audit_increment, memory_increment, storage_overwrite, storage_append 여기서 모두 처리
void auditspace_increment() { 
  // TODO: Re_thread의 의미 ..?
  if (auditspace_is_memory_full()) { // TODO 구현해야 할 함수
    ch_auditspace.storage_start = ch_auditspace.order % AUDIT_ORDER_MAX;
  } else {
    ch_auditspace.storage_start = ch_auditspace.order;
  }
}

bool auditspace_is_memory_full() { // 명세: memory_status_check
  // 원래 3가지 status -> is_full, is_remained_enough, is_85_percent_full
  // 구현상 true/false를 위해 is_full일때만 true 반환하도록
  if (ch_auditspace.order >= AUDIT_ORDER_MAX) {
    return true;
  }
  return false;
}

void auditspace_read_data_from_auditbox() { // audit_data_read
  if (ch_auditbox.re_thread == thread_get_id_thd(getCurrp())) { // TODO: 맞게 동작하는지 (2021-11-28 주석)
    auditspace_copy_buffer_from_auditbox();
  }
}

void auditspace_copy_buffer_from_auditbox() { // copy_buffer
  ch_auditspace.space[ch_auditspace.storage_start] = ch_auditbox.buffer; // TODO: shallow copy?
  ch_auditspace.order++; // TODO: 명세에 없지만 임의로 추가
  ch_auditbox.flag = false; // TODO: 명세에 없는 것 같지만 일단 추가
}


/*===========================================================================*/
/* TODO: 일단 존재하는 함수들 ........                                       */
/*===========================================================================*/



// 입력: 스레드 리스트
// 출력: 리스트 ...?
// audit_update_accessible_thread에서 호출됨
// void audit_scan_access_thread() {
//   void;
// }

// Input: 스레드 리스트
// Output: void
// 내용: ch_auditbox.wr_thread update
// originally update_mailbox_accessible_thread
// Q. 언제 호출? 의도를 알 수 없는 함수?
// void audit_update_accessible_thread() {
//   // TODO
//   // ch_auditbox.wr_threads = 
//   void;
// } 




// 입력: (1) auditbox (전역변수), (2) 확인할 thread_id
// 출력: (1) auditbox의 buffer, (2) Success 여부 (true or false) // TODO 이를 어떻게 핸들링??
// TODO: 누가 호출? -> 감사 스레드가 호출! (감사 스레드가 실행해야 할 함수 ??)
// bool audit_read_data() {
  
// }



// 입력: (1) auditbox (전역변수), (2) 현재 thread ??, (3) auditdata
// 출력: (1) auditbox (전역변수 ???), (2) Success 여부 (true or false)
// TODO: 누가 호출? -> 감사 스레드가 호출! (감사 스레드가 실행해야 할 함수 ??
// void audit_write_data(thread_t * thread, auditdata_t audit_data) {
//   if (audit_write_authorize(thread)) {

//   } else {

//   }
// }

/** @} */