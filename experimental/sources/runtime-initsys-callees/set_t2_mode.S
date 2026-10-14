/* Timer-2 mode register at 0xb0001010. The value moves to a1 and SetT2
 * performs the store.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetT2_MODE
    .type SetT2_MODE,@function
    .ent SetT2_MODE
SetT2_MODE:
    daddu $a1, $a0, $zero
    lui $a0, 0xb000
    j SetT2
    ori $a0, $a0, 0x1010
    .end SetT2_MODE
    .size SetT2_MODE,.-SetT2_MODE
