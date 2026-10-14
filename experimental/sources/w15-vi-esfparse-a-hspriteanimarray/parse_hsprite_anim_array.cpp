// Observed: ParseHSpriteAnimArray__10VIESFParse at 0x01172e58 is 264 bytes. Argument: parse (a0; object
// file word at +36).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result or a tag other than
//      0x2610 returns -1.
//   2. n = NumSubObjects__9VIObjFile(file). For each of n entries (none for n <= 0) it calls
//      ReadBegin__9VIObjFileRUsT1(file, &tag2, &second2) again; a negative result or a tag other
//      than 0x2600 returns -1, otherwise ParseHSpriteAnimObj__10VIESFParse(parse) is called and a
//      negative result returns -1.
//   3. Returns -1 when ReadEnd__9VIObjFile(file) is negative, else 0.
// Inferred: a container of hierarchical sprite animations (0x2600 is the animation tag).
// Unknown: the second ReadBegin outputs are not used.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int NumSubObjects__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int ParseHSpriteAnimObj__10VIESFParse(void* parse);

int ParseHSpriteAnimArray__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2610) return -1;
    int count = NumSubObjects__9VIObjFile(file);
    for (int i = 0; i < count; ++i) {
        unsigned short entry_tag;
        unsigned short entry_second;
        int result = -1;
        if (ReadBegin__9VIObjFileRUsT1(file, &entry_tag, &entry_second) >= 0 && entry_tag == 0x2600) {
            result = ParseHSpriteAnimObj__10VIESFParse(parse);
        }
        if (result < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
