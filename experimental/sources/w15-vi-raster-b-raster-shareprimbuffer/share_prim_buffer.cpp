// Observed: SharePrimBuffer__8VIRasteri at 0x01113390 is 44 bytes in the pinned SLUS-20565 image. It finds the 12-byte slot at index a1
// in the table pointed to by the word at offset 19328 (0x4b80) of the raster object, loads the pointer in the slot's first word, adds
// one to the word at offset 4 of the pointee, and returns zero. No range or null check is made.
// Inferred: the pointee is a primitive buffer whose word at +4 is a reference count (the same pattern as ShareSurface), and the zero is
// an int status.
// Unknown: the primbuffer layout beyond that word, and what releases the count. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int SharePrimBuffer__8VIRasteri(unsigned char* self, int index) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 0x4b80));
    unsigned char* buffer = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    *reinterpret_cast<int*>(buffer + 4) += 1;
    return 0;
}
}
