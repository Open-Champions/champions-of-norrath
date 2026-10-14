// Observed: ParseSprite__10VIESFParse at 0x0116e320, 484 bytes. Argument: parse (a0: the ESF parser record, object file word at
// +36). ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result returns -1. The tag then selects one handler, called
// with the parse record:
//   0x2000 ParseSimpleSpriteObj    0x2200 ParseHSpriteObj        0x2700 ParseCSpriteObj          0x2a00 ParseLODSpriteObj
//   0x2b00 ParsePointLightObj      0x2c00 ParseGroupSpriteObj    0x2d00 ParsePointSpriteObj      0x2e00 ParseStreamAudioSpriteObj
//   0x2f00 ParseFloraSpriteObj     0xb100 ParseSoundSpriteObj    0xc100 ParseParticleSpriteObj   0xc300 ParseEffectVolumeSpriteObj
// (all in the __10VIESFParse class). The handler's result is returned when it is not negative, otherwise -1. Any other tag
// calls ReadEnd__9VIObjFile(file) and returns -1 whatever that call returns.
// Inferred: the dispatcher over the top-level sprite record kinds; the tag-to-kind pairing is the handler names.
// Unknown: the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ParseSimpleSpriteObj__10VIESFParse(void* parse);
int ParseHSpriteObj__10VIESFParse(void* parse);
int ParseCSpriteObj__10VIESFParse(void* parse);
int ParseLODSpriteObj__10VIESFParse(void* parse);
int ParsePointLightObj__10VIESFParse(void* parse);
int ParseGroupSpriteObj__10VIESFParse(void* parse);
int ParsePointSpriteObj__10VIESFParse(void* parse);
int ParseStreamAudioSpriteObj__10VIESFParse(void* parse);
int ParseFloraSpriteObj__10VIESFParse(void* parse);
int ParseSoundSpriteObj__10VIESFParse(void* parse);
int ParseParticleSpriteObj__10VIESFParse(void* parse);
int ParseEffectVolumeSpriteObj__10VIESFParse(void* parse);

int ParseSprite__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    int result;
    switch (tag) {
    case 0x2000: result = ParseSimpleSpriteObj__10VIESFParse(parse); break;
    case 0x2200: result = ParseHSpriteObj__10VIESFParse(parse); break;
    case 0x2700: result = ParseCSpriteObj__10VIESFParse(parse); break;
    case 0x2a00: result = ParseLODSpriteObj__10VIESFParse(parse); break;
    case 0x2b00: result = ParsePointLightObj__10VIESFParse(parse); break;
    case 0x2c00: result = ParseGroupSpriteObj__10VIESFParse(parse); break;
    case 0x2d00: result = ParsePointSpriteObj__10VIESFParse(parse); break;
    case 0x2e00: result = ParseStreamAudioSpriteObj__10VIESFParse(parse); break;
    case 0x2f00: result = ParseFloraSpriteObj__10VIESFParse(parse); break;
    case 0xb100: result = ParseSoundSpriteObj__10VIESFParse(parse); break;
    case 0xc100: result = ParseParticleSpriteObj__10VIESFParse(parse); break;
    case 0xc300: result = ParseEffectVolumeSpriteObj__10VIESFParse(parse); break;
    default:
        ReadEnd__9VIObjFile(file);
        return -1;
    }
    return result < 0 ? -1 : result;
}
}
