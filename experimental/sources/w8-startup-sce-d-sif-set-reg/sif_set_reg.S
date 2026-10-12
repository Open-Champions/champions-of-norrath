/* Observed sceSifSetReg at 0x002d54e0. Sixteen bytes: service 121, then
 * return. Both arguments stay in their incoming registers and the kernel
 * result stays in v0. What the kernel does with them is not established here;
 * the callers in the SIF code use it with a register index and a value.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceSifSetReg
    .type sceSifSetReg,@function
    .ent sceSifSetReg
sceSifSetReg:
    li $v1,121
    syscall
    jr $ra
    nop
    .end sceSifSetReg
    .size sceSifSetReg,.-sceSifSetReg
