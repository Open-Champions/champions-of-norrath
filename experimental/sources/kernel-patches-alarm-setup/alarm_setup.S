/* Setup request used by InitAlarm (the local setup at 0x002ddd60).
 * Service 116; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_alarm_setup
    .type kernel_alarm_setup,@function
    .ent kernel_alarm_setup
kernel_alarm_setup:
    li $v1,116
    syscall
    jr $ra
    nop
    .end kernel_alarm_setup
    .size kernel_alarm_setup,.-kernel_alarm_setup
