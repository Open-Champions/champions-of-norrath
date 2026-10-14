// Observed: MC_BuildPackedPlayer__FP11playerEntryi at 0x00251058 is 96 bytes. The entries are
// 41 bytes apart; entry is the address of the first and n picks the one used, e = entry + 41 * n.
// It calls playerInit__FiiPcScScScScT2(n, signed byte e[32], e, signed bytes e[33], e[34], e[36],
// e[35], e + 37), where the third argument is the name text at the start of the entry and the
// last is the text at byte 37. It then calls convertPointersToIndicies__Fi(n). Returns nothing.
// Inferred: bytes 32 to 36 are the five choices that playerInit takes (the call passes byte 35
// before byte 36's position, i.e. in the order 33, 34, 36, 35); the entry is the packed record
// of a player, from the function name.
// Unknown: what each of bytes 32 to 36 selects, and the type of the text at byte 37.

extern "C" void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char first_byte,
                                            signed char second_byte, signed char third_byte,
                                            signed char fourth_byte, char* tint);
extern "C" void convertPointersToIndicies__Fi(int index);

extern "C" void MC_BuildPackedPlayer__FP11playerEntryi(void* entry, int n) {
    char* e = static_cast<char*>(entry) + n * 41;
    playerInit__FiiPcScScScScT2(n, static_cast<signed char>(e[32]), e, static_cast<signed char>(e[33]),
                                static_cast<signed char>(e[34]), static_cast<signed char>(e[36]),
                                static_cast<signed char>(e[35]), e + 37);
    convertPointersToIndicies__Fi(n);
}
