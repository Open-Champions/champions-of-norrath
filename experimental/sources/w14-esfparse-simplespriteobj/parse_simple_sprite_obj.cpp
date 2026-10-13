// Observed: ParseSimpleSpriteObj__10VIESFParse at 0x01172128 is 1,032 bytes. Argument: parse (a0,
// the ESF parser record: creation-argument word at +0, raster word at +12, collision-system word
// at +20, scene word at +24, dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1 (the tag is not
//      examined); reads an unsigned id, six floats into a local 24-byte box and, only when the
//      16-bit second output is non-zero, one more float (otherwise 500.0); ReadEnd__9VIObjFile
//      (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 4 returns -1, and type 4 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 1, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 4, handle)
//      (negative returns -1);
//   4. the box is copied into the sprite (three floats to sprite+8..16, three to sprite+20..28),
//      Init__8VISphereRC6VIBBox(sprite+32, &box) is called and the float is stored at sprite+48;
//   5. with a non-zero raster word and ObjectVersion__9VIObjFile(file) below 2 it expects a 0x1001
//      header (otherwise -1) and, once per NumSubObjects__9VIObjFile(file) (re-read each pass), a
//      header with tag 0x1000 followed by ParseSurfaceObj__10VIESFParse(parse), then ReadEnd;
//      any failure returns -1. It then reads a header; the palette is -1 unless the read worked
//      and the tag is 0x1110, in which case it is ParseMaterialPalObj__10VIESFParse(parse); it is
//      stored at sprite+72 (also when -1) and a negative value returns -1. Then
//      ShareMaterialPal__8VIRasteri(raster, palette) (negative returns -1), the primitive buffer
//      ParsePrimBuffer__10VIESFParse(parse) stored at sprite+64 (also when negative, which returns
//      -1) and SharePrimBuffer__8VIRasteri(raster, buffer) (negative returns -1);
//   6. with a zero raster word it only skips: for versions below 2 one header and ReadEnd, then two
//      more header/ReadEnd pairs, storing -1 in sprite+72 and sprite+64;
//   7. when the version is non-zero it parses the collision buffer ParseCollBuffer__10VIESFParse
//      (parse), stores it at sprite+68 (also when negative, which returns -1) and calls
//      ShareCollBuffer__9VICollidei(collision, buffer) (negative returns -1);
//   8. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 1 and resource type 4 mean "simple sprite"; the extra float (default 500)
// is a draw/cull distance stored at +48; +64, +68 and +72 are the shared primitive, collision and
// palette handles. Unknown: field names, and the meaning of tags 0x1001/0x1000/0x1110.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct SimpleBox { float v[6]; };

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
void Init__8VISphereRC6VIBBox(void* sphere, const SimpleBox* box);
int ParseSurfaceObj__10VIESFParse(void* parse);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParsePrimBuffer__10VIESFParse(void* parse);
int ParseCollBuffer__10VIESFParse(void* parse);
int ShareMaterialPal__8VIRasteri(void* raster, int palette);
int SharePrimBuffer__8VIRasteri(void* raster, int buffer);
int ShareCollBuffer__9VICollidei(void* collision, int buffer);

int ParseSimpleSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    SimpleBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    unsigned short tag;
    unsigned short second;
    unsigned id;
    float distance;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRUi(file, &id);
    for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, &box.v[i]);
    if (second != 0) Read__9VIObjFileRf(file, &distance);
    else distance = 500.0f;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 4) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 1, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 4, handle) < 0) return -1;
    float* lower = reinterpret_cast<float*>(sprite + 8);
    float* upper = reinterpret_cast<float*>(sprite + 20);
    lower[0] = box.v[0];
    lower[1] = box.v[1];
    lower[2] = box.v[2];
    upper[0] = box.v[3];
    upper[1] = box.v[4];
    upper[2] = box.v[5];
    Init__8VISphereRC6VIBBox(sprite + 32, &box);
    *reinterpret_cast<float*>(sprite + 48) = distance;
    if (words[3] != 0) {
        void* raster = NORRATH_WORD_ADDRESS(words[3]);
        if (static_cast<unsigned>(ObjectVersion__9VIObjFile(file)) < 2) {
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
        *reinterpret_cast<int*>(sprite + 72) = palette;
        if (palette < 0) return -1;
        if (ShareMaterialPal__8VIRasteri(raster, palette) < 0) return -1;
        int buffer = ParsePrimBuffer__10VIESFParse(parse);
        *reinterpret_cast<int*>(sprite + 64) = buffer;
        if (buffer < 0) return -1;
        if (SharePrimBuffer__8VIRasteri(raster, buffer) < 0) return -1;
    } else {
        if (static_cast<unsigned>(ObjectVersion__9VIObjFile(file)) < 2) {
            ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
            ReadEnd__9VIObjFile(file);
        }
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        ReadEnd__9VIObjFile(file);
        *reinterpret_cast<int*>(sprite + 72) = -1;
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        *reinterpret_cast<int*>(sprite + 64) = -1;
        ReadEnd__9VIObjFile(file);
    }
    if (ObjectVersion__9VIObjFile(file) != 0) {
        int collision = ParseCollBuffer__10VIESFParse(parse);
        *reinterpret_cast<int*>(sprite + 68) = collision;
        if (collision < 0) return -1;
        if (ShareCollBuffer__9VICollidei(NORRATH_WORD_ADDRESS(words[5]), collision) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
