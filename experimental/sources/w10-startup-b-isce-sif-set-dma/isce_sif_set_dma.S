/* Observed isceSifSetDma at 0x002d54b0, 16 bytes. Kernel service number -119. The
 * arguments and the kernel result stay in their incoming registers. sceSifSetDma
 * (w9-startup-crt-f) and the SIF command sender call it. What the kernel does with
 * the arguments is unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl isceSifSetDma
    .type isceSifSetDma,@function
    .ent isceSifSetDma
isceSifSetDma:
    li $v1,-119
    syscall
    jr $ra
    nop
    .end isceSifSetDma
    .size isceSifSetDma,.-isceSifSetDma
