/* Observed SleepThread at 0x2d5000, 16 bytes. Kernel service 50. The arguments and the kernel
 * result stay in their incoming registers. What the kernel does with them is
 * unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl SleepThread
    .type SleepThread,@function
    .ent SleepThread
SleepThread:
    li $v1,50
    syscall
    jr $ra
    nop
    .end SleepThread
    .size SleepThread,.-SleepThread
