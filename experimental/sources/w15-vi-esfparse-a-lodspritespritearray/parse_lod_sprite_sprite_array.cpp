// Observed: ParseLODSpriteSpriteArray__10VIESFParse at 0x01177570 is 244 bytes. Argument: parse (a0;
// object file word at +36).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result or a tag other than
//      0x2a20 returns -1.
//   2. n = NumSubObjects__9VIObjFile(file). When the second ReadBegin output is zero it calls
//      ParseSubSprite__10VIESFParse(parse) n times, otherwise ParseSprite__10VIESFParse(parse)
//      n times (no calls for n <= 0); a negative result returns -1 at once.
//   3. Returns -1 when ReadEnd__9VIObjFile(file) is negative, else 0.
// Inferred: a container of the sprites for a LOD sprite; the second output selects sub-sprites
// versus full sprites. Unknown: the meaning of the second ReadBegin output beyond that test.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int NumSubObjects__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int ParseSubSprite__10VIESFParse(void* parse);
int ParseSprite__10VIESFParse(void* parse);

int ParseLODSpriteSpriteArray__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2a20) return -1;
    if (second == 0) {
        int count = NumSubObjects__9VIObjFile(file);
        for (int i = 0; i < count; ++i) {
            if (ParseSubSprite__10VIESFParse(parse) < 0) return -1;
        }
    } else {
        int count = NumSubObjects__9VIObjFile(file);
        for (int i = 0; i < count; ++i) {
            if (ParseSprite__10VIESFParse(parse) < 0) return -1;
        }
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
