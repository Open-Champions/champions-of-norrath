/* Address-lookup request. Service 131; the argument and kernel result stay in
 * their incoming registers. This is not kFindAddress.
 */
    .set noreorder
    .text
    .balign 8
    .globl FindAddress
    .type FindAddress,@function
    .ent FindAddress
FindAddress:
    li $v1,131
    syscall
    jr $ra
    nop
    .end FindAddress
    .size FindAddress,.-FindAddress
