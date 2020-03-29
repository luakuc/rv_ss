.section .text.user_sample
.globl _user_sample

_user_sample:
    addi a0, a0, 1
    ecall
    j _user_sample
