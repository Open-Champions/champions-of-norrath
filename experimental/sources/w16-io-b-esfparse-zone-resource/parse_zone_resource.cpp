// Observed: ParseZoneResource__10VIESFParse at 0x01174d88 is 880 bytes and returns an int.
// Argument: parse (callback word at +4, callback argument word at +8, a flag word at +12,
// object file word at +36). If the word at +4 is nonzero it is called as a function with
// the word at +8 as its only argument. Then ReadBegin__9VIObjFileRUsT1(file, &tag,
// &second): a negative result returns -1. The tag selects a parse call, each given parse:
//   0x2000 ParseSimpleSpriteObj    0x2200 ParseHSpriteObj      0x2310 ParseSimpleSubSpriteObj
//   0x2700 ParseCSpriteObj         0x2a00 ParseLODSpriteObj    0x2b00 ParsePointLightObj
//   0x2c00 ParseGroupSpriteObj     0xb100 ParseSoundSpriteObj  0xc100 ParseParticleSpriteObj
//     (all of the __10VIESFParse class): a negative result returns -1, otherwise the
//     result itself is returned.
//   0x2d00 ParsePointSpriteObj     0x2e00 ParseStreamAudioSpriteObj
//   0x2f00 ParseFloraSpriteObj     0xc300 ParseEffectVolumeSpriteObj
//   0x6000 ParseActorObj           0x3200 ParseZoneBaseObj
//   0x3270 ParseZoneRoomActorsObj  0x3290 ParseZoneRoomStaticLightingsObj:
//     a negative result returns -1, otherwise 0.
//   0x1110 ParseMaterialPalObj, only when the flag word at +12 is nonzero; result as the
//     first group.
//   0x1001 (only when the flag word at +12 is nonzero): for each index below
//     NumSubObjects__9VIObjFile(file) (called before every test) a ReadBegin must give a
//     tag of 0x1000 and ParseSurfaceObj__10VIESFParse(parse) must not be negative, else -1;
//     then ReadEnd__9VIObjFile(file): negative returns -1, otherwise 0.
//   Any other tag, or 0x1110/0x1001 with the flag word zero: ReadEnd__9VIObjFile(file),
//     -1 if negative, otherwise 0. That skips the block.
// Inferred: the callback is a progress hook run once per resource; the tags 0x1001 and
// 0x1110 are the surface list and material palette blocks of a zone's resource directory.
// Unknown: the names of the three parse fields, and what the flag word at +12 means.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
// The stored callback is called with one word argument; a host check redefines this too.
#ifndef NORRATH_CALL_WORD
#define NORRATH_CALL_WORD(function_word, argument) \
    reinterpret_cast<void (*)(unsigned)>(static_cast<__UINTPTR_TYPE__>(function_word))(argument)
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int ParseSurfaceObj__10VIESFParse(void* parse);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParseSimpleSpriteObj__10VIESFParse(void* parse);
int ParseHSpriteObj__10VIESFParse(void* parse);
int ParseSimpleSubSpriteObj__10VIESFParse(void* parse);
int ParseCSpriteObj__10VIESFParse(void* parse);
int ParseLODSpriteObj__10VIESFParse(void* parse);
int ParsePointLightObj__10VIESFParse(void* parse);
int ParseGroupSpriteObj__10VIESFParse(void* parse);
int ParseSoundSpriteObj__10VIESFParse(void* parse);
int ParseParticleSpriteObj__10VIESFParse(void* parse);
int ParsePointSpriteObj__10VIESFParse(void* parse);
int ParseStreamAudioSpriteObj__10VIESFParse(void* parse);
int ParseFloraSpriteObj__10VIESFParse(void* parse);
int ParseEffectVolumeSpriteObj__10VIESFParse(void* parse);
int ParseActorObj__10VIESFParse(void* parse);
int ParseZoneBaseObj__10VIESFParse(void* parse);
int ParseZoneRoomActorsObj__10VIESFParse(void* parse);
int ParseZoneRoomStaticLightingsObj__10VIESFParse(void* parse);

int ParseZoneResource__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    if (words[1] != 0) NORRATH_CALL_WORD(words[1], words[2]);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    int result;
    switch (tag) {
    case 0x2000: result = ParseSimpleSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2200: result = ParseHSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2310: result = ParseSimpleSubSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2700: result = ParseCSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2a00: result = ParseLODSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2b00: result = ParsePointLightObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2c00: result = ParseGroupSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0xb100: result = ParseSoundSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0xc100: result = ParseParticleSpriteObj__10VIESFParse(parse); return result >= 0 ? result : -1;
    case 0x2d00: return ParsePointSpriteObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x2e00: return ParseStreamAudioSpriteObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x2f00: return ParseFloraSpriteObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0xc300: return ParseEffectVolumeSpriteObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x6000: return ParseActorObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x3200: return ParseZoneBaseObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x3270: return ParseZoneRoomActorsObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x3290: return ParseZoneRoomStaticLightingsObj__10VIESFParse(parse) < 0 ? -1 : 0;
    case 0x1110:
        if (words[3] != 0) {
            result = ParseMaterialPalObj__10VIESFParse(parse);
            return result >= 0 ? result : -1;
        }
        break;
    case 0x1001:
        if (words[3] != 0) {
            for (int i = 0; i < NumSubObjects__9VIObjFile(file); ++i) {
                unsigned short itemTag;
                unsigned short itemSecond;
                if (ReadBegin__9VIObjFileRUsT1(file, &itemTag, &itemSecond) < 0) return -1;
                if (itemTag != 0x1000) return -1;
                if (ParseSurfaceObj__10VIESFParse(parse) < 0) return -1;
            }
            return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
        }
        break;
    default:
        break;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
