#ifndef SGXNN_T_H__
#define SGXNN_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "../../App/App/App.h"

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void printf_helloworld(void);
void init_array(void);
int input(char* data);
int learning_process(void);
double square_error(void);
void save_nn(void);
void load_model(void);
int predict(void);
void decryptMessage(char* encMessageIn, size_t len, char* decMessageOut, size_t lenOut);
void encryptMessage(char* decMessageIn, size_t len, char* encMessageOut, size_t lenOut);

sgx_status_t SGX_CDECL print_error_message(sgx_status_t ret);
sgx_status_t SGX_CDECL ocall_print_string(const char* str);
sgx_status_t SGX_CDECL ocall_print_num(uint32_t num);
sgx_status_t SGX_CDECL ocall_read_image(char* number);
sgx_status_t SGX_CDECL ocall_read_label(char* number);
sgx_status_t SGX_CDECL ocall_save_nn(int* retval, const uint8_t* sealed_data, size_t sealed_size);
sgx_status_t SGX_CDECL ocall_load_nn(int* retval, uint8_t* sealed_data, size_t sealed_size);
sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
