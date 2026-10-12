/* Semaphore-signal request (SignalSema at 0x002d5100).
 * Service 66; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl SignalSema
    .type SignalSema,@function
    .ent SignalSema
SignalSema:
    li $v1,66
    syscall
    jr $ra
    nop
    .end SignalSema
    .size SignalSema,.-SignalSema
