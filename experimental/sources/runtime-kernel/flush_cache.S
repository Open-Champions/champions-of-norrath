/* Preserve the caller's mode argument and return the kernel's result. */
    .set noreorder
    .text
    .balign 8
    .globl FlushCache
    .type FlushCache,@function
    .ent FlushCache
FlushCache:
    li $v1,100
    syscall
    jr $ra
    nop
    .end FlushCache
    .size FlushCache,.-FlushCache
