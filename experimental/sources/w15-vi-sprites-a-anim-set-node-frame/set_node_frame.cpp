// Observed: SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame at 0x0105d100, 128 bytes. Arguments (this, node, frame, source). When the
// word at this+8 is not zero it returns -1 and writes nothing. Otherwise it reads the second word of the 8-byte record node*8 inside the
// array at this+36, adds the frame number, and uses the sum as an index into the array at this+40 in 32-byte steps. It copies 32 bytes
// from source to that element: the first 16 and the word at +16 as one run, then three floats from +20, +24 and +28. It returns zero.
// There are no bounds checks.
// Inferred: format zero is a full-precision frame of 32 bytes (the other overload handles format one with 16-byte packed frames); the
// second record word is the node's first frame index, filled by Init.
// Unknown: the field meanings of the frame record.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame(void* object, int node, int frame, const void* source) {
    const unsigned* words = static_cast<const unsigned*>(object);
    if (words[2] != 0) return -1;
    const unsigned char* records = NORRATH_WORD_TO_POINTER(words[9]);
    unsigned char* frames = NORRATH_WORD_TO_POINTER(words[10]);
    int index = frame + *reinterpret_cast<const int*>(records + node * 8 + 4);
    unsigned char* destination = frames + index * 32;
    const unsigned char* from = static_cast<const unsigned char*>(source);
    for (int i = 0; i < 20; ++i) destination[i] = from[i];
    *reinterpret_cast<float*>(destination + 20) = *reinterpret_cast<const float*>(from + 20);
    *reinterpret_cast<float*>(destination + 24) = *reinterpret_cast<const float*>(from + 24);
    *reinterpret_cast<float*>(destination + 28) = *reinterpret_cast<const float*>(from + 28);
    return 0;
}
}
