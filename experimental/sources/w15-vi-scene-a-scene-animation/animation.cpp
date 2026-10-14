// Observed: Animation__7VIScenei at 0x01144e00, 28 bytes. Arguments: the scene (S) and an index (I). Word S+1028 is a pointer to an array of
// 12-byte records; the function returns the first 32-bit word of record I. The index is not range checked and nothing is written.
// Inferred: S+1020 is the animation pool (CreateAnimation__7VIScenei and ReleaseAnimation__7VIScenei add to and erase from it) and its
// record array pointer sits 8 bytes in; each record's first word is the pointer to a VIHSpriteAnim.
// Unknown: the meaning of the other eight bytes of a record.
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
unsigned Animation__7VIScenei(unsigned char* self, int index) {
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 1028));
    return *reinterpret_cast<unsigned*>(records + index * 12);
}
}
