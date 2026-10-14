// Observed: ParseWorldRegions__10VIESFParseR7VIWorld at 0x0117b320, 184 bytes. Arguments: parse (a0, object file word at +36) and
// world (a1, whose word at +80 is a signed count and word at +84 a destination address). ReadBegin__9VIObjFileRUsT1(file, &tag,
// &second): a negative result, or a tag other than 0x8240, returns -1 (the tag mismatch does not call ReadEnd). Otherwise it calls
// Read__9VIObjFileRUc(file, destination + i) for each index i below the count; both the count word and the destination word are
// read again on every pass, and nothing is read when the count is zero or negative. ReadEnd__9VIObjFile(file) follows: a negative
// result returns -1, otherwise 0.
// Inferred: the count/destination pair is a byte array that the file fills one byte at a time (the region ids or flags of the
// world); the tag names a region list record.
// Unknown: the member names and what the bytes mean.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Read__9VIObjFileRUc(void* file, unsigned char* value);

int ParseWorldRegions__10VIESFParseR7VIWorld(unsigned char* parse, unsigned char* world) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x8240) return -1;
    int* table = reinterpret_cast<int*>(world + 80);
    for (int i = 0; i < table[0]; ++i) {
        Read__9VIObjFileRUc(file, static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(static_cast<unsigned>(table[1]))) + i);
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
