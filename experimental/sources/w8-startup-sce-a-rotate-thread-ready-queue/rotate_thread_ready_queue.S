/* Observed RotateThreadReadyQueue at 0x002d4f90, 16 bytes. Kernel service 43. The arguments and
 * the kernel result stay in their incoming registers. What the kernel does
 * with them is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl RotateThreadReadyQueue
    .type RotateThreadReadyQueue,@function
    .ent RotateThreadReadyQueue
RotateThreadReadyQueue:
    li $v1,43
    syscall
    jr $ra
    nop
    .end RotateThreadReadyQueue
    .size RotateThreadReadyQueue,.-RotateThreadReadyQueue
