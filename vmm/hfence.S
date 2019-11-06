.text

.globl hfence_gvma

// funct7(hfence.gvma), rs2(vmid), rs1(gaddr), funct3(priv), rd(0), opcode(system)
// - opecode[7] : system (1110011)
// - rd[5]      : 0 (00000)
// - funct3[3]  : PRIV(000)
// - rs1[5]     : 5 bit operand
// - rs2[5]     : 5 bit operand
// - funct7[7]  : hfence_gvma (0110001)

// from QEMU/target/riscv/insn32.decode
//hfence_gvma 0110001    ..... ..... 000 00000 1110011 @hfence_gvma
//hfence_bvma 0010001    ..... ..... 000 00000 1110011 @hfence_bvma

hfence_gvma:
    .word 0x62000073 // 0110001,00000,00000,000,00000,1110011
    ret
