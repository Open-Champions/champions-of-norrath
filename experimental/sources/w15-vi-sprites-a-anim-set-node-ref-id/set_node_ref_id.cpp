// Observed: SetNodeRefID__13VIHSpriteAnimii at 0x0105d0e8 is 24 bytes. It loads the word at this+36, adds index*8 to it and stores the
// third argument at that address (the first word of an 8-byte record). It calls nothing and returns zero. No bounds check.
// Inferred: this+36 is an array of 8-byte per-node records whose first word is a reference id; the name comes from the symbol.
// Unknown: the second word of each record (SetKeyframe stores into a different array at +44), the array length, and the meaning of the id.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int SetNodeRefID__13VIHSpriteAnimii(void* object, int index, int refId) {
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + 36));
    *reinterpret_cast<int*>(records + index * 8) = refId;
    return 0;
}
}
