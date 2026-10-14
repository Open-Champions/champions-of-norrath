/* Entry-address request used by InitAlarm (the local GetEntryAddress at 0x002dddb8).
 * Service 91; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_alarm_get_entry
    .type kernel_alarm_get_entry,@function
    .ent kernel_alarm_get_entry
kernel_alarm_get_entry:
    li $v1,91
    syscall
    jr $ra
    nop
    .end kernel_alarm_get_entry
    .size kernel_alarm_get_entry,.-kernel_alarm_get_entry
