/* Observed WakeupThread at 0x002d5010, 16 bytes. Kernel service 51. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl WakeupThread
    .type WakeupThread,@function
    .ent WakeupThread
WakeupThread:
    li $v1,51
    syscall
    jr $ra
    nop
    .end WakeupThread
    .size WakeupThread,.-WakeupThread
