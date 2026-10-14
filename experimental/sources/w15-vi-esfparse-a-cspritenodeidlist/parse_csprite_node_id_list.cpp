// Observed: ParseCSpriteNodeIDList__10VIESFParseR9VICSprite at 0x01174688 is 268 bytes. Arguments:
// parse (a0; object file word at +36) and sprite (a1, a character sprite).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative, or a tag other than 0x2915,
//      returns -1.
//   2. Reads a signed count (Read__9VIObjFileRi). When the count is positive, for each entry two
//      signed values a and b are read (Read__9VIObjFileRi, a first). The slot is a when the
//      second ReadBegin output is non-zero and 3 when it is zero; b is stored into the word at
//      sprite + 3860 + 4 * slot.
//   3. Returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: a list of node identifiers by slot (older files, whose second ReadBegin output is
// zero, set only slot 3). Unknown: the meaning of the second ReadBegin output beyond that
// test, the slot count (the slot is not range checked), and why a is still read when unused.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);

int ParseCSpriteNodeIDList__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2915) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        int first;
        int value;
        Read__9VIObjFileRi(file, &first);
        int slot = first;
        Read__9VIObjFileRi(file, &value);
        if (second == 0) slot = 3;
        *reinterpret_cast<int*>(sprite + 4 + 3856 + 4 * slot) = value;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
