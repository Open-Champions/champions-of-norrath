/* Observed RemoveIntcHandler at 0x002d4de0. Sixteen bytes: service 17, then
 * return. Cause and handler id stay in the incoming argument registers. The
 * kernel result stays in v0.
 */
    .set noreorder
    .text
    .balign 8
    .globl RemoveIntcHandler
    .type RemoveIntcHandler,@function
    .ent RemoveIntcHandler
RemoveIntcHandler:
    li $v1,17
    syscall
    jr $ra
    nop
    .end RemoveIntcHandler
    .size RemoveIntcHandler,.-RemoveIntcHandler
