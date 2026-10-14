// Observed: ParseSurface__10VIESFParse at 0x0117ac60 is 96 bytes. Argument: parse (a0, the ESF parser record;
// the object file word is at +36). It calls ReadBegin__9VIObjFileRUsT1(file, &tag, &second); a negative
// result returns -1, and so does a tag other than 0x1000 (no ReadEnd on either path). Otherwise it
// calls ParseSurfaceObj__10VIESFParse(parse) and returns -1 when that result is negative, else the result unchanged
// (the handle).
// Inferred: this is the entry used for a nested surface object; the handler reads and closes its own object.
// Unknown: the second ReadBegin output is written but not read here.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ParseSurfaceObj__10VIESFParse(void* parse);

int ParseSurface__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x1000) return -1;
    int result = ParseSurfaceObj__10VIESFParse(parse);
    return result < 0 ? -1 : result;
}
}
