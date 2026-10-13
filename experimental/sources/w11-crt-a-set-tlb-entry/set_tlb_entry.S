/* Kernel-service request stub _SetTLBEntry (original address 0x002dfcc8, 16 bytes).
 * Service 86; arguments and the kernel result stay in their incoming registers.
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
