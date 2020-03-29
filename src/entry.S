.section .text.start
.globl _start
_start:
    // disable all interrupts
	csrw sie, zero
    csrw sip, zero
    li tp, 0
    li gp, 0

    la sp, _kernel_stack;
    call start_kernel

    j _shutdown

_shutdown:
    li a7, 8 # SBI_CALL_SHUTDOWN
    li a0, 0
    li a1, 0
    li a2, 0
    ecall
    j _shutdown
