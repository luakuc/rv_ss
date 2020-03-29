#pragma once

#include <stdbool.h>

#include "vcpu.h"

#define SBI_SUCCESS 0
#define SBI_ERR_FAILURE -1
#define SBI_ERR_NOT_SUPPORTED -2
#define SBI_ERR_INVALID_PARAM -3
#define SBI_ERR_DENIED -4
#define SBI_ERR_INVALID_ADDRESS -5

bool emulate_sbi_call(virtual_cpu_t* vcpu);
