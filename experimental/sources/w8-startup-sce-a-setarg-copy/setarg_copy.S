/* Observed kernel_setarg_copy at 0x002ddac0, 16 bytes. Kernel service 90. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_setarg_copy
    .type kernel_setarg_copy,@function
    .ent kernel_setarg_copy
kernel_setarg_copy:
    li $v1,90
    syscall
    jr $ra
    nop
    .end kernel_setarg_copy
    .size kernel_setarg_copy,.-kernel_setarg_copy
