/* Observed iSetAlarm at 0x002d4ec0, 16 bytes. Kernel service -254. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl iSetAlarm
    .type iSetAlarm,@function
    .ent iSetAlarm
iSetAlarm:
    li $v1,-254
    syscall
    jr $ra
    nop
    .end iSetAlarm
    .size iSetAlarm,.-iSetAlarm
