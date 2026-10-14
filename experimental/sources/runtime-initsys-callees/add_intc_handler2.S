/* Interrupt-handler registration request. Service 16; the argument and kernel
 * result stay in their incoming registers. No handler table is built here.
 */
    .set noreorder
    .text
    .balign 8
    .globl AddIntcHandler2
    .type AddIntcHandler2,@function
    .ent AddIntcHandler2
AddIntcHandler2:
    li $v1,16
    syscall
    jr $ra
    nop
    .end AddIntcHandler2
    .size AddIntcHandler2,.-AddIntcHandler2
