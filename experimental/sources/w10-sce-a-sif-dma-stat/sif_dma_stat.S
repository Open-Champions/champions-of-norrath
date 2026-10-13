/* Observed sceSifDmaStat at 0x2d5480, 16 bytes. Kernel service 118. The argument and the
 * kernel result stay in their incoming registers. What the kernel does with
 * them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceSifDmaStat
    .type sceSifDmaStat,@function
    .ent sceSifDmaStat
sceSifDmaStat:
    li $v1,118
    syscall
    jr $ra
    nop
    .end sceSifDmaStat
    .size sceSifDmaStat,.-sceSifDmaStat
