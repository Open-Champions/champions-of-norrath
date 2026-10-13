/* Kernel-service request stub _Exit (original address 0x002d4d00, 16 bytes).
 * Service 4; the status argument stays in its incoming register and the
 * kernel result is left in the result register.
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
