// Observed: Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType at 0x0105cf48, 188 bytes. With (this, format, a, b, c, rateA,
// rateB, playback) it stores them at this+8, +12, +16, +20, +24 (float), +28 (float) and +32. It then calls CalcDataSize__13VIHSpriteAnim
// (a negative result returns -1) and Allocate__13VIHSpriteAnim (a negative result returns -1). With the block base read from this+52 it
// adds the base to the words at +36, +40 and +44 and stores them back. Finally, for i from 0 while i < the word at +12 (re-read each
// pass), it writes the running value i * (the word at +16, re-read) into the second word of the i-th 8-byte record starting at the new
// +36 value. It returns zero.
// Inferred: +36, +40 and +44 start as offsets into one allocated block (the sibling setters index +36 and +44 in 8-byte steps and +40 in
// 16- or 32-byte steps by format), +12 is the node count and +16 the frames per node, so the second record word is each node's first
// frame index (SetNodeFrame adds it to the frame number). The names are guesses from use, not from symbols.
// Unknown: the meaning of +20, +24 and +28 beyond "stored", and the exact types of the format and playback arguments (enumerations).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CalcDataSize__13VIHSpriteAnim(void* object);
int Allocate__13VIHSpriteAnim(void* object);

int Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(void* object, int format, int nodeCount, int framesPerNode, int extra,
                                                                       float rateA, float rateB, int playback) {
    unsigned* words = static_cast<unsigned*>(object);
    words[2] = format;
    words[3] = nodeCount;
    words[4] = framesPerNode;
    words[5] = extra;
    *reinterpret_cast<float*>(words + 6) = rateA;
    *reinterpret_cast<float*>(words + 7) = rateB;
    words[8] = playback;
    if (CalcDataSize__13VIHSpriteAnim(object) < 0) return -1;
    if (Allocate__13VIHSpriteAnim(object) < 0) return -1;
    unsigned base = words[13];
    unsigned records = base + words[9];
    words[10] = base + words[10];
    words[11] = base + words[11];
    words[9] = records;
    unsigned char* record = NORRATH_WORD_TO_POINTER(records);
    int offset = 0;
    for (int i = 0; i < static_cast<int>(words[3]); ++i) {
        *reinterpret_cast<int*>(record + 4) = offset;
        record += 8;
        offset += words[4];
    }
    return 0;
}
}
