// Observed: ParseWorldZoneProxies__10VIESFParseR7VIWorld at 0x01176760, 480 bytes. Arguments: parse (a0, object file word at +36)
// and world (a1, whose word at +76 is the base of an array of 592-byte zone records). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result, or a tag other than 0x8210, returns -1 (a mismatch does
//      not call ReadEnd);
//   2. Read__9VIObjFileRi reads a count;
//   3. for each index i below the count (the count is read again after every pass), with Z = (world word at +76) + 592 * i and that
//      word read again before every call:
//        - Init__6VIZone(Z); a negative result returns -1;
//        - Read__9VIObjFileRUl at Z + 8 and at Z + 16, Read__9VIObjFileRUi at Z + 24;
//        - nine Read__9VIObjFileRf calls at Z + 32, +36, +40, +44, +48, +52, +56, +60, +64;
//        - when `second` is exactly 1, ReadString__9VIObjFilePcUs(file, local, 31) into a 32-byte local that nothing reads
//          afterwards;
//   4. ReadEnd__9VIObjFile(file): a negative result returns -1, otherwise 0.
// Inferred: the zone records (592 bytes each, the size ParseWorldZones__10VIESFParseR7VIWorld steps by) are first created here as
// proxies: two 64-bit identifiers at +8 and +16 (the keys ParseWorldZones later reads), one word at +24 and nine floats (a box and
// transform); a version-1 file also carries a name, which this routine reads and discards.
// Unknown: the member names, the meaning of the fields, and the ReadString flag (31 looks like a buffer limit).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUl(void* file, unsigned long long* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadString__9VIObjFilePcUs(void* file, char* buffer, unsigned short size);
int Init__6VIZone(void* zone);

int ParseWorldZoneProxies__10VIESFParseR7VIWorld(unsigned char* parse, unsigned char* world) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    unsigned* worldWords = reinterpret_cast<unsigned*>(world);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int count;
    char name[32];
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x8210) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        int offset = i * 592;
        if (Init__6VIZone(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(worldWords[19])) + offset) < 0) return -1;
        Read__9VIObjFileRUl(file, reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(worldWords[19])) + offset + 8));
        Read__9VIObjFileRUl(file, reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(worldWords[19])) + offset + 16));
        Read__9VIObjFileRUi(file, reinterpret_cast<unsigned*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(worldWords[19])) + offset + 24));
        for (int field = 32; field <= 64; field += 4) {
            Read__9VIObjFileRf(file, reinterpret_cast<float*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(worldWords[19])) + offset + field));
        }
        if (second == 1) ReadString__9VIObjFilePcUs(file, name, 31);
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
