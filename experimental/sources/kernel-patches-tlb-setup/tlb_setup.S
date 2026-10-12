/* Setup request used by InitTLBFunctions (the local setup at 0x002dfbd0).
 * Service 116; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_tlb_setup
    .type kernel_tlb_setup,@function
    .ent kernel_tlb_setup
kernel_tlb_setup:
    li $v1,116
    syscall
    jr $ra
    nop
    .end kernel_tlb_setup
    .size kernel_tlb_setup,.-kernel_tlb_setup
