// Observed: ParseSoundArray__10VIESFParse at 0x0117b4b8 is 164 bytes. Argument: parse (a0, the ESF
// parser record; sound device word at +16, object file word at +36).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result or a tag other than 0xb070
//      returns -1.
//   2. When the sound device word is non-zero it calls ParseSound__10VIESFParse(parse) once per
//      sub-object: the count is NumSubObjects__9VIObjFile(file), read again before each call
//      and compared with the number of calls already made (none for a count of zero or less);
//      a negative ParseSound result returns -1 at once. When the device word is zero no sound is
//      parsed.
//   3. Returns -1 when ReadEnd__9VIObjFile(file) is negative, else 0.
// Inferred: 0xb070 is a sound array container. Unknown: the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int NumSubObjects__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int ParseSound__10VIESFParse(void* parse);

int ParseSoundArray__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0xb070) return -1;
    if (words[4] != 0) {
        int i = 0;
        while (i++ < NumSubObjects__9VIObjFile(file)) {
            if (ParseSound__10VIESFParse(parse) < 0) return -1;
        }
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
