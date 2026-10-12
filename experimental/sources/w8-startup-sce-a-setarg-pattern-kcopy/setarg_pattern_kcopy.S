/* Observed the local kCopy at 0x002ddad0, 48 bytes, in the object that holds
 * SetArg. The word at 0x004451fc, the second word of SetArg's SysEntry pair
 * (next to the number 90), holds its address, so the bytes themselves are the
 * data. It copies a2 bytes from a1 to a0, one byte per pass, and returns 0. A
 * zero count skips the loop. The delay-slot filler is the observed one. The
 * name is an added one; it differs from the 56-byte word-wise kCopy bodies of
 * the other kernel patch objects.
 * Unknown: what the kernel does with the table entry.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_setarg_pattern_kcopy
    .type kernel_setarg_pattern_kcopy,@function
    .ent kernel_setarg_pattern_kcopy
kernel_setarg_pattern_kcopy:
    beqz $a2,.Lsetarg_pattern_kcopy_done
    daddu $a3,$zero,$zero
.Lsetarg_pattern_kcopy_loop:
    lbu $v1,0($a1)
    addiu $a3,$a3,1
    addiu $a1,$a1,1
    sltu $v0,$a3,$a2
    sb $v1,0($a0)
    addiu $a0,$a0,1
    bnez $v0,.Lsetarg_pattern_kcopy_loop
    nop
.Lsetarg_pattern_kcopy_done:
    jr $ra
    daddu $v0,$zero,$zero
    .end kernel_setarg_pattern_kcopy
    .size kernel_setarg_pattern_kcopy,.-kernel_setarg_pattern_kcopy
