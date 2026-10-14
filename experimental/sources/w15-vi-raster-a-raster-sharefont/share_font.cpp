// Observed: ShareFont__8VIRasteri at 0x01113c60 is 44 bytes in the pinned SLUS-20565 image. It finds the 12-byte slot at index a1 in the table pointed to by
// the word at offset 19516 of the raster object, loads the pointer in the slot's first word, adds one to the word at offset 4 of the
// pointed-to object, and returns zero. No range or null check is made.
// Inferred: the word at +4 is a reference count of a font and the zero is an int status.
// Unknown: the object layout beyond that word, and what releases the count. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int ShareFont__8VIRasteri(unsigned char* self, int index) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19516));
    unsigned char* object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    *reinterpret_cast<int*>(object + 4) += 1;
    return 0;
}
}
