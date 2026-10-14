/* Timer-2 count register at 0xb0001000. The value moves to a1 and SetT2
 * performs the store.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetT2_COUNT
    .type SetT2_COUNT,@function
    .ent SetT2_COUNT
SetT2_COUNT:
    daddu $a1, $a0, $zero
    lui $a0, 0xb000
    j SetT2
    ori $a0, $a0, 0x1000
    .end SetT2_COUNT
    .size SetT2_COUNT,.-SetT2_COUNT
