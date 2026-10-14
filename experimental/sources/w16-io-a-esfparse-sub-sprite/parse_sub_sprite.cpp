// Observed: ParseSubSprite__10VIESFParse at 0x01179b88, 220 bytes. Argument: parse (a0: the ESF parser record, with a progress
// callback word at +4, its argument word at +8 and the object file word at +36). In order it:
//   1. calls the callback word with the argument word when it is non-zero;
//   2. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1;
//   3. dispatches on the tag: 0x2310 calls ParseSimpleSubSpriteObj__10VIESFParse(parse), 0x2320 calls
//      ParseSkinSubSpriteObj__10VIESFParse(parse) and 0x2a40 calls ParseSkinLODSpriteObj__10VIESFParse(parse); the result is
//      returned when it is not negative, otherwise -1;
//   4. any other tag calls ReadEnd__9VIObjFile(file) and returns -1 whatever that call returns.
// Inferred: the tags select the three sub-sprite record kinds the sprite-array containers hold; the names follow the handlers.
// Unknown: the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ParseSimpleSubSpriteObj__10VIESFParse(void* parse);
int ParseSkinSubSpriteObj__10VIESFParse(void* parse);
int ParseSkinLODSpriteObj__10VIESFParse(void* parse);

int ParseSubSprite__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    unsigned callback = words[1];
    if (callback != 0) reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(callback))(NORRATH_WORD_ADDRESS(words[2]));
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    int result;
    if (tag == 0x2310) {
        result = ParseSimpleSubSpriteObj__10VIESFParse(parse);
    } else if (tag == 0x2320) {
        result = ParseSkinSubSpriteObj__10VIESFParse(parse);
    } else if (tag == 0x2a40) {
        result = ParseSkinLODSpriteObj__10VIESFParse(parse);
    } else {
        ReadEnd__9VIObjFile(file);
        return -1;
    }
    return result < 0 ? -1 : result;
}
}
