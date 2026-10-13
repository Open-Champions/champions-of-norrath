/* Observed Deci2Call at 0x2d5510, 16 bytes. Kernel service 124. The arguments and the kernel
 * result stay in their incoming registers. What the kernel does with them is
 * unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl Deci2Call
    .type Deci2Call,@function
    .ent Deci2Call
Deci2Call:
    li $v1,124
    syscall
    jr $ra
    nop
    .end Deci2Call
    .size Deci2Call,.-Deci2Call
