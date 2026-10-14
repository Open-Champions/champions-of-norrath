// Observed: ParseWorldBase__10VIESFParseR7VIWorld at 0x01176488 is 244 bytes. Arguments: parse (a0, the
// ESF parser record: scene word at +24, object file word at +36) and a world reference (a1) that
// this function never reads; it works on the scene word plus offsets instead. In order it:
//   1. calls RemoveAllActors__7VIScene(scene) and returns -1 when negative;
//   2. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative, or a tag other than 0x8200,
//      returns -1;
//   3. when the second 16-bit output is non-zero, calls
//      ParseWorldBaseHeader__10VIESFParseR7VIWorld(parse, scene + 4) (negative returns -1);
//   4. ParseWorldZoneProxies__10VIESFParseR7VIWorld(parse, scene + 4) (negative returns -1);
//   5. ParseWorldTree__10VIESFParseR11VIWorldTree(parse, scene + 56) (negative returns -1);
//   6. when the second output is 2 or more, ParseWorldRegions__10VIESFParseR7VIWorld(parse,
//      scene + 4) (negative returns -1);
//   7. ReadEnd__9VIObjFile (negative returns -1), then UpdateAllActors__7VIScene(scene): negative
//      returns -1, otherwise 0.
// Inferred: the scene word points at a world object (the sub-object at +4 is passed as the world
// and the tree sits at +56), 0x8200 is the world base tag, and the second ReadBegin output is a
// format version that gates the header (non-zero) and the regions (2 or more). Unknown: the
// version name and the reason the world parameter is ignored.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int RemoveAllActors__7VIScene(void* scene);
int UpdateAllActors__7VIScene(void* scene);
int ParseWorldBaseHeader__10VIESFParseR7VIWorld(void* parse, void* world);
int ParseWorldZoneProxies__10VIESFParseR7VIWorld(void* parse, void* world);
int ParseWorldTree__10VIESFParseR11VIWorldTree(void* parse, void* tree);
int ParseWorldRegions__10VIESFParseR7VIWorld(void* parse, void* world);

int ParseWorldBase__10VIESFParseR7VIWorld(unsigned char* parse, void* world) {
    (void)world;
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned char* scene = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[6]));
    unsigned short tag;
    unsigned short second;
    if (RemoveAllActors__7VIScene(scene) < 0) return -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x8200) return -1;
    if (second != 0) {
        if (ParseWorldBaseHeader__10VIESFParseR7VIWorld(parse, scene + 4) < 0) return -1;
    }
    if (ParseWorldZoneProxies__10VIESFParseR7VIWorld(parse, scene + 4) < 0) return -1;
    if (ParseWorldTree__10VIESFParseR11VIWorldTree(parse, scene + 56) < 0) return -1;
    if (second >= 2) {
        if (ParseWorldRegions__10VIESFParseR7VIWorld(parse, scene + 4) < 0) return -1;
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return UpdateAllActors__7VIScene(scene) < 0 ? -1 : 0;
}
}
