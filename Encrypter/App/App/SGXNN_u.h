#ifndef SGXNN_U_H__
#define SGXNN_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */

#include "../../App/App/App.h"

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PRINT_ERROR_MESSAGE_DEFINED__
#define PRINT_ERROR_MESSAGE_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, print_error_message, (sgx_status_t ret));
#endif
#ifndef OCALL_PRINT_STRING_DEFINED__
#define OCALL_PRINT_STRING_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));
#endif
#ifndef OCALL_PRINT_NUM_DEFINED__
#define OCALL_PRINT_NUM_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_num, (uint32_t num));
#endif
#ifndef OCALL_READ_IMAGE_DEFINED__
#define OCALL_READ_IMAGE_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_read_image, (char* number));
#endif
#ifndef OCALL_READ_LABEL_DEFINED__
#define OCALL_READ_LABEL_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_read_label, (char* number));
#endif
#ifndef OCALL_SAVE_NN_DEFINED__
#define OCALL_SAVE_NN_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_save_nn, (const uint8_t* sealed_data, size_t sealed_size));
#endif
#ifndef OCALL_LOAD_NN_DEFINED__
#define OCALL_LOAD_NN_DEFINED__
int SGX_UBRIDGE(SGX_NOCONVENTION, ocall_load_nn, (uint8_t* sealed_data, size_t sealed_size));
#endif
#ifndef SGX_OC_CPUIDEX_DEFINED__
#define SGX_OC_CPUIDEX_DEFINED__
void SGX_UBRIDGE(SGX_CDECL, sgx_oc_cpuidex, (int cpuinfo[4], int leaf, int subleaf));
#endif
#ifndef SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
#endif
#ifndef SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));
#endif
#ifndef SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_setwait_untrusted_events_ocall, (const void* waiter, const void* self));
#endif
#ifndef SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_multiple_untrusted_events_ocall, (const void** waiters, size_t total));
#endif

sgx_status_t printf_helloworld(sgx_enclave_id_t eid);
sgx_status_t init_array(sgx_enclave_id_t eid);
sgx_status_t input(sgx_enclave_id_t eid, int* retval, char* data);
sgx_status_t learning_process(sgx_enclave_id_t eid, int* retval);
sgx_status_t square_error(sgx_enclave_id_t eid, double* retval);
sgx_status_t save_nn(sgx_enclave_id_t eid);
sgx_status_t load_model(sgx_enclave_id_t eid);
sgx_status_t predict(sgx_enclave_id_t eid, int* retval);
sgx_status_t decryptMessage(sgx_enclave_id_t eid, char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut);
sgx_status_t encryptMessage(sgx_enclave_id_t eid, char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
