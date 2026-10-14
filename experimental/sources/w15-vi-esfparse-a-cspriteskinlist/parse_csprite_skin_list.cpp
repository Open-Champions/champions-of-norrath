// Observed: ParseCSpriteSkinList__10VIESFParseR9VICSprite at 0x01174258 is 292 bytes. Arguments: parse
// (a0; dictionary word at +32, object file word at +36) and sprite (a1, a character sprite).
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2900, returns -1.
//   2. Reads a signed count (Read__9VIObjFileRi). When the count is positive, for each entry:
//      an unsigned id (Read__9VIObjFileRUi) and a signed slot index (Read__9VIObjFileRi) are read;
//      the handle starts as -1; a non-zero id is looked up with
//      Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type, &handle), and a zero result
//      returns -1 at once (the found type is not examined). The handle is then stored into the
//      word at sprite + 3856 + 4 * slot.
//   3. Returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: a list of skin resource handles by slot (-1 for an entry without an id).
// Unknown: the slot count (the index is not range checked here), the expected resource type, and
// the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);

int ParseCSpriteSkinList__10VIESFParseR9VICSprite(unsigned char* parse, unsigned char* sprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned short tag;
    unsigned short second;
    int count;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2900) return -1;
    Read__9VIObjFileRi(file, &count);
    for (int i = 0; i < count; ++i) {
        unsigned id;
        int slot;
        int type;
        int handle;
        Read__9VIObjFileRUi(file, &id);
        Read__9VIObjFileRi(file, &slot);
        handle = -1;
        if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) == 0) return -1;
        *reinterpret_cast<int*>(sprite + 3856 + 4 * slot) = handle;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
