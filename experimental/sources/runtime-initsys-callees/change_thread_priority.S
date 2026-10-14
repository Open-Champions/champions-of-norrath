/* Thread-priority request. Service 41; the argument and kernel result stay in
 * their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl ChangeThreadPriority
    .type ChangeThreadPriority,@function
    .ent ChangeThreadPriority
ChangeThreadPriority:
    li $v1,41
    syscall
    jr $ra
    nop
    .end ChangeThreadPriority
    .size ChangeThreadPriority,.-ChangeThreadPriority
