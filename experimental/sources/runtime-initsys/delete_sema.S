/* Semaphore deletion request. Service 65; the identifier and kernel result stay
 * in their incoming registers.
 */
    .set noreorder
    .text
    .balign 8
    .globl DeleteSema
    .type DeleteSema,@function
    .ent DeleteSema
DeleteSema:
    li $v1,65
    syscall
    jr $ra
    nop
    .end DeleteSema
    .size DeleteSema,.-DeleteSema
