/* Observed GsPutIMR at 0x002d5420. Sixteen bytes: service 113, then return.
 * The mask stays in the incoming argument register. The kernel result stays
 * in v0.
 */
    .set noreorder
    .text
    .balign 8
    .globl GsPutIMR
    .type GsPutIMR,@function
    .ent GsPutIMR
GsPutIMR:
    li $v1,113
    syscall
    jr $ra
    nop
    .end GsPutIMR
    .size GsPutIMR,.-GsPutIMR
