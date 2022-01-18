/**
 * CHAOS
 */

/**
 * @file    chfia.c
 * @brief   FIA
 *
 * @addtogroup FIA
 * @details FIA
 * @{
 */

#include "ch.h"

// TODO: 구현상 편의를 위한 임시 전역 변수
ch_FIA_t ch_fia;

/*===========================================================================*/
/* functions for data structures                                             */
/*===========================================================================*/

void _fia_init(void) {
  ch_fia.seed_id = (ID_t) 0;
  ch_fia.seed_sv = (SecretValue_t) 0;

  ch_fia.verification = true; // ch_secure
  // ID_t id_list[MAX_THREAD_NUMBER]; // IDSV_list
  // SecretValue_t sv_list[MAX_THREAD_NUMBER]; // IDSV_list
  ch_fia.list_count = 0;
}

/**
 * @{
 */ 
// struct IDSV
// ID_t idsv_get_id(IDSV_t *idsv) {
//   return idsv->id; // list?
// }

// SecretValue_t idsv_get_sv(IDSV_t *idsv) {
//   return idsv->sv; // list?
// }

// void idsv_set_id(IDSV_t *idsv, ID_t id) {
//   idsv->id = id;
// }

// void idsv_set_sv(IDSV_t *idsv, SecretValue_t sv) {
//   idsv->sv = sv;
// }



// struct IDSV_queue

// IDSV_t *idqueue_get_idsv_list(IDSV_queue_t *idq) {
//   return idq->idsv_list;
// }

// // // TODO 맞는 함수인가?
// // void idqueue_set_idsv(IDSV_queue_t *idq, IDSV_t idsv) {
// //   idsv_set_id(&idq->idsv_list[idq->count], idsv.id);
// //   idsv_set_sv(&idq->idsv_list[idq->count], idsv.sv);
// //   idqueue_set_count(idq, idqueue_get_count(idq)+1);
// // }

// uint32_t idqueue_get_count(IDSV_queue_t *idq) {
//   return idq->count;
// }

// void idqueue_set_count(IDSV_queue_t *idq, uint32_t count) {
//   idq->count = count;
// }


// struct IDSV_queue_bool_t
// IDSV_queue_t idqueuebool_get_idqueue(IDSV_queue_bool_t *idqb) {
//   return idqb->idq;
// }

// bool idqueuebool_get_bool(IDSV_queue_bool_t *idqb) {
//   return idqb->bv;
// }

// void idqueuebool_set_idqueue(IDSV_queue_bool_t *idqb, IDSV_queue_t *idq) {
//   idqb->idq = *idq;
// }

// void idqueuebool_set_bool(IDSV_queue_bool_t *idqb, bool bv) {  
//   idqb->bv = bv;
// }
/** @} */

/*===========================================================================*/
/* Module functions (Internal)                                               */
/*===========================================================================*/
/**
 * @{
 */

SecretValue_t get_kernel_secret(void) { //  이 부분만 fia_ 안붙임
  return (SecretValue_t)0;
}

uint32_t fia_get_pseudo_random_uint32_t(uint32_t * n_in) {
    // TODO: pseudo-random 구현 (temporary)
    // 현재는 call by reference로 원래 변수를 1 증가한다.
    *n_in = (*n_in) + 1;
    return (*n_in);
}

// ID_t * ch_fia_get_id_ptr(void) { // seedIDlist
//   return &(ch_fia.seed_id);
// }

// SecretValue_t * ch_fia_get_sv_ptr(void) { // seedSVlist
//   return &(ch_fia.seed_sv);
// }

bool fia_generate_id(SecretValue_t kValue) { // GenerateID
  if (kValue == get_kernel_secret()) {
    fia_get_pseudo_random_uint32_t(&(ch_fia.seed_id));
    return true; // 원래는 (id, bool)이지만 id의 전역 변수화로 bool만 반환
  }
  return false;
}

bool fia_generate_sv(SecretValue_t kValue) { // GenerateSV
  if (kValue == get_kernel_secret()) {
    fia_get_pseudo_random_uint32_t(&(ch_fia.seed_sv));
    return true; // 원래는 (sv, bool)이지만 sv의 전역 변수화로 bool만 반환
  }
  return false;
}

// 명세: is_id_duplicated + get_ID_list_from_IDSV_list
bool fia_is_id_duplicated(ID_t id_in, SecretValue_t kValue) { // CheckDupId
  uint32_t i = 0;
  // IDSV_queue_t idq = ch.secure.kernel_idsvq;

  if (kValue == get_kernel_secret()) { // get_ID_list_from_IDSV_list 부분이 합쳐짐
    for (i = 0; i < ch_fia.list_count; ++i) {
      if (ch_fia.id_list[i] == id_in) return true;
    }

    // ID_t id = idsv_get_id((idqueue_get_idsv(&ch.secure.kernel_idsvq))); 
    // if (id_in == id) {
    //   return TRUE;
    // }
  }
  return false;
}

// Note: 명세의 IDSV_list는 global 변수처럼 취급 (ch_secure의 멤버 변수; ch.secure.kernel_idsvq)
// void fia_append_idsv(ID_t new_id, SecretValue_t new_sv) {
//   ch_fia.id_list[ch_fia.list_count] = new_id;
//   ch_fia.sv_list[ch_fia.list_count] = new_sv;
//   ch_fia.list_count++;
// }

// // storeToKernelArea_state
// bool fia_store_idsv(SecretValue_t kValue, ID_t id, SecretValue_t sv) {
//   if (kValue == get_kernel_secret()) {
//     ch_fia_append_idsv(id, sv);
//     return true;
//   }
//   return false;
// }

// 명세: storeToKernelArea + storeToKernelArea_state + append_idsv
// 완전 다시 구현함
bool fia_store_idsv_to_kernel_area(SecretValue_t kValue, ID_t id_in, SecretValue_t sv_in) {
  if (kValue == get_kernel_secret()) {
    ch_fia.id_list[ch_fia.list_count] = id_in;
    ch_fia.sv_list[ch_fia.list_count] = sv_in;
    ch_fia.list_count++;
    return true;
  }
  return false;
}

// ReadFromKernelArea_state
// ReadFromKernelArea

/*===========================================================================*/
/* API functions                                                             */
/*===========================================================================*/

// 다시 구현함
// Note: chthreads.c에서 호출되는 함수!
IDSV_bool_t fia_register_process_id(SecretValue_t kValue) { // RegistProcessID
  IDSV_bool_t idsv_bool;

  if (kValue == get_kernel_secret()) {
    if (fia_generate_id(kValue) && fia_generate_sv(kValue) \
    && !fia_is_id_duplicated(ch_fia.seed_id, kValue) \
    && fia_store_idsv_to_kernel_area(kValue, ch_fia.seed_id, ch_fia.seed_sv)) {
      idsv_bool.id = ch_fia.seed_id;
      idsv_bool.sv = ch_fia.seed_sv;
      idsv_bool.bv = true;
      return idsv_bool;
    }
  }

  // TODO: 위 주석은 반드시 해제해야함
  idsv_bool.id = 0;
  idsv_bool.sv = 0;
  idsv_bool.bv = false;
  return idsv_bool;
}

// bool fia_is_idsv_in(SecretValue_t kValue, ID_t id_in, SecretValue_t sv_in) { // TODO ?
//   uint32_t i = 0;
//   // IDSV_queue_t idq = ch.secure.kernel_idsvq;
//   if (kValue == get_kernel_secret()) {
//     for (i = 0; i < ch_fia.list_count; ++i) {
//       if (ch_fia.id_list[i] == id_in && ch_fia.sv_list[i] == sv_in) return true;
//     }
//   }
//   return false;
// }

// 명세: CheckIDSecret + is_idsv_in
// Note: chthreads.c에서 호출되는 함수!
bool fia_check_id_sv(SecretValue_t kValue, ID_t id_in, SecretValue_t sv_in) { 
  if (kValue == get_kernel_secret()) {
      for (uint32_t i = 0; i < ch_fia.list_count; ++i) {
        if (ch_fia.id_list[i] == id_in && ch_fia.sv_list[i] == sv_in) return true;
      }
  }
  return false;
}
/** @} */
/** @} */