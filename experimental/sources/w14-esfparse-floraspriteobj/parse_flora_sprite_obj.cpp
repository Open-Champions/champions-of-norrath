// Observed: ParseFloraSpriteObj__10VIESFParse at 0x011788a0 is 688 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, raster word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1 (the tag is not
//      examined); reads an unsigned id and six floats into a local 24-byte box; ReadEnd__9VIObjFile
//      (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 25 returns -1, and type 25 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 13, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 25, handle)
//      (negative returns -1);
//   4. it copies the box into the sprite (three floats to sprite+8..16, three to sprite+20..28)
//      and calls Init__8VISphereRC6VIBBox(sprite+32, &box);
//   5. when the raster word is non-zero it reads a header (ReadBegin) and stores in sprite+68 the
//      result of ParseMaterialPalObj__10VIESFParse(parse) if the read succeeded with tag 0x1110,
//      else -1 (stored even when negative; a negative value returns -1); it calls
//      ShareMaterialPal__8VIRasteri(raster, palette) (negative returns -1), then stores in
//      sprite+64 the result of ParseFloraPrimBuffer__10VIESFParse(parse) (stored even when
//      negative; negative returns -1) and calls SharePrimBuffer__8VIRasteri(raster, primbuffer)
//      (negative returns -1);
//   6. when the raster word is zero it instead skips two sub-objects (ReadBegin then ReadEnd, twice,
//      results ignored) and stores -1 in sprite+68 and sprite+64;
//   7. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 13 and resource type 25 mean "flora sprite"; the word at parse+12 is the
// raster that owns the shared palette and primitive buffers (null when only skipping). Unknown:
// the field names at sprite+64/+68, and what the sub-object tags other than 0x1110 are.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct FloraBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__8VISphereRC6VIBBox(void* sphere, const FloraBox* box);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParseFloraPrimBuffer__10VIESFParse(void* parse);
int ShareMaterialPal__8VIRasteri(void* raster, int palette);
int SharePrimBuffer__8VIRasteri(void* raster, int buffer);

int ParseFloraSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    FloraBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
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
        if (type != 25) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 13, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 25, handle) < 0) return -1;
    float* lower = reinterpret_cast<float*>(sprite + 8);
    float* upper = reinterpret_cast<float*>(sprite + 20);
    lower[0] = box.v[0];
    lower[1] = box.v[1];
    lower[2] = box.v[2];
    upper[0] = box.v[3];
    upper[1] = box.v[4];
    upper[2] = box.v[5];
    Init__8VISphereRC6VIBBox(sprite + 32, &box);
    int* palette_slot = reinterpret_cast<int*>(sprite + 68);
    int* buffer_slot = reinterpret_cast<int*>(sprite + 64);
    if (words[3] != 0) {
        void* raster = NORRATH_WORD_ADDRESS(words[3]);
        int palette = -1;
        unsigned short palette_tag;
        unsigned short palette_second;
        if (ReadBegin__9VIObjFileRUsT1(file, &palette_tag, &palette_second) >= 0 && palette_tag == 0x1110)
            palette = ParseMaterialPalObj__10VIESFParse(parse);
        *palette_slot = palette;
        if (palette < 0) return -1;
        if (ShareMaterialPal__8VIRasteri(raster, palette) < 0) return -1;
        int buffer = ParseFloraPrimBuffer__10VIESFParse(parse);
        *buffer_slot = buffer;
        if (buffer < 0) return -1;
        if (SharePrimBuffer__8VIRasteri(raster, buffer) < 0) return -1;
    } else {
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        ReadEnd__9VIObjFile(file);
        *palette_slot = -1;
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        *buffer_slot = -1;
        ReadEnd__9VIObjFile(file);
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
