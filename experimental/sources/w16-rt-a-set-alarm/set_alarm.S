/* Observed SetAlarm at 0x002d4e60. The body is sixteen bytes: kernel service
 * 252, then return. The incoming argument registers are left alone and the
 * kernel result stays in v0. What service 252 does with those registers is
 * unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetAlarm
    .type SetAlarm,@function
    .ent SetAlarm
SetAlarm:
    li $v1,252
    syscall
    jr $ra
    nop
    .end SetAlarm
    .size SetAlarm,.-SetAlarm
