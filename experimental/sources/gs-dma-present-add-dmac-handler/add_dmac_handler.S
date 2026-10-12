/* Observed AddDmacHandler at 0x002d4df0. Sixteen bytes: service 18, then
 * return. The channel, handler, and next id stay in the incoming argument
 * registers. The kernel result stays in v0. What service 18 does with those
 * registers is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl AddDmacHandler
    .type AddDmacHandler,@function
    .ent AddDmacHandler
AddDmacHandler:
    li $v1,18
    syscall
    jr $ra
    nop
    .end AddDmacHandler
    .size AddDmacHandler,.-AddDmacHandler
