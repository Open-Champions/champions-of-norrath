// Observed: ParseHSpriteTriggers__10VIESFParseR9VIHSprite at 0x01172c10 is 212 bytes. Arguments: parse
// (a0; object file word at +36) and sprite (a1, a hierarchical sprite; an array object at sprite+88
// whose data pointer is the word at array+4).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative, or a tag other than 0x2450,
//      returns -1.
//   2. Reads a signed count with Read__9VIObjFileRi, then calls
//      Init__t7VIArray1Z16VIHSpriteTriggeri(sprite + 88, count); a negative result returns -1.
//   3. For i in [0, count) it reads one signed integer with Read__9VIObjFileRi into the first word
//      of 16-byte element i of the array's data (the data pointer is reloaded for every element).
//   4. Returns -1 when ReadEnd__9VIObjFile(file) is negative, else 0.
// Inferred: 0x2450 is the trigger list tag; each trigger element is 16 bytes with the file
// supplying only its first word. Unknown: the other three words of an element and the second
// ReadBegin output.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Init__t7VIArray1Z16VIHSpriteTriggeri(void* array, int count);

int ParseHSpriteTriggers__10VIESFParseR9VIHSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2450) return -1;
    Read__9VIObjFileRi(file, &count);
    unsigned char* array = sprite + 88;
    if (Init__t7VIArray1Z16VIHSpriteTriggeri(array, count) < 0) return -1;
    for (int i = 0; i < count; ++i) {
        unsigned char* data = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(array + 4)));
        Read__9VIObjFileRi(file, reinterpret_cast<int*>(data + i * 16));
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
