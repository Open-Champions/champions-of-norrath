/* Observed _InitTLB at 0x2d5550, 16 bytes. Kernel service 130. The arguments and the kernel
 * result stay in their incoming registers. What the kernel does with them is
 * unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl _InitTLB
    .type _InitTLB,@function
    .ent _InitTLB
_InitTLB:
    li $v1,130
    syscall
    jr $ra
    nop
    .end _InitTLB
    .size _InitTLB,.-_InitTLB
