// Observed: ParsePointSpriteObj__10VIESFParse at 0x011785d0 is 276 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, scene word at +24, dictionary word at +32,
// object file word at +36). In order it:
//   1. reads an unsigned id and then a signed value (called "kind" below) from the object file;
//   2. when the id is non-zero it asks the dictionary (Find__12VIDictionaryUiR14VIResourceTypeRi,
//      with the id and two output words: a resource type and a handle). When it finds the id:
//      a type other than 23 returns -1; type 23 calls ReadEnd__9VIObjFile (result ignored) and
//      returns the found handle;
//   3. otherwise it calls CreateSprite__7VIScene12VISpriteTypei(scene, 2, parse[0]); a negative
//      handle is returned as -1. It fetches the sprite with Sprite__7VIScenei(scene, handle),
//      and, for a non-zero id, registers it with
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 23, handle), returning
//      -1 when that is negative;
//   4. it calls Init__13VIPointSprite11VIPointType(sprite, kind), then ReadEnd and returns -1 when
//      that is negative, else the new handle.
// Inferred: sprite type 2 and resource type 23 both mean "point sprite"; the id is the dictionary
// key under which a point sprite is shared between files. Unknown: the argument at parse+0 and the
// enumerators behind 2, 23 and the kind value.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__13VIPointSprite11VIPointType(void* sprite, int kind);

int ParsePointSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned id;
    int kind;
    int handle;
    int type;
    Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRi(file, &kind);
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 23) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 2, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    void* sprite = Sprite__7VIScenei(scene, handle);
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 23, handle) < 0) return -1;
    Init__13VIPointSprite11VIPointType(sprite, kind);
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
