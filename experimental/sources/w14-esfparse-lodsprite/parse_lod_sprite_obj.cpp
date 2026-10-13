// Observed: ParseLODSpriteObj__10VIESFParse at 0x011772e8 is 648 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, a flag word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1, and so does a tag
//      other than 0x2a10 (without ReadEnd);
//   2. reads an unsigned id and six floats into a local 24-byte box, then ReadEnd__9VIObjFile
//      (negative returns -1);
//   3. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 15 returns -1, and type 15 calls ReadEnd
//      (ignored) and returns the found handle;
//   4. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 7, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 15, handle)
//      (negative returns -1);
//   5. it copies the box into the sprite (three floats to sprite+8..16, three to sprite+20..28)
//      and calls Init__8VISphereRC6VIBBox(sprite+32, &box);
//   6. when ObjectVersion__9VIObjFile(file) is zero: with the flag word at +12 non-zero it reads
//      a header (ReadBegin; negative or a tag other than 0x1110 is a failure, returning -1) and
//      calls ParseMaterialPalObj__10VIESFParse(parse) (negative returns -1); with the flag zero it
//      reads and closes one header (ReadBegin then ReadEnd, results ignored). For a non-zero
//      version this whole step is skipped;
//   7. ParseLODSpriteSpriteArray__10VIESFParse(parse) and
//      ParseLODSpriteLevels__10VIESFParseR11VILODSprite(parse, sprite) (each negative returns -1),
//      then ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 7 and resource type 15 mean "level-of-detail sprite"; 0x2a10 is its body
// tag inside the 0x2a00 container; the old (version 0) layout embeds a material palette header
// that the parser consumes or skips. Unknown: the meaning of the flag at +12, and the two
// non-authored callees.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct LodBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int ObjectVersion__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__8VISphereRC6VIBBox(void* sphere, const LodBox* box);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParseLODSpriteSpriteArray__10VIESFParse(void* parse);
int ParseLODSpriteLevels__10VIESFParseR11VILODSprite(void* parse, void* sprite);

int ParseLODSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    LodBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x2a10) return -1;
    Read__9VIObjFileRUi(file, &id);
    for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, &box.v[i]);
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 15) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 7, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 15, handle) < 0) return -1;
    float* lower = reinterpret_cast<float*>(sprite + 8);
    float* upper = reinterpret_cast<float*>(sprite + 20);
    lower[0] = box.v[0];
    lower[1] = box.v[1];
    lower[2] = box.v[2];
    upper[0] = box.v[3];
    upper[1] = box.v[4];
    upper[2] = box.v[5];
    Init__8VISphereRC6VIBBox(sprite + 32, &box);
    if (ObjectVersion__9VIObjFile(file) == 0) {
        if (words[3] != 0) {
            int result = -1;
            unsigned short palette_tag;
            unsigned short palette_second;
            if (ReadBegin__9VIObjFileRUsT1(file, &palette_tag, &palette_second) >= 0 && palette_tag == 0x1110)
                result = ParseMaterialPalObj__10VIESFParse(parse);
            if (result < 0) return -1;
        } else {
            ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
            ReadEnd__9VIObjFile(file);
        }
    }
    if (ParseLODSpriteSpriteArray__10VIESFParse(parse) < 0) return -1;
    if (ParseLODSpriteLevels__10VIESFParseR11VILODSprite(parse, sprite) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
