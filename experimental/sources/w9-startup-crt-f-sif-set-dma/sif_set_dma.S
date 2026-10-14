/* Observed sceSifSetDma at 0x2d54a0, 16 bytes. Kernel service 119. The arguments and the
 * kernel result stay in their incoming registers. What the kernel does with
 * them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceSifSetDma
    .type sceSifSetDma,@function
    .ent sceSifSetDma
sceSifSetDma:
    li $v1,119
    syscall
    jr $ra
    nop
    .end sceSifSetDma
    .size sceSifSetDma,.-sceSifSetDma
