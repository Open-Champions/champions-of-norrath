/* Observed SuspendThread at 0x002d5050, 16 bytes. Kernel service 55. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl SuspendThread
    .type SuspendThread,@function
    .ent SuspendThread
SuspendThread:
    li $v1,55
    syscall
    jr $ra
    nop
    .end SuspendThread
    .size SuspendThread,.-SuspendThread
