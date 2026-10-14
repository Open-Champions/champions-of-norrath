// Observed: ParseWorldObj__10VIESFParsei at 0x011760d8 is 168 bytes. Arguments: parse (a0, the
// ESF parser record: scene word at +24, object file word at +36) and a flag (a1). In order it:
//   1. calls ReadBegin__9VIObjFileRUsT1(file, &tag, &second) and returns -1 when that is negative
//      or when the 16-bit tag is not 0x8100;
//   2. calls ReadEnd__9VIObjFile(file) (the tagged object is a header with no payload read here)
//      and returns -1 when negative;
//   3. calls ParseWorldBase__10VIESFParseR7VIWorld(parse, scene_word + 4) and returns -1 when
//      negative, then ReadEnd again with the same rule;
//   4. when the flag is non-zero it returns 0 here; otherwise it calls
//      ParseWorldZones__10VIESFParseR7VIWorld(parse, scene_word + 4) and returns 0 when that is
//      not negative, else -1.
// Inferred: the world is the sub-object at scene+4 (the callee argument is the scene word plus 4),
// 0x8100 is the world object tag, and the flag selects "base only" parsing, skipping the zones.
// Unknown: the meaning of the second ReadBegin output (written, never read) and the flag name.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ParseWorldBase__10VIESFParseR7VIWorld(void* parse, void* world);
int ParseWorldZones__10VIESFParseR7VIWorld(void* parse, void* world);

int ParseWorldObj__10VIESFParsei(unsigned char* parse, int flag) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x8100) return -1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (ParseWorldBase__10VIESFParseR7VIWorld(parse, NORRATH_WORD_ADDRESS(words[6] + 4)) < 0) return -1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (flag != 0) return 0;
    return ParseWorldZones__10VIESFParseR7VIWorld(parse, NORRATH_WORD_ADDRESS(words[6] + 4)) < 0 ? -1 : 0;
}
}
