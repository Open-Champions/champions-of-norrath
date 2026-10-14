/* _kExitTLBHandler (pinned image address 0x002e0100, 244 bytes). Authored assembly.
 *
 * Observed order of effects:
 *   1. Status (cp0 reg 12) is read and bits 0, 1, 3, 4 are cleared (mask -28).
 *   2. EPC (cp0 reg 14) is loaded from the word at frame offset 0x228.
 *   3. HI, HI1, LO, LO1 and SA are reloaded from 64-bit slots at 0x200, 0x208,
 *      0x210, 0x218 and 0x220.
 *   4. The 128-bit registers 1..25 and 28..31 are reloaded from frame offset 16*n,
 *      with k0 holding the frame address (registers 26 and 27 are not touched).
 *   5. Status is read again, bits 0, 1 and 4 are set (0x13), and the routine ends
 *      with eret.
 * The frame is the 0x230-byte block the pinned image starts at 0x008e5780. It is
 * imported here as norrath_ee_exception_frame; the code that fills it is not part
 * of this unit. Unknown: which exception paths reach this routine at run time.
 */
    .set noreorder
    .set noat
    .text
    .balign 4
    .globl _kExitTLBHandler
    .type _kExitTLBHandler,@function
    .ent _kExitTLBHandler
_kExitTLBHandler:
    mfc0 $1,$12
    li $26,-28
    and $1,$1,$26
    mtc0 $1,$12
    sync.p
    lui $2,%hi(norrath_ee_exception_frame+0x228)
    lw $2,%lo(norrath_ee_exception_frame+0x228)($2)
    mtc0 $2,$14
    sync.p
    lui $2,%hi(norrath_ee_exception_frame+0x200)
    ld $2,%lo(norrath_ee_exception_frame+0x200)($2)
    mthi $2
    lui $2,%hi(norrath_ee_exception_frame+0x208)
    ld $2,%lo(norrath_ee_exception_frame+0x208)($2)
    mthi1 $2
    lui $2,%hi(norrath_ee_exception_frame+0x210)
    ld $2,%lo(norrath_ee_exception_frame+0x210)($2)
    mtlo $2
    lui $2,%hi(norrath_ee_exception_frame+0x218)
    ld $2,%lo(norrath_ee_exception_frame+0x218)($2)
    mtlo1 $2
    lui $2,%hi(norrath_ee_exception_frame+0x220)
    ld $2,%lo(norrath_ee_exception_frame+0x220)($2)
    mtsa $2
    sync.p
    lui $26,%hi(norrath_ee_exception_frame)
    addiu $26,$26,%lo(norrath_ee_exception_frame)
    .irp n,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,28,29,30,31
    lq $\n,\n*16($26)
    .endr
    mfc0 $26,$12
    ori $26,$26,0x13
    mtc0 $26,$12
    sync.p
    eret
    .end _kExitTLBHandler
    .size _kExitTLBHandler,.-_kExitTLBHandler
