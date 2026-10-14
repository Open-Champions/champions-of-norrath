/* Observed iSignalSema at 0x002d5110. Sixteen bytes: service -67, then
 * return. The semaphore id stays in the incoming argument register. The
 * kernel result stays in v0. SignalSema is service 66 and is not this symbol.
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
