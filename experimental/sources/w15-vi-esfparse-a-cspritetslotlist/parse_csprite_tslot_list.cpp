// Observed: ParseCSpriteTSlotList__10VIESFParseR9VICSprite at 0x01174898 is 308 bytes. Arguments:
// parse (a0; object file word at +36) and sprite (a1, a character sprite).
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2930, returns -1.
//   2. Reads a signed count (Read__9VIObjFileRi). When the count is positive, for each entry three
//      signed values are read: a slot index, b and c. b is stored into the word at
//      sprite + 3900 + 24 * slot and c into the word at sprite + 3904 + 24 * slot.
//   3. Returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: "TSlot" is read from the handler name (a text slot list); entries are 24 bytes and the
// two values land in adjacent words. Unknown: the other four words of an entry, the slot count
// (the index is not range checked), and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);

int ParseCSpriteTSlotList__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2930) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        int slot;
        int first;
        int second_value;
        Read__9VIObjFileRi(file, &slot);
        Read__9VIObjFileRi(file, &first);
        Read__9VIObjFileRi(file, &second_value);
        *reinterpret_cast<int*>(sprite + 12 + 3888 + 24 * slot) = first;
        *reinterpret_cast<int*>(sprite + 16 + 3888 + 24 * slot) = second_value;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
