/* Observed sceSifSetDChain at 0x002d54c0. The body is sixteen bytes:
 * service 120, then return. This function writes no argument. v0 keeps the
 * kernel result; the published void import ignores it. Service -120 is the
 * separate isceSifSetDChain request and is not this function. What service
 * 120 programs is unknown. The published import name is
 * norrath_sound_set_dma_chain.
 */
    .set noreorder
    .text
    .balign 8
    .globl norrath_sound_set_dma_chain
    .type norrath_sound_set_dma_chain,@function
    .ent norrath_sound_set_dma_chain
norrath_sound_set_dma_chain:
    li $v1,120
    syscall
    jr $ra
    nop
    .end norrath_sound_set_dma_chain
    .size norrath_sound_set_dma_chain,.-norrath_sound_set_dma_chain
