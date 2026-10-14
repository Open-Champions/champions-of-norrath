/* Observed kernel_setarg_setup at 0x002ddb00, 16 bytes. Kernel service 116. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_setarg_setup
    .type kernel_setarg_setup,@function
    .ent kernel_setarg_setup
kernel_setarg_setup:
    li $v1,116
    syscall
    jr $ra
    nop
    .end kernel_setarg_setup
    .size kernel_setarg_setup,.-kernel_setarg_setup
