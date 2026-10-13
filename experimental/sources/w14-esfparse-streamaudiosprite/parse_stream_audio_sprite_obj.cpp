// Observed: ParseStreamAudioSpriteObj__10VIESFParse at 0x011786e8 is 440 bytes. Argument: parse
// (a0, the ESF parser record: creation-argument word at +0, scene word at +24, dictionary word at
// +32, object file word at +36). In order it:
//   1. reads an unsigned id and then six floats into a local 24-byte box (three, then three);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (id, &type,
//      &handle); on a hit a type other than 24 returns -1, and type 24 calls ReadEnd__9VIObjFile
//      (result ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 12, parse[0]) (a negative handle
//      returns -1), Sprite__7VIScenei(scene, handle), and, for a non-zero id,
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 24, handle) (a
//      negative result returns -1);
//   4. Init__19VIStreamAudioSpriteRC6VIBBox(sprite, &box), then ReadString__9VIObjFilePcUs(file,
//      sprite+72, 127), a signed int into sprite+64 and a second signed int into sprite+68, then
//      ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 12 and resource type 24 mean "stream audio sprite"; the string at +72 is
// the relative path of the stream; +64 and +68 are two integer attributes (the existing accessors
// for priority and looping read some of these). Unknown: which attribute is which, the 127 limit's
// meaning beyond the buffer size, and the field names.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct StreamAudioBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadString__9VIObjFilePcUs(void* file, char* buffer, unsigned short size);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__19VIStreamAudioSpriteRC6VIBBox(void* sprite, const StreamAudioBox* box);

int ParseStreamAudioSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    StreamAudioBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    unsigned id;
    int type;
    int handle;
    int second;
    Read__9VIObjFileRUi(file, &id);
    for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, &box.v[i]);
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 24) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 12, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 24, handle) < 0) return -1;
    Init__19VIStreamAudioSpriteRC6VIBBox(sprite, &box);
    ReadString__9VIObjFilePcUs(file, reinterpret_cast<char*>(sprite + 72), 127);
    Read__9VIObjFileRi(file, reinterpret_cast<int*>(sprite + 64));
    Read__9VIObjFileRi(file, &second);
    *reinterpret_cast<int*>(sprite + 68) = second;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
