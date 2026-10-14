/* Disable EE interrupts when COP0 Status bit 16 is set. Returns 1 after that
 * bit reads clear, and 0 when it was already clear. The spin repeats the
 * disable until the bit drops.
 */
    .set noreorder
    .text
    .balign 8
    .globl DIntr
    .type DIntr,@function
    .ent DIntr
DIntr:
    mfc0 $v1, $12
    lui $v0, 1
    and $v1, $v1, $v0
    beqz $v1, .Lalready
    sltu $a0, $zero, $v1
    nop
.Lspin:
    di
    sync.p
    mfc0 $v0, $12
    lui $v1, 1
    and $v0, $v0, $v1
    nop
    nop
    bnez $v0, .Lspin
    nop
    jr $ra
    daddu $v0, $a0, $zero
.Lalready:
    daddu $a0, $zero, $zero
    daddu $v0, $a0, $zero
    jr $ra
    nop
    .end DIntr
    .size DIntr,.-DIntr
