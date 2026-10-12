/* Observed DeleteThread at 0x002d4ef0, 16 bytes. Kernel service 33. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl DeleteThread
    .type DeleteThread,@function
    .ent DeleteThread
DeleteThread:
    li $v1,33
    syscall
    jr $ra
    nop
    .end DeleteThread
    .size DeleteThread,.-DeleteThread
