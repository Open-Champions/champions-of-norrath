/* Observed sceSifSetDChain at 0x2d54c0, 16 bytes. Kernel service 120. The arguments and the
 * kernel result stay in their incoming registers. What the kernel does with
 * them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceSifSetDChain
    .type sceSifSetDChain,@function
    .ent sceSifSetDChain
sceSifSetDChain:
    li $v1,120
    syscall
    jr $ra
    nop
    .end sceSifSetDChain
    .size sceSifSetDChain,.-sceSifSetDChain
