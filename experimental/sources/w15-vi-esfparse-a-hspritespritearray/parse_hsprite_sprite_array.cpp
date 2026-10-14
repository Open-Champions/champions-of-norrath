// Observed: ParseHSpriteSpriteArray__10VIESFParse at 0x0117ad18 is 172 bytes. Argument: parse (a0; object file word at +36).
// ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative or a tag other than 0x2220 returns -1.
// n = NumSubObjects__9VIObjFile(file); when n is positive it calls ParseSubSprite__10VIESFParse(parse)
// n times, returning -1 at the first negative result. Then it returns -1 when ReadEnd__9VIObjFile(file)
// is negative, else 0.
// Inferred: a container holding sub-sprites of a hierarchical sprite.
// Unknown: the second ReadBegin output is not used.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int NumSubObjects__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int ParseSubSprite__10VIESFParse(void* parse);

int ParseHSpriteSpriteArray__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2220) return -1;
    int count = NumSubObjects__9VIObjFile(file);
    for (int i = 0; i < count; ++i) {
        if (ParseSubSprite__10VIESFParse(parse) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
