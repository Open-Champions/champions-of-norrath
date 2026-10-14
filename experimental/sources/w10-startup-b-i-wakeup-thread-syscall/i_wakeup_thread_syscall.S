/* Observed _iWakeupThread at 0x002d5020, 16 bytes. Kernel service number -52. The
 * argument and the kernel result stay in their incoming registers. It is the
 * routine that iWakeupThread (w9-startup-crt-d) calls for a thread other than the
 * current one. What the kernel does with the argument is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl _iWakeupThread
    .type _iWakeupThread,@function
    .ent _iWakeupThread
_iWakeupThread:
    li $v1,-52
    syscall
    jr $ra
    nop
    .end _iWakeupThread
    .size _iWakeupThread,.-_iWakeupThread
