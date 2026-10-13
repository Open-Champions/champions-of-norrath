// Observed: ParseObject__10VIESFParse at 0x0116df88 is 920 bytes. Argument: parse (a0). Fields: a
// callback function word at +4 and its argument word at +8, and the object file word at +36.
//   1. If the word at +4 is non-zero it calls it with the word at +8 as the only argument.
//   2. r = ReadBegin__9VIObjFileRUsT1(file, &tag, &second) with two adjacent unsigned shorts; a
//      negative r returns -1.
//   3. The 16-bit tag selects one handler, which is called with parse and whose int result is the
//      result r2 (the handlers read and close their own object): 0x1000 ParseSurfaceObj, 0x1001
//      ParseSurfaceArrayObj, 0x1110 ParseMaterialPalObj, 0x2000 ParseSimpleSpriteObj, 0x2200
//      ParseHSpriteObj, 0x2600 ParseHSpriteAnimObj, 0x2700 ParseCSpriteObj, 0x2a00
//      ParseLODSpriteObj, 0x2b00 ParsePointLightObj, 0x2c00 ParseGroupSpriteObj, 0x2d00
//      ParsePointSpriteObj, 0x2e00 ParseStreamAudioSpriteObj, 0x2f00 ParseFloraSpriteObj, 0x6000
//      ParseActorObj, 0x7000 ParseFontObj, 0xb000 ParseAdpcmObj, 0xb030 ParseXmObj, 0xb100
//      ParseSoundSpriteObj, 0xc000 ParseParticleDefinitionObj, 0xc100 ParseParticleSpriteObj,
//      0xc200 ParseSpellEffectObj, 0xc300 ParseEffectVolumeSpriteObj (all __10VIESFParse methods
//      taking only parse); 0x8000 ParseWorldObj__10VIESFParsei(parse, 0); 0xa000 constructs a
//      local VIArray<VIResourceElem> (__t7VIArray1Z14VIResourceElem), calls
//      ParseResourceDirObj__10VIESFParseiR13VIResourceDir(parse, 0, &array), destroys the array with
//      _$_t7VIArray1Z14VIResourceElem(&array, 2) and returns -1 when that parse result was
//      negative, else 0 at once (without the final step below). Any other tag (including 0x1111
//      and every value not listed) calls ReadEnd__9VIObjFile(file) and uses its result as r2.
//   4. It returns -1 when r2 is negative, else 0.
// Inferred: the top-level dispatcher of the ESF resource format: one object is opened, parsed by
// the handler for its tag and closed; unknown tags are skipped by ReadEnd; the callback at +4
// is a per-object progress hook. The local array is at most 32 bytes (frame size).
// Unknown: the meaning of the second ReadBegin output (written but not read here), the callback
// contract, the exact size of VIArray<VIResourceElem>, and which handlers close their object.
extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ParseSurfaceObj__10VIESFParse(void* parse);
int ParseSurfaceArrayObj__10VIESFParse(void* parse);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParseSimpleSpriteObj__10VIESFParse(void* parse);
int ParseHSpriteObj__10VIESFParse(void* parse);
int ParseHSpriteAnimObj__10VIESFParse(void* parse);
int ParseCSpriteObj__10VIESFParse(void* parse);
int ParseLODSpriteObj__10VIESFParse(void* parse);
int ParsePointLightObj__10VIESFParse(void* parse);
int ParseGroupSpriteObj__10VIESFParse(void* parse);
int ParsePointSpriteObj__10VIESFParse(void* parse);
int ParseStreamAudioSpriteObj__10VIESFParse(void* parse);
int ParseFloraSpriteObj__10VIESFParse(void* parse);
int ParseActorObj__10VIESFParse(void* parse);
int ParseFontObj__10VIESFParse(void* parse);
int ParseAdpcmObj__10VIESFParse(void* parse);
int ParseXmObj__10VIESFParse(void* parse);
int ParseSoundSpriteObj__10VIESFParse(void* parse);
int ParseParticleDefinitionObj__10VIESFParse(void* parse);
int ParseParticleSpriteObj__10VIESFParse(void* parse);
int ParseSpellEffectObj__10VIESFParse(void* parse);
int ParseEffectVolumeSpriteObj__10VIESFParse(void* parse);
int ParseWorldObj__10VIESFParsei(void* parse, int flag);
int ParseResourceDirObj__10VIESFParseiR13VIResourceDir(void* parse, int flag, void* array);
void* __t7VIArray1Z14VIResourceElem(void* array);
void _$_t7VIArray1Z14VIResourceElem(void* array, int flags);
}

// Words that hold 32-bit target addresses: a host check redefines these macros.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
typedef void (*ProgressCallback)(void* argument);

extern "C" {
int ParseObject__10VIESFParse(unsigned char* parse) {
    unsigned callback = *reinterpret_cast<unsigned*>(parse + 4);
    if (callback != 0) {
        reinterpret_cast<ProgressCallback>(NORRATH_WORD_ADDRESS(callback))(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(parse + 8)));
    }
    void* file = NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(parse + 36));
    unsigned short header[2];
    if (ReadBegin__9VIObjFileRUsT1(file, &header[0], &header[1]) < 0) {
        return -1;
    }
    int result;
    switch (header[0]) {
    case 0x1000:
        result = ParseSurfaceObj__10VIESFParse(parse);
        break;
    case 0x1001:
        result = ParseSurfaceArrayObj__10VIESFParse(parse);
        break;
    case 0x1110:
        result = ParseMaterialPalObj__10VIESFParse(parse);
        break;
    case 0x2000:
        result = ParseSimpleSpriteObj__10VIESFParse(parse);
        break;
    case 0x2200:
        result = ParseHSpriteObj__10VIESFParse(parse);
        break;
    case 0x2600:
        result = ParseHSpriteAnimObj__10VIESFParse(parse);
        break;
    case 0x2700:
        result = ParseCSpriteObj__10VIESFParse(parse);
        break;
    case 0x2a00:
        result = ParseLODSpriteObj__10VIESFParse(parse);
        break;
    case 0x2b00:
        result = ParsePointLightObj__10VIESFParse(parse);
        break;
    case 0x2c00:
        result = ParseGroupSpriteObj__10VIESFParse(parse);
        break;
    case 0x2d00:
        result = ParsePointSpriteObj__10VIESFParse(parse);
        break;
    case 0x2e00:
        result = ParseStreamAudioSpriteObj__10VIESFParse(parse);
        break;
    case 0x2f00:
        result = ParseFloraSpriteObj__10VIESFParse(parse);
        break;
    case 0x6000:
        result = ParseActorObj__10VIESFParse(parse);
        break;
    case 0x7000:
        result = ParseFontObj__10VIESFParse(parse);
        break;
    case 0xb000:
        result = ParseAdpcmObj__10VIESFParse(parse);
        break;
    case 0xb030:
        result = ParseXmObj__10VIESFParse(parse);
        break;
    case 0xb100:
        result = ParseSoundSpriteObj__10VIESFParse(parse);
        break;
    case 0xc000:
        result = ParseParticleDefinitionObj__10VIESFParse(parse);
        break;
    case 0xc100:
        result = ParseParticleSpriteObj__10VIESFParse(parse);
        break;
    case 0xc200:
        result = ParseSpellEffectObj__10VIESFParse(parse);
        break;
    case 0xc300:
        result = ParseEffectVolumeSpriteObj__10VIESFParse(parse);
        break;
    case 0x8000:
        result = ParseWorldObj__10VIESFParsei(parse, 0);
        break;
    case 0xa000: {
        alignas(8) unsigned char array[32];
        __t7VIArray1Z14VIResourceElem(array);
        int parsed = ParseResourceDirObj__10VIESFParseiR13VIResourceDir(parse, 0, array);
        _$_t7VIArray1Z14VIResourceElem(array, 2);
        return parsed < 0 ? -1 : 0;
    }
    default:
        result = ReadEnd__9VIObjFile(file);
        break;
    }
    return result < 0 ? -1 : 0;
}
}
