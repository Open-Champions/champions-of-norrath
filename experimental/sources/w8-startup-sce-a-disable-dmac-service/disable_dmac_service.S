/* Observed _DisableDmac at 0x002d4e50, 16 bytes. Kernel service 23. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl _DisableDmac
    .type _DisableDmac,@function
    .ent _DisableDmac
_DisableDmac:
    li $v1,23
    syscall
    jr $ra
    nop
    .end _DisableDmac
    .size _DisableDmac,.-_DisableDmac
