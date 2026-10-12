/* Observed SetVSyncFlag at 0x002d5450, 16 bytes. Kernel service 115. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl SetVSyncFlag
    .type SetVSyncFlag,@function
    .ent SetVSyncFlag
SetVSyncFlag:
    li $v1,115
    syscall
    jr $ra
    nop
    .end SetVSyncFlag
    .size SetVSyncFlag,.-SetVSyncFlag
