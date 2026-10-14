/* Independently authored EE bootstrap for the pinned entry contract.
 * No incoming stack or GP is usable. Kernel services remain external.
 * The pinned optional _init/_fini targets are both zero and are omitted.
 */
    .set noreorder
    .set noat
    .section .text.startup,"ax",@progbits
    .balign 8
    .globl _start
    .type _start,@function
    .ent _start
_start:
    /* Clear all ordinary 128-bit GPRs; preserve kernel registers k0/k1. */
    .irp reg,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,28,29,30,31
    por $\reg,$zero,$zero
    .endr
    mthi $zero
    mtlo $zero
    mthi1 $zero
    mtlo1 $zero
    mtsah $zero,0
    .irp reg,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    mtc1 $zero,$f\reg
    .endr
    adda.s $f0,$f1
    sync.p
    ctc1 $zero,$31

    /* Preserve byte-prefix / aligned quadword / byte-tail clearing.
     * _bssend deliberately differs from _end: do not erase the SRS tables.
     */
    la $t0,_fbss
    la $t1,_bssend
.Lprefix:
    andi $t2,$t0,15
    beq $t2,$zero,.Lwide_setup
    nop
    sb $zero,0($t0)
    b .Lprefix
    addiu $t0,$t0,1
.Lwide_setup:
    li $t2,-16
    and $t2,$t1,$t2
.Lwide:
    beq $t0,$t2,.Ltail
    nop
    sq $zero,0($t0)
    b .Lwide
    addiu $t0,$t0,16
.Ltail:
    beq $t0,$t1,.Lkernel
    nop
    sb $zero,0($t0)
    b .Ltail
    addiu $t0,$t0,1
.Lkernel:
    la $a0,_gp
    move $gp,$a0
    li $a1,0x01fc0000
    li $a2,0x00040000
    la $a3,_args
    la $t0,_root
    li $v1,60
    syscall
    move $sp,$v0

    la $a0,_end
    li $a1,-1
    li $v1,61
    syscall
    jal _InitSys
    nop
    jal FlushCache
    move $a0,$zero
    ei
    la $t0,_args
    lw $a0,0($t0)
    jal main
    addiu $a1,$t0,4
    j exit
    move $a0,$v0
    .end _start
    .size _start,.-_start
