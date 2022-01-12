/**
 * CHAOS
 */

/**
 * @file    chaudit.h
 * @brief   Audit system
 *
 * @addtogroup audit
 * @{
 */

#ifndef CHAUDIT_H
#define CHAUDIT_H

// #define EVENT_NAME_MAX 50   // TODO: 필요?
// #define BUFFER_LIST_MAX 16 // TODO: 필요?
#define WR_THREADS_MAX 2048 // TODO: MAX_THREAD_NUMBER (2048) 과의 동기화
#define AUDIT_ORDER_MAX 256

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
typedef struct  {
  ID_t id;
  const char * event; // function name (TODO: 최대 길이 제한 필요?)
  bool status; // true for success, false for fail
} auditdata_t;

typedef struct  {
  auditdata_t buffer; // TODO: 16개 있다고 들었는데... 일단 하나만 저장?
  uint32_t memory_boundary_start;
  uint32_t memory_boundary_end;
  ID_t wr_threads[WR_THREADS_MAX]; // dynamic하게 처리?? 리스트로 처리?
  ID_t re_thread;
  bool flag;

  uint32_t wr_threads_count; // it must be smllaer than WR_THREADS_MAX
} auditbox_t;

typedef struct {
  uint32_t storage_start;
  uint32_t storage_end;   // 나중에 값이 바뀌는 issue 있음 (TODO)
  uint32_t order;
  auditdata_t space[AUDIT_ORDER_MAX];
} auditspace_t;

/*===========================================================================*/
/* Initializers -> chsys.c의 chSysInit에서 호출                              */
/*===========================================================================*/
void _audit_init(void);
auditdata_t auditdata_init(void); // originally init_share_space;
void auditbox_init(void); // originally init_mailbox
void auditspace_init(void);


/*===========================================================================*/
/* Auditbox 관련 (일반 스레드 -> buffer 쓰기): API 함수에서 호출             */
/*===========================================================================*/
void auditbox_try_update_buffer(const char * function_name, bool is_success); // 명세에 없는 함수
bool auditbox_check_write_authorize(thread_t * thread); // mailbox_write_authorize
// bool auditbox_check_validity(ID_t thread_id);
void auditbox_update_buffer(auditdata_t audit_data);

// ID_t * auditbox_copy_wr_threads(); // originally copy_wr_thread
// ID_t * auditbox_scan_permit_thread(ID_t * wr_threads);
// void audit_update_accessible_thread(); // originally update_mailbox_accessible_thread


/*===========================================================================*/
/* Audit thread - Auditbox 관련 (Auditspace) -> chsys.h의 chSysSwitch        */
/*===========================================================================*/
void auditspace_process(void);
void auditspace_increment(void);
bool auditspace_is_memory_full(void);
void auditspace_read_data_from_auditbox(void);
void auditspace_copy_buffer_from_auditbox(void);


/*===========================================================================*/
/* Other functions                                                           */
/*===========================================================================*/

// auditdata_t * audit_copy_buffer();
// bool audit_read_data();

// auditdata_t audit_update_auditdata(); // TODO: 어디서 호출하는지 모름
// void audit_update_buffer();

// void audit_write_data();


#endif /* CHAUDIT_H */
/** @} */