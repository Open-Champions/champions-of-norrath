/* Thread-id request. Service 47; the argument and kernel result stay in their
 * incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl GetThreadId
    .type GetThreadId,@function
    .ent GetThreadId
GetThreadId:
    li $v1,47
    syscall
    jr $ra
    nop
    .end GetThreadId
    .size GetThreadId,.-GetThreadId
