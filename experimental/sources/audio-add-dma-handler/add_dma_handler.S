/* Observed AddDmacHandler at 0x002d4df0. The body is sixteen bytes:
 * service 18, then return. Channel, handler, and next-handler stay in the
 * incoming argument registers. The kernel result stays in v0. What service
 * 18 does with those registers is unknown. The published import name is
 * norrath_sound_add_dma_handler.
 */
    .set noreorder
    .text
    .balign 8
    .globl norrath_sound_add_dma_handler
    .type norrath_sound_add_dma_handler,@function
    .ent norrath_sound_add_dma_handler
norrath_sound_add_dma_handler:
    li $v1,18
    syscall
    jr $ra
    nop
    .end norrath_sound_add_dma_handler
    .size norrath_sound_add_dma_handler,.-norrath_sound_add_dma_handler
