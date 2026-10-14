/* Copy request used by InitExecPS2 (the local Copy at 0x002dd950).
 * Service 90; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_execps2_copy
    .type kernel_execps2_copy,@function
    .ent kernel_execps2_copy
kernel_execps2_copy:
    li $v1,90
    syscall
    jr $ra
    nop
    .end kernel_execps2_copy
    .size kernel_execps2_copy,.-kernel_execps2_copy
