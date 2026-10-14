// Observed: ParseLODSpriteLevels__10VIESFParseR11VILODSprite at 0x01177668 is 312 bytes. Arguments:
// parse (a0; scene word at +24, dictionary word at +32, object file word at +36) and lod (a1, a LOD
// sprite).
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x2a30, returns -1.
//   2. Reads a signed level count into the word at lod+68. For level i in [0, count) (the count
//      is read from lod+68 again after every level; entries are 8 bytes apart from lod+0): an
//      unsigned id is read (Read__9VIObjFileRUi) and looked up with
//      Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type, &handle), where the handle is
//      written straight into the word at lod + 8*i + 76; a zero result returns -1 (the type and a
//      zero id are not examined). Then ShareSprite__7VIScenei(scene, handle) is called (a negative
//      result returns -1) and one float is read into lod + 8*i + 72 (Read__9VIObjFileRf).
//   3. When the count word is not zero (including a negative count) the float at
//      lod + 8*count + 64 is copied to lod+48. Then it returns -1 when ReadEnd__9VIObjFile is
//      negative, else 0.
// Inferred: the float of the last level (distance) is copied to a LOD sprite field at +48; the
// level table starts near lod+64. Unknown: the meaning of the stored float, the table capacity
// (the count is not range checked) and the second ReadBegin output (not used).

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

int ParseLODSpriteLevels__10VIESFParseR11VILODSprite(unsigned char* parse, unsigned char* lod) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned short tag;
    unsigned short second;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2a30) return -1;
    int* count = reinterpret_cast<int*>(lod + 68);
    Read__9VIObjFileRi(file, count);
    for (int i = 0; i < *count; ++i) {
        unsigned char* level = lod + 8 * i;
        unsigned id;
        int type;
        Read__9VIObjFileRUi(file, &id);
        if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, reinterpret_cast<int*>(level + 76)) == 0) return -1;
        if (ShareSprite__7VIScenei(scene, *reinterpret_cast<int*>(level + 76)) < 0) return -1;
        Read__9VIObjFileRf(file, reinterpret_cast<float*>(level + 72));
    }
    int final_count = *count;
    if (final_count != 0) *reinterpret_cast<float*>(lod + 48) = *reinterpret_cast<float*>(lod + final_count * 8 + 64);
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
