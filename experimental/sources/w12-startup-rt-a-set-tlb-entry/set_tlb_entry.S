/* Kernel-service request stub _SetTLBEntry (pinned image address 0x002dfcc8, 16 bytes).
 * Observed: loads service number 86 into v1, issues syscall, returns; the argument
 * and result registers are untouched, so the caller's arguments go to the kernel
 * in their incoming registers and nothing is saved or restored. What the service does is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl _SetTLBEntry
    .type _SetTLBEntry,@function
    .ent _SetTLBEntry
_SetTLBEntry:
    li $v1,86
    syscall
    jr $ra
    nop
    .end _SetTLBEntry
    .size _SetTLBEntry,.-_SetTLBEntry
