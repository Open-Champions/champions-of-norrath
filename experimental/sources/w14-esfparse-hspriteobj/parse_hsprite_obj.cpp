// Observed: ParseHSpriteObj__10VIESFParse at 0x01172530 is 1,016 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, raster word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1 (the tag is not
//      examined); reads an unsigned id and six floats into a local 24-byte box; ReadEnd__9VIObjFile
//      (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 5 returns -1, and type 5 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 4, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), Init__9VIHSprite(sprite) (negative returns -1) and, for a
//      non-zero id, Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 5,
//      handle) (negative returns -1);
//   4. the box is copied into the sprite (three floats to sprite+8..16, three to sprite+20..28) and
//      Init__8VISphereRC6VIBBox(sprite+32, &box) is called;
//   5. with a non-zero raster word: when ObjectVersion__9VIObjFile(file) is zero it expects a
//      0x1001 header (otherwise -1) followed, once per NumSubObjects__9VIObjFile(file) (re-read
//      each pass), by a header with tag 0x1000 and ParseSurfaceObj__10VIESFParse(parse), then
//      ReadEnd (any failure returns -1). Then, for every version, it reads a header and requires
//      tag 0x1110, whose body is ParseMaterialPalObj__10VIESFParse(parse); a failed read, another
//      tag or a negative palette result returns -1 (the palette handle is not stored);
//   6. with a zero raster word it only skips: one header and ReadEnd for version zero, then one
//      more header and ReadEnd (results ignored);
//   7. ParseHSpriteSpriteArray__10VIESFParse(parse), ParseHSpriteHierarchy__10VIESFParseR9VIHSprite
//      (parse, sprite), then the reference map ParseRefMap__10VIESFParse(parse) stored at
//      sprite+384 (also when negative, which returns -1) and ShareRefMap__7VIScenei(scene, map);
//      for ObjectVersion at least 2, ParseHSpriteTriggers__10VIESFParseR9VIHSprite(parse, sprite);
//      ParseHSpriteAttachments__10VIESFParseR9VIHSprite(parse, sprite); then the animation
//      ParseHSpriteAnim__10VIESFParse(parse) stored at sprite+388 (also when negative, which
//      returns -1), ShareAnimation__7VIScenei(scene, animation) and
//      AddPlay__9VIHSpriteiR7VIScene(sprite, sprite[+388], scene), each negative result returning
//      -1;
//   8. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 4 and resource type 5 mean "hierarchical sprite"; +384 and +388 are the
// shared reference-map and animation handles. Unknown: field names, and the callees' bodies (none
// of the helper parses are authored here).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct HSpriteBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int ObjectVersion__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int Init__9VIHSprite(void* sprite);
void Init__8VISphereRC6VIBBox(void* sphere, const HSpriteBox* box);
int ParseSurfaceObj__10VIESFParse(void* parse);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParseHSpriteSpriteArray__10VIESFParse(void* parse);
int ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(void* parse, void* sprite);
int ParseRefMap__10VIESFParse(void* parse);
int ShareRefMap__7VIScenei(void* scene, int map);
int ParseHSpriteTriggers__10VIESFParseR9VIHSprite(void* parse, void* sprite);
int ParseHSpriteAttachments__10VIESFParseR9VIHSprite(void* parse, void* sprite);
int ParseHSpriteAnim__10VIESFParse(void* parse);
int ShareAnimation__7VIScenei(void* scene, int animation);
int AddPlay__9VIHSpriteiR7VIScene(void* sprite, int animation, void* scene);

int ParseHSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    HSpriteBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRUi(file, &id);
    for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, &box.v[i]);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 5) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 4, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (Init__9VIHSprite(sprite) < 0) return -1;
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 5, handle) < 0) return -1;
    float* lower = reinterpret_cast<float*>(sprite + 8);
    float* upper = reinterpret_cast<float*>(sprite + 20);
    lower[0] = box.v[0];
    lower[1] = box.v[1];
    lower[2] = box.v[2];
    upper[0] = box.v[3];
    upper[1] = box.v[4];
    upper[2] = box.v[5];
    Init__8VISphereRC6VIBBox(sprite + 32, &box);
    if (words[3] != 0) {
        if (ObjectVersion__9VIObjFile(file) == 0) {
            unsigned short array_tag;
            unsigned short array_second;
            if (ReadBegin__9VIObjFileRUsT1(file, &array_tag, &array_second) < 0) return -1;
            if (array_tag != 0x1001) return -1;
            for (int i = 0; i < NumSubObjects__9VIObjFile(file); ++i) {
                unsigned short surface_tag;
                unsigned short surface_second;
                if (ReadBegin__9VIObjFileRUsT1(file, &surface_tag, &surface_second) < 0) return -1;
                if (surface_tag != 0x1000) return -1;
                if (ParseSurfaceObj__10VIESFParse(parse) < 0) return -1;
            }
            if (ReadEnd__9VIObjFile(file) < 0) return -1;
        }
        int palette = -1;
        unsigned short palette_tag;
        unsigned short palette_second;
        if (ReadBegin__9VIObjFileRUsT1(file, &palette_tag, &palette_second) >= 0 && palette_tag == 0x1110)
            palette = ParseMaterialPalObj__10VIESFParse(parse);
        if (palette < 0) return -1;
    } else {
        if (ObjectVersion__9VIObjFile(file) == 0) {
            ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
            ReadEnd__9VIObjFile(file);
        }
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        ReadEnd__9VIObjFile(file);
    }
    if (ParseHSpriteSpriteArray__10VIESFParse(parse) < 0) return -1;
    if (ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(parse, sprite) < 0) return -1;
    int map = ParseRefMap__10VIESFParse(parse);
    *reinterpret_cast<int*>(sprite + 384) = map;
    if (map < 0) return -1;
    if (ShareRefMap__7VIScenei(scene, map) < 0) return -1;
    if (static_cast<unsigned>(ObjectVersion__9VIObjFile(file)) >= 2) {
        if (ParseHSpriteTriggers__10VIESFParseR9VIHSprite(parse, sprite) < 0) return -1;
    }
    if (ParseHSpriteAttachments__10VIESFParseR9VIHSprite(parse, sprite) < 0) return -1;
    int animation = ParseHSpriteAnim__10VIESFParse(parse);
    *reinterpret_cast<int*>(sprite + 388) = animation;
    if (animation < 0) return -1;
    if (ShareAnimation__7VIScenei(scene, animation) < 0) return -1;
    if (AddPlay__9VIHSpriteiR7VIScene(sprite, *reinterpret_cast<int*>(sprite + 388), scene) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
