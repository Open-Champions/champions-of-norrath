// Observed: ParseCSpritePartDefinitions__10VIESFParse at 0x01174a90 is 248 bytes. Argument: parse
// (a0; object file word at +36).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative, or a tag other than 0x2950,
//      returns -1.
//   2. For each sub-object, with the count NumSubObjects__9VIObjFile(file) read again before every
//      entry and compared with the number of entries already started: ReadBegin__9VIObjFileRUsT1
//      is called again (a negative result returns -1); a tag other than 0xc000 calls
//      ReadEnd__9VIObjFile (result ignored) and returns -1; tag 0xc000 calls
//      ParseParticleDefinitionObj__10VIESFParse(parse) and a negative result returns -1.
//   3. Returns -1 when the final ReadEnd__9VIObjFile(file) is negative, else 0.
// Inferred: a list of particle definition objects for a character sprite (0xc000 is the tag that
// ParseObject dispatches to the particle definition handler). Unknown: the second ReadBegin
// outputs (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int NumSubObjects__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int ParseParticleDefinitionObj__10VIESFParse(void* parse);

int ParseCSpritePartDefinitions__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2950) return -1;
    int i = 0;
    while (i++ < NumSubObjects__9VIObjFile(file)) {
        unsigned short entry_tag;
        unsigned short entry_second;
        if (ReadBegin__9VIObjFileRUsT1(file, &entry_tag, &entry_second) < 0) return -1;
        if (entry_tag != 0xc000) {
            ReadEnd__9VIObjFile(file);
            return -1;
        }
        if (ParseParticleDefinitionObj__10VIESFParse(parse) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
