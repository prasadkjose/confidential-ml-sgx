#include "SGXNN_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


typedef struct ms_input_t {
	int ms_retval;
	char* ms_data;
} ms_input_t;

typedef struct ms_learning_process_t {
	int ms_retval;
} ms_learning_process_t;

typedef struct ms_square_error_t {
	double ms_retval;
} ms_square_error_t;

typedef struct ms_predict_t {
	int ms_retval;
} ms_predict_t;

typedef struct ms_decryptMessage_t {
	char* ms_encMessageIn;
	size_t ms_len;
	char* ms_decMessageOut;
	size_t ms_lenOut;
} ms_decryptMessage_t;

typedef struct ms_encryptMessage_t {
	char* ms_decMessageIn;
	size_t ms_len;
	char* ms_encMessageOut;
	size_t ms_lenOut;
} ms_encryptMessage_t;

typedef struct ms_print_error_message_t {
	sgx_status_t ms_ret;
} ms_print_error_message_t;

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_ocall_print_num_t {
	uint32_t ms_num;
} ms_ocall_print_num_t;

typedef struct ms_ocall_read_image_t {
	char* ms_number;
} ms_ocall_read_image_t;

typedef struct ms_ocall_read_label_t {
	char* ms_number;
} ms_ocall_read_label_t;

typedef struct ms_ocall_save_nn_t {
	int ms_retval;
	const uint8_t* ms_sealed_data;
	size_t ms_sealed_size;
} ms_ocall_save_nn_t;

typedef struct ms_ocall_load_nn_t {
	int ms_retval;
	uint8_t* ms_sealed_data;
	size_t ms_sealed_size;
} ms_ocall_load_nn_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4200)
#endif

static sgx_status_t SGX_CDECL sgx_printf_helloworld(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	printf_helloworld();
	return status;
}

static sgx_status_t SGX_CDECL sgx_init_array(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	init_array();
	return status;
}

static sgx_status_t SGX_CDECL sgx_input(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_input_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_input_t* ms = SGX_CAST(ms_input_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_data = ms->ms_data;
	size_t _len_data = 813;
	char* _in_data = NULL;

	CHECK_UNIQUE_POINTER(_tmp_data, _len_data);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_data != NULL && _len_data != 0) {
		if ( _len_data % sizeof(*_tmp_data) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_data = (char*)malloc(_len_data);
		if (_in_data == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_data, _len_data, _tmp_data, _len_data)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ms->ms_retval = input(_in_data);

err:
	if (_in_data) free(_in_data);
	return status;
}

static sgx_status_t SGX_CDECL sgx_learning_process(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_learning_process_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_learning_process_t* ms = SGX_CAST(ms_learning_process_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = learning_process();


	return status;
}

static sgx_status_t SGX_CDECL sgx_square_error(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_square_error_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_square_error_t* ms = SGX_CAST(ms_square_error_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = square_error();


	return status;
}

static sgx_status_t SGX_CDECL sgx_save_nn(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	save_nn();
	return status;
}

static sgx_status_t SGX_CDECL sgx_load_model(void* pms)
{
	sgx_status_t status = SGX_SUCCESS;
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	load_model();
	return status;
}

static sgx_status_t SGX_CDECL sgx_predict(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_predict_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_predict_t* ms = SGX_CAST(ms_predict_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = predict();


	return status;
}

static sgx_status_t SGX_CDECL sgx_decryptMessage(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_decryptMessage_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_decryptMessage_t* ms = SGX_CAST(ms_decryptMessage_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_encMessageIn = ms->ms_encMessageIn;
	size_t _tmp_len = ms->ms_len;
	size_t _len_encMessageIn = _tmp_len;
	char* _in_encMessageIn = NULL;
	char* _tmp_decMessageOut = ms->ms_decMessageOut;
	size_t _tmp_lenOut = ms->ms_lenOut;
	size_t _len_decMessageOut = _tmp_lenOut;
	char* _in_decMessageOut = NULL;

	CHECK_UNIQUE_POINTER(_tmp_encMessageIn, _len_encMessageIn);
	CHECK_UNIQUE_POINTER(_tmp_decMessageOut, _len_decMessageOut);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_encMessageIn != NULL && _len_encMessageIn != 0) {
		if ( _len_encMessageIn % sizeof(*_tmp_encMessageIn) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_encMessageIn = (char*)malloc(_len_encMessageIn);
		if (_in_encMessageIn == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_encMessageIn, _len_encMessageIn, _tmp_encMessageIn, _len_encMessageIn)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_decMessageOut != NULL && _len_decMessageOut != 0) {
		if ( _len_decMessageOut % sizeof(*_tmp_decMessageOut) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_decMessageOut = (char*)malloc(_len_decMessageOut)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_decMessageOut, 0, _len_decMessageOut);
	}

	decryptMessage(_in_encMessageIn, _tmp_len, _in_decMessageOut, _tmp_lenOut);
	if (_in_decMessageOut) {
		if (memcpy_s(_tmp_decMessageOut, _len_decMessageOut, _in_decMessageOut, _len_decMessageOut)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_encMessageIn) free(_in_encMessageIn);
	if (_in_decMessageOut) free(_in_decMessageOut);
	return status;
}

static sgx_status_t SGX_CDECL sgx_encryptMessage(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_encryptMessage_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_encryptMessage_t* ms = SGX_CAST(ms_encryptMessage_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_decMessageIn = ms->ms_decMessageIn;
	size_t _tmp_len = ms->ms_len;
	size_t _len_decMessageIn = _tmp_len;
	char* _in_decMessageIn = NULL;
	char* _tmp_encMessageOut = ms->ms_encMessageOut;
	size_t _tmp_lenOut = ms->ms_lenOut;
	size_t _len_encMessageOut = _tmp_lenOut;
	char* _in_encMessageOut = NULL;

	CHECK_UNIQUE_POINTER(_tmp_decMessageIn, _len_decMessageIn);
	CHECK_UNIQUE_POINTER(_tmp_encMessageOut, _len_encMessageOut);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_decMessageIn != NULL && _len_decMessageIn != 0) {
		if ( _len_decMessageIn % sizeof(*_tmp_decMessageIn) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_decMessageIn = (char*)malloc(_len_decMessageIn);
		if (_in_decMessageIn == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_decMessageIn, _len_decMessageIn, _tmp_decMessageIn, _len_decMessageIn)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_encMessageOut != NULL && _len_encMessageOut != 0) {
		if ( _len_encMessageOut % sizeof(*_tmp_encMessageOut) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_encMessageOut = (char*)malloc(_len_encMessageOut)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_encMessageOut, 0, _len_encMessageOut);
	}

	encryptMessage(_in_decMessageIn, _tmp_len, _in_encMessageOut, _tmp_lenOut);
	if (_in_encMessageOut) {
		if (memcpy_s(_tmp_encMessageOut, _len_encMessageOut, _in_encMessageOut, _len_encMessageOut)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_decMessageIn) free(_in_decMessageIn);
	if (_in_encMessageOut) free(_in_encMessageOut);
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[10];
} g_ecall_table = {
	10,
	{
		{(void*)(uintptr_t)sgx_printf_helloworld, 0, 0},
		{(void*)(uintptr_t)sgx_init_array, 0, 0},
		{(void*)(uintptr_t)sgx_input, 0, 0},
		{(void*)(uintptr_t)sgx_learning_process, 0, 0},
		{(void*)(uintptr_t)sgx_square_error, 0, 0},
		{(void*)(uintptr_t)sgx_save_nn, 0, 0},
		{(void*)(uintptr_t)sgx_load_model, 0, 0},
		{(void*)(uintptr_t)sgx_predict, 0, 0},
		{(void*)(uintptr_t)sgx_decryptMessage, 0, 0},
		{(void*)(uintptr_t)sgx_encryptMessage, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[12][10];
} g_dyn_entry_table = {
	12,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL print_error_message(sgx_status_t ret)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_print_error_message_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_print_error_message_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_print_error_message_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_print_error_message_t));
	ocalloc_size -= sizeof(ms_print_error_message_t);

	ms->ms_ret = ret;
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_num(uint32_t num)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_print_num_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_num_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_num_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_num_t));
	ocalloc_size -= sizeof(ms_ocall_print_num_t);

	ms->ms_num = num;
	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_read_image(char* number)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_number = sizeof(char);

	ms_ocall_read_image_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_read_image_t);
	void *__tmp = NULL;

	void *__tmp_number = NULL;

	CHECK_ENCLAVE_POINTER(number, _len_number);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (number != NULL) ? _len_number : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_read_image_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_read_image_t));
	ocalloc_size -= sizeof(ms_ocall_read_image_t);

	if (number != NULL) {
		ms->ms_number = (char*)__tmp;
		__tmp_number = __tmp;
		if (_len_number % sizeof(*number) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_number, 0, _len_number);
		__tmp = (void *)((size_t)__tmp + _len_number);
		ocalloc_size -= _len_number;
	} else {
		ms->ms_number = NULL;
	}
	
	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
		if (number) {
			if (memcpy_s((void*)number, _len_number, __tmp_number, _len_number)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_read_label(char* number)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_number = sizeof(char);

	ms_ocall_read_label_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_read_label_t);
	void *__tmp = NULL;

	void *__tmp_number = NULL;

	CHECK_ENCLAVE_POINTER(number, _len_number);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (number != NULL) ? _len_number : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_read_label_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_read_label_t));
	ocalloc_size -= sizeof(ms_ocall_read_label_t);

	if (number != NULL) {
		ms->ms_number = (char*)__tmp;
		__tmp_number = __tmp;
		if (_len_number % sizeof(*number) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_number, 0, _len_number);
		__tmp = (void *)((size_t)__tmp + _len_number);
		ocalloc_size -= _len_number;
	} else {
		ms->ms_number = NULL;
	}
	
	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
		if (number) {
			if (memcpy_s((void*)number, _len_number, __tmp_number, _len_number)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_save_nn(int* retval, const uint8_t* sealed_data, size_t sealed_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_sealed_data = sealed_size;

	ms_ocall_save_nn_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_save_nn_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(sealed_data, _len_sealed_data);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (sealed_data != NULL) ? _len_sealed_data : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_save_nn_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_save_nn_t));
	ocalloc_size -= sizeof(ms_ocall_save_nn_t);

	if (sealed_data != NULL) {
		ms->ms_sealed_data = (const uint8_t*)__tmp;
		if (_len_sealed_data % sizeof(*sealed_data) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, sealed_data, _len_sealed_data)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_sealed_data);
		ocalloc_size -= _len_sealed_data;
	} else {
		ms->ms_sealed_data = NULL;
	}
	
	ms->ms_sealed_size = sealed_size;
	status = sgx_ocall(5, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_load_nn(int* retval, uint8_t* sealed_data, size_t sealed_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_sealed_data = sealed_size;

	ms_ocall_load_nn_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_load_nn_t);
	void *__tmp = NULL;

	void *__tmp_sealed_data = NULL;

	CHECK_ENCLAVE_POINTER(sealed_data, _len_sealed_data);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (sealed_data != NULL) ? _len_sealed_data : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_load_nn_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_load_nn_t));
	ocalloc_size -= sizeof(ms_ocall_load_nn_t);

	if (sealed_data != NULL) {
		ms->ms_sealed_data = (uint8_t*)__tmp;
		__tmp_sealed_data = __tmp;
		if (_len_sealed_data % sizeof(*sealed_data) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_sealed_data, 0, _len_sealed_data);
		__tmp = (void *)((size_t)__tmp + _len_sealed_data);
		ocalloc_size -= _len_sealed_data;
	} else {
		ms->ms_sealed_data = NULL;
	}
	
	ms->ms_sealed_size = sealed_size;
	status = sgx_ocall(6, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
		if (sealed_data) {
			if (memcpy_s((void*)sealed_data, _len_sealed_data, __tmp_sealed_data, _len_sealed_data)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(int);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	void *__tmp_cpuinfo = NULL;

	CHECK_ENCLAVE_POINTER(cpuinfo, _len_cpuinfo);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (cpuinfo != NULL) ? _len_cpuinfo : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));
	ocalloc_size -= sizeof(ms_sgx_oc_cpuidex_t);

	if (cpuinfo != NULL) {
		ms->ms_cpuinfo = (int*)__tmp;
		__tmp_cpuinfo = __tmp;
		if (_len_cpuinfo % sizeof(*cpuinfo) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset(__tmp_cpuinfo, 0, _len_cpuinfo);
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		ocalloc_size -= _len_cpuinfo;
	} else {
		ms->ms_cpuinfo = NULL;
	}
	
	ms->ms_leaf = leaf;
	ms->ms_subleaf = subleaf;
	status = sgx_ocall(7, ms);

	if (status == SGX_SUCCESS) {
		if (cpuinfo) {
			if (memcpy_s((void*)cpuinfo, _len_cpuinfo, __tmp_cpuinfo, _len_cpuinfo)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);

	ms->ms_self = self;
	status = sgx_ocall(8, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);

	ms->ms_waiter = waiter;
	status = sgx_ocall(9, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);

	ms->ms_waiter = waiter;
	ms->ms_self = self;
	status = sgx_ocall(10, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(void*);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(waiters, _len_waiters);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (waiters != NULL) ? _len_waiters : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);

	if (waiters != NULL) {
		ms->ms_waiters = (const void**)__tmp;
		if (_len_waiters % sizeof(*waiters) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, waiters, _len_waiters)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		ocalloc_size -= _len_waiters;
	} else {
		ms->ms_waiters = NULL;
	}
	
	ms->ms_total = total;
	status = sgx_ocall(11, ms);

	if (status == SGX_SUCCESS) {
		if (retval) *retval = ms->ms_retval;
	}
	sgx_ocfree();
	return status;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
