// Observed: ParseGroupSpriteObj__10VIESFParse at 0x01177af0 is 536 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, scene word at +24, dictionary word at +32,
// object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1, and so does a tag
//      other than 0x2c10 (without ReadEnd);
//   2. reads an unsigned id and six floats into a local 24-byte box (three, then three), then
//      ReadEnd__9VIObjFile (negative returns -1);
//   3. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 20 returns -1, and type 20 calls ReadEnd
//      (ignored) and returns the found handle;
//   4. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 11, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 20, handle)
//      (negative returns -1);
//   5. it copies the box into the sprite (first three floats to sprite+8..16, last three to
//      sprite+20..28), calls Init__8VISphereRC6VIBBox(sprite+32, &box), then
//      ParseGroupSpriteSpriteArray__10VIESFParse(parse) and
//      ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite(parse, sprite) (each negative
//      returning -1), and finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 11 and resource type 20 mean "group sprite"; 0x2c10 is the group sprite
// body tag inside the 0x2c00 container; the sphere at sprite+32 is initialised from the box
// (a bounding sphere). Unknown: the two group callees are not authored here, and the box layout
// (min then max) is inferred from the copy to +8 and +20.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct GroupBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__8VISphereRC6VIBBox(void* sphere, const GroupBox* box);
int ParseGroupSpriteSpriteArray__10VIESFParse(void* parse);
int ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite(void* parse, void* sprite);

int ParseGroupSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    GroupBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2c10) return -1;
    Read__9VIObjFileRUi(file, &id);
    for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, &box.v[i]);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 20) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 11, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 20, handle) < 0) return -1;
    float* lower = reinterpret_cast<float*>(sprite + 8);
    float* upper = reinterpret_cast<float*>(sprite + 20);
    lower[0] = box.v[0];
    lower[1] = box.v[1];
    lower[2] = box.v[2];
    upper[0] = box.v[3];
    upper[1] = box.v[4];
    upper[2] = box.v[5];
    Init__8VISphereRC6VIBBox(sprite + 32, &box);
    if (ParseGroupSpriteSpriteArray__10VIESFParse(parse) < 0) return -1;
    if (ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite(parse, sprite) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
