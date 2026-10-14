/* Observed sceGsResetPath at 0x002d1888, 104 bytes. It writes 1 to
 * 0x10003c10 and 2 to 0x10003c20, sets bit 9 of VU0 vi28, copies two
 * quadwords from 0x00444890 into 0x10005000, and writes 1 to 0x10003000.
 * The quadword source is the 32-byte init_vif_regs.12 object. This body
 * uses that absolute address. Hardware effects of those stores are unknown.
 */
    .set noreorder
    .text
    .balign 8
    .globl sceGsResetPath
    .type sceGsResetPath,@function
    .ent sceGsResetPath
sceGsResetPath:
    lui $v0,0x1000
    li $a3,1
    ori $v0,$v0,0x3c10
    lui $v1,0x1000
    sw $a3,0($v0)
    ori $v1,$v1,0x3c20
    li $v0,2
    daddu $a0,$zero,$zero
    sw $v0,0($v1)
    sync
    cfc2 $a0,$28
    ori $a0,$a0,0x200
    ctc2 $a0,$28
    sync.p
    lui $a1,0x44
    lui $a2,0x1000
    addiu $a1,$a1,18576
    ori $a2,$a2,0x5000
    lq $a0,0($a1)
    lui $v1,0x1000
    ori $v1,$v1,0x3000
    sq $a0,0($a2)
    lq $v0,16($a1)
    sq $v0,0($a2)
    jr $ra
    sw $a3,0($v1)
    .end sceGsResetPath
    .size sceGsResetPath,.-sceGsResetPath
