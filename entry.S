#include "csr.h"

.section .text.start
.globl _start
_start:
    // disable all interrupts
	csrw CSR_SIE, zero
    csrw CSR_SIP, zero
    li tp, 0

    //TODO setup global pointer

    la sp, kernel_stack + 0x1000
    call start_kernel

    j _shutdown

_shutdown:
    li a7, 8 # SBI_CALL_SHUTDOWN
    li a0, 0
    li a1, 0
    li a2, 0
    ecall
    j _shutdown
