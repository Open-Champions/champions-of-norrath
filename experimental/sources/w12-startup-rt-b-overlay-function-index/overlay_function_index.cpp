// Observed sFunctionIndex, a file-local symbol of the .srs overlay data at 0x011edf00 (name taken from
// the executable's symbol table; the authored name carries the address because other local symbols
// share names). The next symbol, sIopFunctionData, starts at 0x011edf08, so the slot is eight bytes;
// only its first word is used by the one reader, SetIopFunction__FUii (w11-game-a-set-iop-function),
// which treats it as the index of the next free word in the sSend buffer and advances it. The bytes in
// the file image at that address are zero, so the initial value is 0.
// This unit supplies the first word only, aligned to eight bytes; the four bytes after it are not
// owned here and nothing is known about them. Its position in the .srs layout is not authored.
// Nothing was executed on the EE.
extern "C" {
alignas(8) int norrath_overlay_sFunctionIndex_11edf00 = 0;
}
