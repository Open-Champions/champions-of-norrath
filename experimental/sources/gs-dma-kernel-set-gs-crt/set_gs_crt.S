/* Observed SetGsCrt at 0x002d4ce0. Sixteen bytes: service 2, then return.
 * The three incoming argument registers are not written. The kernel result
 * stays in v0. What service 2 does with those registers is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetGsCrt
    .type SetGsCrt,@function
    .ent SetGsCrt
SetGsCrt:
    li $v1,2
    syscall
    jr $ra
    nop
    .end SetGsCrt
    .size SetGsCrt,.-SetGsCrt
