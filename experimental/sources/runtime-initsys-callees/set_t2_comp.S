/* Timer-2 compare register at 0xb0001020. The value moves to a1 and SetT2
 * performs the store.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetT2_COMP
    .type SetT2_COMP,@function
    .ent SetT2_COMP
SetT2_COMP:
    daddu $a1, $a0, $zero
    lui $a0, 0xb000
    j SetT2
    ori $a0, $a0, 0x1020
    .end SetT2_COMP
    .size SetT2_COMP,.-SetT2_COMP
