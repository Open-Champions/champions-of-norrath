// Observed: ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite at 0x01177d08 is 368 bytes.
// Arguments: parse (a0; scene word at +24, dictionary word at +32, object file word at +36) and group
// (a1, a group sprite).
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2c30, returns -1.
//   2. Reads a signed member count into the word at group+64. For member i in [0, count) (the count
//      is read from group+64 again after every member; entries are 32 bytes apart from group+0):
//      an unsigned id is read (Read__9VIObjFileRUi) and looked up with
//      Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type, &handle), where the handle is
//      written straight into the word at group + 32*i + 68; a zero result returns -1 (the type is
//      not examined). Then ShareSprite__7VIScenei(scene, handle) is called, a negative result
//      returns -1, and seven floats are read into group + 32*i + 72, +76, +80, +84, +88, +92, +96
//      in that order (Read__9VIObjFileRf).
//   3. Returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: a list of member sprites, each with a handle and seven floats (probably a placement
// transform). Unknown: what the floats mean, what the group holds in +0..+63 and in the first
// four bytes of each entry, the capacity of the table (the count is not range checked), and the
// second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int ShareSprite__7VIScenei(void* scene, int handle);

int ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite(unsigned char* parse, unsigned char* group) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2c30) return -1;
    int* count = reinterpret_cast<int*>(group + 64);
    Read__9VIObjFileRi(file, count);
    for (int i = 0; i < *count; ++i) {
        unsigned char* member = group + 32 * i;
        unsigned id;
        int type;
        Read__9VIObjFileRUi(file, &id);
        if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, reinterpret_cast<int*>(member + 68)) == 0) return -1;
        if (ShareSprite__7VIScenei(scene, *reinterpret_cast<int*>(member + 68)) < 0) return -1;
        for (int k = 0; k < 7; ++k) Read__9VIObjFileRf(file, reinterpret_cast<float*>(member + 72 + 4 * k));
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
