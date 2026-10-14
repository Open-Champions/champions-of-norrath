// Observed _kExitTLBHandler at 0x002e0100, 244 bytes, a global routine that the
// kernel-patches SysEntry table points at (service 0x54). It is written in
// assembly because it restores privileged state and returns from an exception.
//
// What it does, in order:
//  1. Clears bits 0, 1, 3 and 4 of the CP0 Status register (mask 0xffffffe4).
//  2. Loads CP0 EPC from the saved-PC word of the exception frame.
//  3. Reloads HI, HI1, LO, LO1 and SA from their frame slots.
//  4. Reloads 128-bit registers 1 through 25 and 28 through 31 (every register
//     except zero, k0 and k1), using k0 as the frame pointer.
//  5. Sets bits 0, 1 and 4 of Status (0x13) and returns with ERET.
//
// Frame layout read here, relative to the frame base (the original's static
// block at 0x008e5780, 0x230 bytes; the code that fills it is not authored):
//   register n at offset 16 * n (n = 1..25, 28..31)
//   +0x200 HI, +0x208 HI1, +0x210 LO, +0x218 LO1, +0x220 SA, +0x228 saved PC
// The frame is imported as norrath_ee_exception_frame.
asm(R"(
    .text
    .align 2
    .globl _kExitTLBHandler
    .type _kExitTLBHandler, @function
    .set push
    .set noreorder
    .set noat
    .macro load_special slot, move_op
    lui $2, %hi(norrath_ee_exception_frame + \slot)
    ld $2, %lo(norrath_ee_exception_frame + \slot)($2)
    \move_op $2
    .endm
_kExitTLBHandler:
    mfc0 $1, $12
    li $26, -28
    and $1, $1, $26
    mtc0 $1, $12
    sync.p
    lui $2, %hi(norrath_ee_exception_frame + 0x228)
    lw $2, %lo(norrath_ee_exception_frame + 0x228)($2)
    mtc0 $2, $14
    sync.p
    load_special 0x200, mthi
    load_special 0x208, mthi1
    load_special 0x210, mtlo
    load_special 0x218, mtlo1
    load_special 0x220, mtsa
    sync.p
    lui $26, %hi(norrath_ee_exception_frame)
    addiu $26, $26, %lo(norrath_ee_exception_frame)
    .irp n, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,28,29,30,31
    lq $\n, \n * 16($26)
    .endr
    mfc0 $26, $12
    ori $26, $26, 0x13
    mtc0 $26, $12
    sync.p
    eret
    .size _kExitTLBHandler, . - _kExitTLBHandler
    .purgem load_special
    .set pop
)");
