/* Copy request used by InitTLBFunctions (the local Copy at 0x002dfb78).
 * Service 90; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_tlb_copy
    .type kernel_tlb_copy,@function
    .ent kernel_tlb_copy
kernel_tlb_copy:
    li $v1,90
    syscall
    jr $ra
    nop
    .end kernel_tlb_copy
    .size kernel_tlb_copy,.-kernel_tlb_copy
