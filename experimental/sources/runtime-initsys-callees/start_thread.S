/* Thread-start request. Service 34; the argument and kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl StartThread
    .type StartThread,@function
    .ent StartThread
StartThread:
    li $v1,34
    syscall
    jr $ra
    nop
    .end StartThread
    .size StartThread,.-StartThread
