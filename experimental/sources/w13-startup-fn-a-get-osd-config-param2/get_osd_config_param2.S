/* Kernel-service request stub GetOsdConfigParam2 (pinned image address 0x002d53f0, 16 bytes).
 * Observed: loads service number 111 into v1, issues syscall, returns; arguments and the
 * result stay in their incoming registers. It is the sibling of GetOsdConfigParam, which
 * uses service 75 and sits at 0x002d5190. Both are called by the system-configuration code.
 * Unknown: what the service reads from its arguments and what it returns; the name suffix
 * is the image's own symbol and says nothing about the argument layout.
 */
    .set noreorder
    .text
    .balign 8
    .globl GetOsdConfigParam2
    .type GetOsdConfigParam2,@function
    .ent GetOsdConfigParam2
GetOsdConfigParam2:
    li $v1,111
    syscall
    jr $ra
    nop
    .end GetOsdConfigParam2
    .size GetOsdConfigParam2,.-GetOsdConfigParam2
