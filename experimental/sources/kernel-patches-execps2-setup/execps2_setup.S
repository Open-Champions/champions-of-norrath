/* Setup request used by InitExecPS2 (the local setup at 0x002dd940).
 * Service 116; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_execps2_setup
    .type kernel_execps2_setup,@function
    .ent kernel_execps2_setup
kernel_execps2_setup:
    li $v1,116
    syscall
    jr $ra
    nop
    .end kernel_execps2_setup
    .size kernel_execps2_setup,.-kernel_execps2_setup
