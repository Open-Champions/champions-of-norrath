// Observed: ParseRefMap__10VIESFParse at 0x01173a20 is 420 bytes. Argument: parse (a0, the ESF parser
// record: creation-argument word at +0, scene word at +24, dictionary word at +32, object file word
// at +36).
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative, or a tag other than 0x5000,
//      returns -1.
//   2. Reads an unsigned id (Read__9VIObjFileRUi) and a signed count (Read__9VIObjFileRi).
//   3. When the id is non-zero and Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type,
//      &handle) succeeds: type other than 8 returns -1, otherwise ReadEnd (result ignored) and the
//      found handle is returned.
//   4. handle = CreateRefMap__7VIScenei(scene, parse[0]) (negative returns -1); map =
//      RefMap__7VIScenei(scene, handle); a non-zero id is registered with
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, map, id, 8, handle) (negative
//      returns -1).
//   5. Init__t5VIMap2ZiZii(map + 8, count) (negative returns -1), then count times: two
//      Read__9VIObjFileRi values a and b are copied to two locals and passed by reference to
//      Insert__t5VIMap2ZiZiRCiT1(map + 8, &a, &b) (negative returns -1).
//   6. Returns -1 when ReadEnd__9VIObjFile(file) is negative, else the handle.
// Inferred: resource type 8 is a reference map; the map is an integer-to-integer table at +8 of the
// map object (the mangled names read as a two-parameter map instantiated for int and int);
// 0x5000 is the reference map tag. Unknown: the second ReadBegin output (not used), the meaning
// of the key/value pairs, and the layouts of VIScene and the map.

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
int CreateRefMap__7VIScenei(void* scene, int argument);
void* RefMap__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int Init__t5VIMap2ZiZii(void* map, int count);
int Insert__t5VIMap2ZiZiRCiT1(void* map, const int* key, const int* value);

int ParseRefMap__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int count;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x5000) return -1;
    Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRi(file, &count);
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 8) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateRefMap__7VIScenei(scene, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* map = static_cast<unsigned char*>(RefMap__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, map, id, 8, handle) < 0) return -1;
    unsigned char* table = map + 8;
    if (Init__t5VIMap2ZiZii(table, count) < 0) return -1;
    for (int i = 0; i < count; ++i) {
        int first;
        int second_value;
        Read__9VIObjFileRi(file, &first);
        Read__9VIObjFileRi(file, &second_value);
        int key = first;
        int value = second_value;
        if (Insert__t5VIMap2ZiZiRCiT1(table, &key, &value) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
