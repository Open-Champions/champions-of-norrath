/* Observed AddIntcHandler at 0x002d4dc0. Sixteen bytes: service 16, then
 * return. Cause, handler, and next stay in the incoming argument registers.
 * The kernel result stays in v0. AddIntcHandler2 at 0x002d4dd0 is the same
 * service and is a different symbol.
 */
    .set noreorder
    .text
    .balign 8
    .globl AddIntcHandler
    .type AddIntcHandler,@function
    .ent AddIntcHandler
AddIntcHandler:
    li $v1,16
    syscall
    jr $ra
    nop
    .end AddIntcHandler
    .size AddIntcHandler,.-AddIntcHandler
