/* Write one timer-2 register. When COP0 Status bit 16 is set, disable
 * interrupts and wait until that bit reads clear. Status bit 2 is set and
 * bit 1 is cleared, the saved bit 16 is restored into the status word, and
 * the caller's value is stored through a0. Return uses ERET to the address
 * held in RA at the call.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetT2
    .type SetT2,@function
    .ent SetT2
SetT2:
    mfc0 $v1, $12
    lui $v0, 1
    and $v1, $v1, $v0
    beqz $v1, .Ljoin
    lui $a2, 1
    nop
.Lspin:
    di
    sync.p
    mfc0 $v0, $12
    and $v0, $v0, $a2
    nop
    nop
    nop
    bnez $v0, .Lspin
    nop
.Ljoin:
    mfc0 $v0, $12
    sync.p
    ori $v0, $v0, 6
    xori $v0, $v0, 2
    or $v0, $v0, $v1
    mtc0 $v0, $12
    sync.p
    sw $a1, 0($a0)
    mtc0 $ra, $30
    sync.p
    eret
    jr $ra
    nop
    .end SetT2
    .size SetT2,.-SetT2
