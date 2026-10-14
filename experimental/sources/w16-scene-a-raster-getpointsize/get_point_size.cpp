// Observed: GetPointSize__8VIRasteri at 0x01113e40 is 32 bytes in the pinned SLUS-20565 image. It indexes the 12-byte slot a1 in the table
// whose pointer is the word at offset 19516 (0x4c3c) of the raster, loads the pointer in the slot's first word, and returns the word at
// offset 20 of what that points to.
// Inferred: the table holds fonts (GetStringWidth__8VIRasteriPCc reads the same table and calls a font routine), so the result is a
// font field; `Init__7VIRFont...` stores its `extra` argument at +20, the same field.
// Unknown: the unit and meaning of that field (the function name says point size). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int GetPointSize__8VIRasteri(unsigned char* self, int index) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 0x4c3c));
    unsigned char* font = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    return *reinterpret_cast<int*>(font + 20);
}
}
