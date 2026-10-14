// Observed: ParseCSpriteContSound__10VIESFParseR9VICSprite at 0x011749d0 is 188 bytes. Arguments:
// parse (a0; sound device word at +16, dictionary word at +32, object file word at +36) and sprite
// (a1, a character sprite).
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2940, returns -1.
//   2. Reads an unsigned id (Read__9VIObjFileRUi).
//   3. The handle starts as -1. When the sound device word is non-zero and the id is non-zero it
//      calls FindTyped__12VIDictionaryUi14VIResourceTypeRi(dict, id, 14, &handle); a zero result
//      returns -1 (without ReadEnd), a non-zero result leaves the found handle.
//   4. Stores the handle into the word at sprite+4592, reads a float into sprite+4596
//      (Read__9VIObjFileRf), then returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: type 14 is the sound resource type (the ADPCM and XM parsers add type 14); the two
// stored fields are a continuous-sound handle (-1 when none) and a float parameter.
// Unknown: the meaning of the float, and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* handle);

int ParseCSpriteContSound__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int handle = -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2940) return -1;
    Read__9VIObjFileRUi(file, &id);
    if (words[4] != 0 && id != 0) {
        if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, 14, &handle) == 0) return -1;
    }
    *reinterpret_cast<int*>(sprite + 4592) = handle;
    Read__9VIObjFileRf(file, reinterpret_cast<float*>(sprite + 4596));
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
