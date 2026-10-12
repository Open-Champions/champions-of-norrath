/* Observed sceSifStopDma at 0x2d53b0, 16 bytes. Kernel service 107. The arguments and the
 * kernel result stay in their incoming registers. What the kernel does with
 * them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceSifStopDma
    .type sceSifStopDma,@function
    .ent sceSifStopDma
sceSifStopDma:
    li $v1,107
    syscall
    jr $ra
    nop
    .end sceSifStopDma
    .size sceSifStopDma,.-sceSifStopDma
