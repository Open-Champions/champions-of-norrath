/* Semaphore creation request. Service 64; the argument and kernel result stay in
 * their incoming registers. No semaphore object is created here.
 */
    .set noreorder
    .text
    .balign 8
    .globl CreateSema
    .type CreateSema,@function
    .ent CreateSema
CreateSema:
    li $v1,64
    syscall
    jr $ra
    nop
    .end CreateSema
    .size CreateSema,.-CreateSema
