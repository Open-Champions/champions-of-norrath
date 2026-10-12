/* Observed iSignalSema at 0x002d5110, 16 bytes. Service -67. The identifier
 * and the kernel result stay in their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl iSignalSema
    .type iSignalSema,@function
    .ent iSignalSema
iSignalSema:
    li $v1,-67
    syscall
    jr $ra
    nop
    .end iSignalSema
    .size iSignalSema,.-iSignalSema
