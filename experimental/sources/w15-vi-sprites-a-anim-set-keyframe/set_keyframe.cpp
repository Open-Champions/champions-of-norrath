// Observed: SetKeyframe__13VIHSpriteAnimiif at 0x0105d208 is 28 bytes. It loads the word at this+44, adds index*8, stores the integer
// argument at +4 of that record and the float argument at +0, and returns zero. It calls nothing; there is no bounds check.
// Inferred: this+44 is an array of 8-byte keyframe records (float at +0, integer at +4); only the offsets and access widths are evidence.
// Unknown: what the float and the integer mean, and the array length.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int SetKeyframe__13VIHSpriteAnimiif(void* object, int index, int value, float time) {
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + 44));
    unsigned char* record = records + index * 8;
    *reinterpret_cast<int*>(record + 4) = value;
    *reinterpret_cast<float*>(record) = time;
    return 0;
}
}
