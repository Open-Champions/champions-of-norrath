/* Observed sceSifGetReg at 0x002d54f0. Sixteen bytes: service 122, then
 * return. The register index stays in its incoming register and the kernel
 * result stays in v0. What the kernel does with the index is not established
 * here.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceSifGetReg
    .type sceSifGetReg,@function
    .ent sceSifGetReg
sceSifGetReg:
    li $v1,122
    syscall
    jr $ra
    nop
    .end sceSifGetReg
    .size sceSifGetReg,.-sceSifGetReg
