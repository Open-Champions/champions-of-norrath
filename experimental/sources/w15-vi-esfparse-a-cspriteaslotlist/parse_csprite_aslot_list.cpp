// Observed: ParseCSpriteASlotList__10VIESFParseR9VICSprite at 0x01174798 is 256 bytes. Arguments:
// parse (a0; object file word at +36) and sprite (a1, a character sprite).
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2920, returns -1.
//   2. Reads a signed count (Read__9VIObjFileRi). When the count is positive, for each entry two
//      signed values are read: a slot index and a value; the value is stored into the word at
//      sprite + 4364 + 12 * slot (slot entries are 12 bytes apart; only the first word is
//      written).
//   3. Returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: "ASlot" is read from the handler name (an animation slot list); the written word is
// the first of three. Unknown: the slot count (the index is not range checked), the other two
// words of an entry, and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);

int ParseCSpriteASlotList__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2920) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        int slot;
        int value;
        Read__9VIObjFileRi(file, &slot);
        Read__9VIObjFileRi(file, &value);
        *reinterpret_cast<int*>(sprite + 12 + 4352 + 12 * slot) = value;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
