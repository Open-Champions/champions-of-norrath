// Observed: ShareAnimation__7VIScenei at 0x01144e20, 44 bytes, in the pinned SLUS-20565 image. Arguments: the scene (X) and an index (I). Word X+1028 is a pointer to an array
// of 12-byte records. The first word of record I is a pointer to an object; the 32-bit word at offset 4 of that object is incremented by one (read, add, write back,
// no overflow or null test). The function returns 0.
// Inferred: the record array holds animation references and the incremented word is a share (reference) count; this is a name-based reading, the count's
// meaning is not otherwise confirmed. Unknown: what the other eight bytes of a record hold, and the index range.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ShareAnimation__7VIScenei(unsigned char* self, int index) {
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 1028));
    unsigned char* object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + index * 12));
    *reinterpret_cast<int*>(object + 4) = *reinterpret_cast<int*>(object + 4) + 1;
    return 0;
}
}
