/* Observed PollSema at 0x002d5130, 16 bytes. Service 69. The identifier and
 * the kernel result stay in their incoming registers. What the kernel does
 * with the identifier is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl PollSema
    .type PollSema,@function
    .ent PollSema
PollSema:
    li $v1,69
    syscall
    jr $ra
    nop
    .end PollSema
    .size PollSema,.-PollSema
