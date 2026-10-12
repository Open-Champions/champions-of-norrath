/* Entry-address request used by InitTLBFunctions (the local GetEntryAddress at 0x002dfbc0).
 * Service 91; the argument registers and the kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_tlb_get_entry
    .type kernel_tlb_get_entry,@function
    .ent kernel_tlb_get_entry
kernel_tlb_get_entry:
    li $v1,91
    syscall
    jr $ra
    nop
    .end kernel_tlb_get_entry
    .size kernel_tlb_get_entry,.-kernel_tlb_get_entry
