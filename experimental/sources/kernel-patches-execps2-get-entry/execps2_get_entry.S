/* Entry-address request used by InitExecPS2 (the local GetEntryAddress at 0x002dd998).
 * Service 91; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_execps2_get_entry
    .type kernel_execps2_get_entry,@function
    .ent kernel_execps2_get_entry
kernel_execps2_get_entry:
    li $v1,91
    syscall
    jr $ra
    nop
    .end kernel_execps2_get_entry
    .size kernel_execps2_get_entry,.-kernel_execps2_get_entry
