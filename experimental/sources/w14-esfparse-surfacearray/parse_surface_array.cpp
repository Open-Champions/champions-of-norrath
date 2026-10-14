// Observed: ParseSurfaceArrayObj__10VIESFParse at 0x0117abe0 is 124 bytes. Argument: parse (a0), the
// same ESF parser record the dispatcher uses (object file word at +36). It reads the word at +12:
//   1. When that word is non-zero it calls ParseSurface__10VIESFParse(parse) once per sub-object,
//      re-reading NumSubObjects__9VIObjFile(file) before every pass (index 0, 1, ...), and returns -1
//      at once when a call returns a negative value.
//   2. It then calls ReadEnd__9VIObjFile(file) and returns -1 when that result is negative, else 0.
// Inferred: the 0x1001 object is an array of surface records, each parsed like a standalone 0x1000
// surface; the word at +12 is a flag that lets the parser skip the contents (it still closes the
// object). Unknown: the meaning of that flag, and what ParseSurface does with the file.
// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int NumSubObjects__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int ParseSurface__10VIESFParse(void* parse);

int ParseSurfaceArrayObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    if (words[3] != 0) {
        for (int i = 0; i < NumSubObjects__9VIObjFile(file); ++i) {
            if (ParseSurface__10VIESFParse(parse) < 0) return -1;
        }
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
