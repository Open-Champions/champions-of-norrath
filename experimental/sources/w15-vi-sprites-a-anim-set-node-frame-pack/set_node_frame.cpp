// Observed: SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame at 0x0105d180, 136 bytes. Arguments (this, node, frame, source). When the
// word at this+8 is not one it returns -1 and writes nothing. Otherwise it reads the second word of the 8-byte record node*8 inside the
// array at this+36, adds the frame number, and uses the sum as an index into the array at this+40 in 16-byte steps. It copies eight
// unsigned 16-bit values from source (offsets 0, 2, 4, 6, 8, 10, 12, 14) to the same offsets of that element. It returns zero. There are
// no bounds checks.
// Inferred: format one is a packed frame of eight halfwords; format zero (full frames of 32 bytes) is handled by the other overload.
// Unknown: the field meanings of the packed record.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame(void* object, int node, int frame, const void* source) {
    const unsigned* words = static_cast<const unsigned*>(object);
    if (words[2] != 1) return -1;
    const unsigned char* records = NORRATH_WORD_TO_POINTER(words[9]);
    unsigned char* frames = NORRATH_WORD_TO_POINTER(words[10]);
    int index = frame + *reinterpret_cast<const int*>(records + node * 8 + 4);
    unsigned short* destination = reinterpret_cast<unsigned short*>(frames + index * 16);
    const unsigned short* from = static_cast<const unsigned short*>(source);
    for (int i = 0; i < 8; ++i) destination[i] = from[i];
    return 0;
}
}
