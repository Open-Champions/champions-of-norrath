/* Observed WaitSema at 0x002d5120. Sixteen bytes: service 68, then return.
 * The semaphore id stays in the incoming argument register. The kernel result
 * stays in v0. What service 68 does with that register is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl WaitSema
    .type WaitSema,@function
    .ent WaitSema
WaitSema:
    li $v1,68
    syscall
    jr $ra
    nop
    .end WaitSema
    .size WaitSema,.-WaitSema
