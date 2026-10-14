/* Observed GetOsdConfigParam at 0x002d5190, 16 bytes. Kernel service 75. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl GetOsdConfigParam
    .type GetOsdConfigParam,@function
    .ent GetOsdConfigParam
GetOsdConfigParam:
    li $v1,75
    syscall
    jr $ra
    nop
    .end GetOsdConfigParam
    .size GetOsdConfigParam,.-GetOsdConfigParam
