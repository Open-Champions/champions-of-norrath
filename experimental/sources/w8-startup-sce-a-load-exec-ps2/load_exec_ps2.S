/* Observed _LoadExecPS2 at 0x002d4d20, 16 bytes. Kernel service 6. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl _LoadExecPS2
    .type _LoadExecPS2,@function
    .ent _LoadExecPS2
_LoadExecPS2:
    li $v1,6
    syscall
    jr $ra
    nop
    .end _LoadExecPS2
    .size _LoadExecPS2,.-_LoadExecPS2
