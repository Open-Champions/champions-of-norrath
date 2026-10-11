/* Observed _GetGsDxDyOffset at 0x002d1990. Sixteen bytes: service 128, then
 * return. The mode and the four destination pointers stay in the incoming
 * argument registers, including t0. The kernel result stays in v0.
 */
    .set noreorder
    .text
    .balign 8
    .globl _GetGsDxDyOffset
    .type _GetGsDxDyOffset,@function
    .ent _GetGsDxDyOffset
_GetGsDxDyOffset:
    li $v1,128
    syscall
    jr $ra
    nop
    .end _GetGsDxDyOffset
    .size _GetGsDxDyOffset,.-_GetGsDxDyOffset
