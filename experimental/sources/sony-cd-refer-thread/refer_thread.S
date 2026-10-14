/* Observed ReferThreadStatus at 0x002d4fe0, 16 bytes. Service 48. The thread
 * identifier, the status buffer, and the kernel result stay in their incoming
 * registers. The buffer layout is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl ReferThreadStatus
    .type ReferThreadStatus,@function
    .ent ReferThreadStatus
ReferThreadStatus:
    li $v1,48
    syscall
    jr $ra
    nop
    .end ReferThreadStatus
    .size ReferThreadStatus,.-ReferThreadStatus
