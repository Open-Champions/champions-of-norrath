/* Observed _sceIDC at 0x002d5a70, 164 bytes. Arguments: a0 = first address, a1 =
 * second address (InvalidDCache passes both rounded down to 64). A loop counter
 * runs over 0, 64, ... 4032 (the 64 sets of the data cache, one per iteration);
 * each iteration handles way 0 and then way 1 of that set the same way:
 *   - sync; cache op 0x10 at (counter + way); sync; read the tag-low register
 *     (coprocessor-0 register 28);
 *   - the tag is masked with 0xfffff000 and the counter is added, giving the
 *     address of the line held there;
 *   - when that address is not below a0 and not above a1 (both unsigned), sync;
 *     cache op 0x16 at (counter + way); sync;
 *   - sync after each way.
 * Nothing is returned. Inferred, from the public R5900 cache-operation list: op
 * 0x10 is "index load tag" and op 0x16 is "index store tag", which writes a zero
 * tag and so invalidates the line; this makes the routine an invalidate of every
 * data-cache line whose address lies in [a0, a1]. The caller is InvalidDCache.
 * Unknown: why the line address adds the set counter to the masked tag (it is
 * written exactly so), and the cache geometry beyond the loop bound.
 */
    .set noreorder
    .text
    .balign 8
    .globl _sceIDC
    .type _sceIDC,@function
    .ent _sceIDC
_sceIDC:
    lui $a3,0xffff
    daddu $a2,$zero,$zero
    ori $a3,$a3,0xf000
    nop
1:
    sync
    cache 0x10,0($a2)
    sync
    mfc0 $v0,$28
    and $v0,$v0,$a3
    addu $v0,$v0,$a2
    sltu $v1,$a1,$v0
    sltu $v0,$v0,$a0
    bnez $v0,2f
    nop
    bnez $v1,2f
    nop
    sync
    cache 0x16,0($a2)
    sync
2:
    sync
    cache 0x10,1($a2)
    sync
    mfc0 $v0,$28
    and $v0,$v0,$a3
    addu $v0,$v0,$a2
    sltu $v1,$a1,$v0
    sltu $v0,$v0,$a0
    bnez $v0,3f
    nop
    bnez $v1,3f
    nop
    sync
    cache 0x16,1($a2)
    sync
3:
    sync
    addiu $a2,$a2,64
    slti $v0,$a2,4096
    bnez $v0,1b
    nop
    jr $ra
    nop
    .end _sceIDC
    .size _sceIDC,.-_sceIDC
