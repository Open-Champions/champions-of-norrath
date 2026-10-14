/* Observed GetMemorySize at 0x2d5540, 16 bytes. Kernel service 127. The arguments and the kernel
 * result stay in their incoming registers. What the kernel does with them is
 * unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl GetMemorySize
    .type GetMemorySize,@function
    .ent GetMemorySize
GetMemorySize:
    li $v1,127
    syscall
    jr $ra
    nop
    .end GetMemorySize
    .size GetMemorySize,.-GetMemorySize
