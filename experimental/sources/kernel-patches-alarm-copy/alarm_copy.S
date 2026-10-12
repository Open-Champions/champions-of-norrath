/* Copy request used by InitAlarm (the local Copy at 0x002ddd70).
 * Service 90; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_alarm_copy
    .type kernel_alarm_copy,@function
    .ent kernel_alarm_copy
kernel_alarm_copy:
    li $v1,90
    syscall
    jr $ra
    nop
    .end kernel_alarm_copy
    .size kernel_alarm_copy,.-kernel_alarm_copy
