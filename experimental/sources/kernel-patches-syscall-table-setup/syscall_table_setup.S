/* Setup request used by InitSystemCallTableAddress (the local setup at 0x002dd8d8).
 * Service 116; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_syscall_table_setup
    .type kernel_syscall_table_setup,@function
    .ent kernel_syscall_table_setup
kernel_syscall_table_setup:
    li $v1,116
    syscall
    jr $ra
    nop
    .end kernel_syscall_table_setup
    .size kernel_syscall_table_setup,.-kernel_syscall_table_setup
