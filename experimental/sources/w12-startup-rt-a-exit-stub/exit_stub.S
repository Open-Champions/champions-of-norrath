/* Kernel-service request stub _Exit (pinned image address 0x002d4d00, 16 bytes).
 * Observed: loads service number 4 into v1, issues syscall, returns; the argument
 * and result registers are untouched, so the caller's status goes to the kernel
 * in its incoming register and nothing is saved or restored. What the service does is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl _Exit
    .type _Exit,@function
    .ent _Exit
_Exit:
    li $v1,4
    syscall
    jr $ra
    nop
    .end _Exit
    .size _Exit,.-_Exit
