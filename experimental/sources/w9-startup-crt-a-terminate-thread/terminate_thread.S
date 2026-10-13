/* Observed TerminateThread at 0x002d4f30, 16 bytes. Kernel service 37. The arguments
 * and the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl TerminateThread
    .type TerminateThread,@function
    .ent TerminateThread
TerminateThread:
    li $v1,37
    syscall
    jr $ra
    nop
    .end TerminateThread
    .size TerminateThread,.-TerminateThread
