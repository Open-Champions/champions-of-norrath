/* Observed PollSema at 0x002d5130. Sixteen bytes: service 69, then return.
 * The semaphore id stays in the incoming argument register. The kernel result
 * stays in v0.
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
