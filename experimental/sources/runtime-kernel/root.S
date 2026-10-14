/* Entry's kernel root callback: request service 35 without using a stack.
 * No kernel return or termination behavior is invented after this syscall.
 */
    .set noreorder
    .text
    .balign 8
    .globl _root
    .type _root,@function
    .ent _root
_root:
    li $v1,35
    syscall
    .end _root
    .size _root,.-_root
