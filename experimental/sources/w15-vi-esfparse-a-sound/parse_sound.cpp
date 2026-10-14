// Observed: ParseSound__10VIESFParse at 0x0116e508 is 156 bytes. Argument: parse (a0; object file word
// at +36). ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result returns -1. Tag 0xb000
// calls ParseAdpcmObj__10VIESFParse(parse) and tag 0xb030 calls ParseXmObj__10VIESFParse(parse); the
// result is returned unchanged when it is not negative, else -1. Any other tag calls
// ReadEnd__9VIObjFile(file) (result ignored) and returns -1.
// Inferred: 0xb000 and 0xb030 are the ADPCM and XM sound object tags (matching the handler names).
// Unknown: the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ParseAdpcmObj__10VIESFParse(void* parse);
int ParseXmObj__10VIESFParse(void* parse);

int ParseSound__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    int result;
    if (tag == 0xb000) {
        result = ParseAdpcmObj__10VIESFParse(parse);
    } else if (tag == 0xb030) {
        result = ParseXmObj__10VIESFParse(parse);
    } else {
        ReadEnd__9VIObjFile(file);
        return -1;
    }
    return result < 0 ? -1 : result;
}
}
