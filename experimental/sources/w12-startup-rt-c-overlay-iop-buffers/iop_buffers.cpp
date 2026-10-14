/* Observed: five local data objects of the sound/IOP interface code loaded above the main
 * image, from the executable's symbol table (addresses in the pinned SLUS_205.65):
 *   sSend            0x011edb00  512 bytes (next symbol sRecv at +0x200), 64-byte aligned
 *   sRecv            0x011edd00  512 bytes (next symbol sFunctionIndex at +0x200)
 *   sFunctionIndex   0x011edf00  8-byte slot (next symbol 8 bytes later); the first word is
 *                                the command cursor used by the authored SetIopFunction__FUii
 *                                and FlushIopCommand__Fb
 *   sIopFunctionData 0x011edf08  512 bytes (next symbol sSifClient at +0x200)
 *   sSifClient       0x011ee108  at most 56 bytes (next symbol sVagTransferBuffer at +0x38); the
 *                                RPC client record of the sound server
 * All five read as zero in the image, so they are defined uninitialised. Their sizes are
 * the distances to the next symbol, which is an upper bound for sSifClient (the image gives no
 * size). A second, unrelated set of objects with the same names exists near 0x011e2580; it is not
 * defined here.
 * The authored SetIopFunction__FUii refers to three of these under the placeholder names
 * norrath_overlay_sSend_11edb00, norrath_overlay_sFunctionIndex_11edf00 and
 * norrath_overlay_sIopFunctionData_11edf08; they are aliases of the same storage so that both
 * spellings resolve to one object. The relative placement of the five objects in memory is not
 * reproduced (the linker lays out uninitialised data), and no load address is claimed.
 * Inferred: sSend is the argument block of the batch call 0x80f0 that FlushIopCommand__Fb sends
 * and sRecv its reply buffer. Unknown: who reads sRecv and what the reply holds.
 */
extern "C" {
alignas(64) int sSend[128];
alignas(64) int sRecv[128];
alignas(8) int sFunctionIndex[2];
alignas(8) int sIopFunctionData[128];
alignas(8) unsigned char sSifClient[56];

extern int norrath_overlay_sSend_11edb00[128] __attribute__((alias("sSend")));
extern int norrath_overlay_sFunctionIndex_11edf00[2] __attribute__((alias("sFunctionIndex")));
extern int norrath_overlay_sIopFunctionData_11edf08[128] __attribute__((alias("sIopFunctionData")));
}
