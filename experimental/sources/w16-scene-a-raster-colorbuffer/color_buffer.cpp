// Observed: ColorBuffer__8VIRasteri at 0x011136e0 is 28 bytes in the pinned SLUS-20565 image. It returns the first word of the 12-byte slot
// a1 in the table whose pointer is the word at offset 19364 (0x4ba4) of the raster. Nothing is checked or modified.
// Inferred: the table holds per-buffer records and the word is a pointer or handle to the colour buffer (from the name only).
// Unknown: the record's other 8 bytes, its type, and the table's length. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
unsigned ColorBuffer__8VIRasteri(unsigned char* self, int index) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 0x4ba4));
    return *reinterpret_cast<unsigned*>(table + index * 12);
}
}
