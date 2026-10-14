// Observed: ParseCSpritePartEmitters__10VIESFParseR9VICSprite at 0x01174b88 is 504 bytes. Arguments:
// parse (a0; word at +12, scene word at +24, dictionary word at +32, object file word at +36) and
// sprite (a1, a character sprite whose emitter array object sits at +4616: element pointer at
// +4 of the array object, elements 1056 bytes each). In order it:
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2960, returns -1;
//   2. reads a signed count (Read__9VIObjFileRi) and calls
//      Init__t7VIArray1Z20VICSpritePartEmitteri(array, count); a negative result returns -1;
//   3. for each index below the count (the count local and the element pointer are read again
//      every iteration): reads an unsigned id (Read__9VIObjFileRUi). The element handle word at
//      element+0 is -1, unless both the scene word plus 1352 and the parse word at +12 are non-zero:
//      then FindTyped__12VIDictionaryUi14VIResourceTypeRi(dict, id, 18, &handle) must succeed
//      (a zero result returns -1 without ReadEnd) and the found handle is stored (no check that
//      the id is non-zero). Then seven signed values (Read__9VIObjFileRi) are read: the first is
//      stored to element+1008; bit 0 of the second sets bit 0 of the word at element+1012 (other
//      bits are left as they were, the word is otherwise not written here); the third is read
//      straight into element+1016; the fourth, fifth, sixth and seventh go to element+1020,
//      +1024, +1028 and +1032 (the fourth and fifth through a local, the last two read straight
//      into the element);
//   4. returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: type 18 is a particle emitter/definition resource looked up per emitter; the emitter
// handle stays -1 when particle support is absent. Unknown: the meaning of the parse word at +12
// and of the stored fields (names), the scene sub-object at +1352 (only the address sum is tested),
// and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* handle);
int Init__t7VIArray1Z20VICSpritePartEmitteri(void* array, int count);

int ParseCSpritePartEmitters__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned char* array = sprite + 4616;
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2960) return -1;
    Read__9VIObjFileRi(file, &count);
    if (Init__t7VIArray1Z20VICSpritePartEmitteri(array, count) < 0) return -1;
    for (int i = 0; i < count; ++i) {
        unsigned id;
        int handle = -1;
        int value;
        Read__9VIObjFileRUi(file, &id);
        if (words[6] + 1352 != 0 && words[3] != 0) {
            if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, id, 18, &handle) == 0) return -1;
        }
        unsigned* data = reinterpret_cast<unsigned*>(array + 4);
#define ELEMENT_WORD(offset) (*reinterpret_cast<int*>(NORRATH_WORD_ADDRESS(*data + 1056 * i + (offset))))
        ELEMENT_WORD(0) = handle;
        Read__9VIObjFileRi(file, &value);
        ELEMENT_WORD(1008) = value;
        Read__9VIObjFileRi(file, &value);
        if (value & 1) ELEMENT_WORD(1012) |= 1;
        Read__9VIObjFileRi(file, &ELEMENT_WORD(1016));
        Read__9VIObjFileRi(file, &value);
        int fourth = value;
        Read__9VIObjFileRi(file, &value);
        ELEMENT_WORD(1020) = fourth;
        ELEMENT_WORD(1024) = value;
        Read__9VIObjFileRi(file, &ELEMENT_WORD(1028));
        Read__9VIObjFileRi(file, &ELEMENT_WORD(1032));
#undef ELEMENT_WORD
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
