/* Observed: the local kCopy at 0x002dfb88, 56 bytes. The TLB SysEntry table holds
 * its address as the second word of its first pair, next to the number 90.
 * It copies a2 >> 2 words from a1 to a0, one word per pass, and returns 0.
 * A zero word count skips the loop. The delay-slot and no-op placement
 * below are the observed bytes. Its 56 bytes are identical to the other
 * kCopy bodies, and the table holds the address of this copy, not of
 * kernel_syscall_pattern_kcopy, so it is a separate definition.
 * Unknown: what the kernel does with the table entry.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_tlb_pattern_kcopy
    .type kernel_tlb_pattern_kcopy,@function
    .ent kernel_tlb_pattern_kcopy
kernel_tlb_pattern_kcopy:
    srl $a2,$a2,2
    beqz $a2,.Ltlb_pattern_kcopy_done
    daddu $a3,$zero,$zero
    nop
.Ltlb_pattern_kcopy_loop:
    lw $v1,0($a1)
    addiu $a3,$a3,1
    addiu $a1,$a1,4
    sltu $v0,$a3,$a2
    sw $v1,0($a0)
    addiu $a0,$a0,4
    bnez $v0,.Ltlb_pattern_kcopy_loop
    nop
.Ltlb_pattern_kcopy_done:
    jr $ra
    daddu $v0,$zero,$zero
    .end kernel_tlb_pattern_kcopy
    .size kernel_tlb_pattern_kcopy,.-kernel_tlb_pattern_kcopy
