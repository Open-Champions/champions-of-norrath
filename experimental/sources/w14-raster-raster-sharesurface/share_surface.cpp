// Observed: ShareSurface__8VIRasteri at 0x011137e8 is 44 bytes in the pinned SLUS-20565 image. It finds the 12-byte slot at index a1
// in the table pointed to by the word at offset 19388 (0x4bbc) of the raster object, loads the surface pointer in the slot's first
// word, adds one to the word at offset 4 of that surface, and returns zero. No range or null check is made.
// Inferred: the word at surface+4 is a reference count and the zero is an int status.
// Unknown: the surface layout beyond that word, and what releases the count. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int ShareSurface__8VIRasteri(unsigned char* self, int index) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 0x4bbc));
    unsigned char* surface = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    *reinterpret_cast<int*>(surface + 4) += 1;
    return 0;
}
}
