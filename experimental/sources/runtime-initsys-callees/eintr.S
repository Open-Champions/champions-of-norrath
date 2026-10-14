/* Enable EE interrupts. Returns 1 when COP0 Status bit 16 was already set.
 */
    .set noreorder
    .text
    .balign 8
    .globl EIntr
    .type EIntr,@function
    .ent EIntr
EIntr:
    mfc0 $v0, $12
    lui $v1, 1
    and $v0, $v0, $v1
    ei
    jr $ra
    sltu $v0, $zero, $v0
    .end EIntr
    .size EIntr,.-EIntr
