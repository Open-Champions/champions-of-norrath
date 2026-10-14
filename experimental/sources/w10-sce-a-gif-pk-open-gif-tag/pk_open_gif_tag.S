/* Observed sceGifPkOpenGifTag at 0x0133f210, 24 bytes, in the 0x01000000 module
 * image. The packet state is in $a0 (cursor in its first word) and the 16-byte
 * tag value arrives as one 128-bit value in $a1. The tag is stored with a
 * quadword store at the cursor, the cursor advances by 16, and the tag
 * address is kept in the state's fourth word (+12) for sceGifPkCloseGifTag.
 * The old cursor is left in $v0. The 128-bit argument cannot be received by
 * compiled code with the reviewed compiler (it passes such a value by
 * reference), so this routine is authored as assembly and compared byte for
 * byte with the original.
 */
    .set noreorder
    .set noat
    .text
    .balign 8
    .globl sceGifPkOpenGifTag
    .type sceGifPkOpenGifTag,@function
    .ent sceGifPkOpenGifTag
sceGifPkOpenGifTag:
    lw $v0,0($a0)
    addiu $v1,$v0,16
    sq $a1,0($v0)
    sw $v1,0($a0)
    jr $ra
    sw $v0,12($a0)
    .end sceGifPkOpenGifTag
    .size sceGifPkOpenGifTag,.-sceGifPkOpenGifTag
