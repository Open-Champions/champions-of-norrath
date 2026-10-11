/* Thread-creation request. Service 32; the argument and kernel result stay in
 * their incoming registers. No thread object is created here.
 */
    .set noreorder
    .text
    .balign 8
    .globl CreateThread
    .type CreateThread,@function
    .ent CreateThread
CreateThread:
    li $v1,32
    syscall
    jr $ra
    nop
    .end CreateThread
    .size CreateThread,.-CreateThread
