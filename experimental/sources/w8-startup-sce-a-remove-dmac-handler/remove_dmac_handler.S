/* Observed RemoveDmacHandler at 0x002d4e10, 16 bytes. Kernel service 19. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl RemoveDmacHandler
    .type RemoveDmacHandler,@function
    .ent RemoveDmacHandler
RemoveDmacHandler:
    li $v1,19
    syscall
    jr $ra
    nop
    .end RemoveDmacHandler
    .size RemoveDmacHandler,.-RemoveDmacHandler
