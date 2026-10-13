// Observed: Surface__8VIRasteri at 0x011137c8 is 28 bytes in the pinned SLUS-20565 image. It reads the table pointer at offset
// 19388 (0x4bbc) of the raster object and returns the first word of the 12-byte entry at index a1 (entry address = table + 12*index).
// No range check is made.
// Inferred: the table is an array of 12-byte surface slots whose first word is a surface pointer, and the return is that pointer.
// Unknown: the entry's remaining words (ShareSurface increments the word at +4 of the pointed-to surface), the table size, and
// whether the index may be out of range. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
unsigned Surface__8VIRasteri(unsigned char* self, int index) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 0x4bbc));
    return *reinterpret_cast<unsigned*>(table + index * 12);
}
}
