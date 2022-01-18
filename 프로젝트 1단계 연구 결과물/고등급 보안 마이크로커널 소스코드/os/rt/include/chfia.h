/**
 * CHAOS
 */

/**
 * @file    chaudit.h
 * @brief   FIA
 *
 * @addtogroup FIA
 * @{
 */

#ifndef CHFIA_H
#define CHFIA_H

//--------------------------------------------------------
#define MAX_THREAD_NUMBER 2048 // TODO - 배열 넘어가는 경우 (test 10.5에서 10000번 이상 생성함 -> 65536)
// #define SECRET_LEN 2 // TODO: is it really necessary?

//--------------------------------------------------------
typedef uint32_t PID_t; // TODO: is it really necessary?
typedef uint32_t ID_t;
typedef uint32_t SecretValue_t; // it was originally uint8_t ... but why?

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

// typedef struct {
//   ID_t id;
//   SecretValue_t sv; // it was sv[SECRET_LEN]... but why?
// } IDSV_t; // TODO: for what?

// typedef struct {
//   ID_t id_list[MAX_THREAD_NUMBER]; // IDSV_list
//   SecretValue_t sv_list[MAX_THREAD_NUMBER]; // IDSV_list
//   uint32_t idq_cnt; // 0으로 초기화? (TODO)
// } IDSV_queue_t; // TODO: for what?

// typedef struct {
//   ID_t id;
//   bool bv;
// } ID_bool_t; // pair (진짜로 필요한가?)

// typedef struct {
//   SecretValue_t sv;
//   bool bv;
// } SV_bool_t; // pair (진짜로 필요한가?)

// typedef struct {
//   IDQueue_t idq;
//   bool bv;
// } IDQueue_bool_t; // pair

// struct ch_secure {
//   IDSV_queue_t kernel_idsvq;
//   // SecretValue_t kernel_sv;
//   bool verification;
// }; // TODO: 아예 FIA쪽으로 통합

typedef struct {
  // IDSV_t idsv;
  ID_t id;
  SecretValue_t sv;
  bool bv;
} IDSV_bool_t; // pair -> RegistProcessID를 위해 필요함

typedef struct {
  ID_t seed_id; // definition seedIDlist :: "ID" where "seedIDlist = 0"
  SecretValue_t seed_sv; // definition seedSVlist :: "SecretValue" where "seedSVlist = 0"

  // 이 아래는 ch_secure에 대한 것들
  bool verification; // ch_secure
  ID_t id_list[MAX_THREAD_NUMBER]; // IDSV_list
  SecretValue_t sv_list[MAX_THREAD_NUMBER]; // IDSV_list
  uint32_t list_count; // 0으로 초기화? (TODO)
} ch_FIA_t;

/*===========================================================================*/
/* functions for data structures                                             */
/*===========================================================================*/
// ID_t idsv_get_id(IDSV_t *idsv);
// SecretValue_t idsv_get_sv(IDSV_t *idsv);
// void idsv_set_id(IDSV_t *idsv, ID_t id);
// void idsv_set_sv(IDSV_t *idsv, SecretValue_t sv);

// IDSV_t *idqueue_get_idsv_list(IDQueue_t *idq);
// void idqueue_set_idsv(IDQueue_t *idq, IDSV_t idsv);
// uint32_t idqueue_get_idsv_cnt(IDQueue_t *idq);
// void idqueue_set_idsv_cnt(IDQueue_t *idq, uint32_t idsv_cnt);

// ID_t idbool_get_id(ID_bool_t *idb);
// bool idbool_get_bool(ID_bool_t *idb);
// void idbool_set_id(ID_bool_t *idb, ID_t id);
// void idbool_set_bool(ID_bool_t *idb, bool bv);

// SecretValue_t svbool_get_sv(SV_bool_t *idb);
// bool svbool_get_bool(SV_bool_t *idb);
// void svbool_set_sv(SV_bool_t *idb, SecretValue_t sv);
// void svbool_set_bool(SV_bool_t *idb, bool bv);

// IDQueue_t idqueuebool_get_idqueue(IDQueue_bool_t *idqb);
// bool idqueuebool_get_bool(IDQueue_bool_t *idqb);
// void idqueuebool_set_idqueue(IDQueue_bool_t *idqb, IDQueue_t *idq);
// void idqueuebool_set_bool(IDQueue_bool_t *idqb, bool bv);


/*===========================================================================*/
/* Module functions (Internal)                                               */
/*===========================================================================*/
void _fia_init(void); // chsys.c의 chSysInit에서 호출

SecretValue_t get_kernel_secret(void);

uint32_t fia_get_pseudo_random_uint32_t(uint32_t * n_in);
// ID_t * fia_get_id_ptr(void);
// SecretValue_t * fia_get_sv_ptr(void);

bool fia_generate_id(SecretValue_t kValue);
bool fia_generate_sv(SecretValue_t kValue);

bool fia_is_id_duplicated(ID_t id_in, SecretValue_t kValue);
void fia_append_idsv(ID_t id, SecretValue_t sv);
bool fia_store_idsv(SecretValue_t kValue, ID_t id, SecretValue_t sv);
bool fia_store_idsv_to_kernel_area(SecretValue_t kValue, ID_t id_in, SecretValue_t sv_in);
// bool fia_is_idsv_in(SecretValue_t kValue, ID_t id_in, SecretValue_t sv_in);


/*===========================================================================*/
/* API functions                                                             */
/*===========================================================================*/
IDSV_bool_t fia_register_process_id(SecretValue_t kValue);
bool fia_check_id_sv(SecretValue_t kValue, ID_t id_in, SecretValue_t sv_in); // CheckIDSecret


#endif /* CHFIA_H */
/** @} */