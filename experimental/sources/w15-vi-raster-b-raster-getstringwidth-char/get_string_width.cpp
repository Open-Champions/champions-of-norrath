// Observed: GetStringWidth__8VIRasteriPCc at 0x01113dd0 is 52 bytes in the pinned SLUS-20565 image. It finds the 12-byte slot at index
// a1 in the table pointed to by the word at offset 19516 (0x4c3c) of the raster object, loads the pointer in the slot's first word, and
// tail-calls GetStringWidth__7VIRFontPCci(font, string, 1), returning that result unchanged.
// Inferred: the table holds fonts, the string is an 8-bit character string, and the constant 1 is a flag whose meaning is not known.
// Unknown: what the flag selects and the result unit. The callee is an unresolved import. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int GetStringWidth__7VIRFontPCci(unsigned char* font, const char* text, int flag);
int GetStringWidth__8VIRasteriPCc(unsigned char* self, int index, const char* text) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 0x4c3c));
    unsigned char* font = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    return GetStringWidth__7VIRFontPCci(font, text, 1);
}
}
