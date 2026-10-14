/* Observed SetOsdConfigParam at 0x002d5180, 16 bytes. Kernel service 74. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetOsdConfigParam
    .type SetOsdConfigParam,@function
    .ent SetOsdConfigParam
SetOsdConfigParam:
    li $v1,74
    syscall
    jr $ra
    nop
    .end SetOsdConfigParam
    .size SetOsdConfigParam,.-SetOsdConfigParam
