// Observed: ParseCSpriteObj__10VIESFParse at 0x01173bc8 is 1,676 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, raster word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). Two version numbers are involved: `second`,
// the second output of the first ReadBegin__9VIObjFileRUsT1, and ObjectVersion__9VIObjFile(file)
// (an unsigned value). In order it:
//   1. ReadBegin(file, &tag, &second): negative returns -1 (the tag is not examined); reads an
//      unsigned id and six floats into a local 24-byte box; then, by `second`: a signed skeleton
//      value when second != 0 (otherwise 0); a float (scale) when second >= 2 (otherwise 1.0);
//      two signed values (race, sex) when second >= 3 (otherwise race 9 and sex 0); a signed value
//      (flags) when second >= 4 (otherwise 1); then ReadEnd__9VIObjFile (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 9 returns -1, and type 9 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 5, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSprite
//      Sexfi(sprite, skeleton, race, sex, scale, flags) (negative returns -1) and, for a non-zero
//      id, Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 9, handle)
//      (negative returns -1); the box is copied to sprite+8..16 and sprite+20..28 and
//      Init__8VISphereRC6VIBBox(sprite+32, &box) is called;
//   4. with a non-zero raster word and ObjectVersion below 3 it expects a 0x1001 header (otherwise
//      -1) and, once per NumSubObjects__9VIObjFile(file) (re-read each pass), a header with tag
//      0x1000 followed by ParseSurfaceObj__10VIESFParse(parse), then ReadEnd (any failure returns
//      -1); then, for every version, a header: the palette is -1 unless the read worked and the tag
//      is 0x1110, in which case it is ParseMaterialPalObj__10VIESFParse(parse); it is stored at
//      sprite+416 (also when -1) and a negative value returns -1; then ParseRefMap__10VIESFParse
//      (parse) is stored at sprite+420 (also when negative, which returns -1);
//   5. with a zero raster word it only skips: a header and ReadEnd for ObjectVersion below 3, then
//      two more header/ReadEnd pairs (results ignored); sprite+416/+420 are not written;
//   6. then, each negative result returning -1: ParseSoundArray__10VIESFParse (ObjectVersion at
//      least 4), ParseCSpriteSpriteArray__10VIESFParse, ParseHSpriteAnimArray__10VIESFParse,
//      ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(parse, sprite); a second ParseRefMap stored
//      at sprite+384 (also when negative); ParseHSpriteTriggers (version >= 4), ParseCSpriteSkinList,
//      ParseCSpritePlayList, ParseCSpriteNodeIDList (>= 2), ParseCSpriteASlotList (non-zero),
//      ParseCSpriteTSlotList (>= 5), ParseCSpriteContSound (>= 6), ParseCSpritePartDefinitions
//      (parse only, >= 7) and ParseCSpritePartEmitters (>= 7) -- the Parse*Slot/List/Sound/Part
//      functions take (parse, sprite) as ...R9VICSprite;
//   7. SetDefaultEntries__9VICSprite(sprite); InitTextSlots__9VICSpriteR8VIRaster(sprite, raster)
//      when the raster word is non-zero (result ignored); SetAnimPriorities__9VICSprite,
//      SetAnimSoundChannels__9VICSprite, Share__9VICSpriteR7VISceneP8VIRaster(sprite, scene,
//      raster), SetDefaults__9VICSpriteR7VIScene(sprite, scene), CalcLODOffDist__9VICSpriteR7VIScene
//      (sprite, scene) and CreateParticleEmitters__9VICSpriteR7VIScene(sprite, scene), each
//      negative result returning -1;
//   8. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 5 and resource type 9 mean "character sprite"; `second` is the header
// revision; the optional blocks are gated on ObjectVersion as listed. Unknown: names of the
// skeleton, race and sex enumerators, and the fields at sprite+384/+416/+420; none of the helper
// parses is authored here.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct CSpriteBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
unsigned ObjectVersion__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi(void* sprite, int skeleton, int race, int sex, float scale, int flags);
void Init__8VISphereRC6VIBBox(void* sphere, const CSpriteBox* box);
int ParseSurfaceObj__10VIESFParse(void* parse);
int ParseMaterialPalObj__10VIESFParse(void* parse);
int ParseRefMap__10VIESFParse(void* parse);
int ParseSoundArray__10VIESFParse(void* parse);
int ParseCSpriteSpriteArray__10VIESFParse(void* parse);
int ParseHSpriteAnimArray__10VIESFParse(void* parse);
int ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(void* parse, void* sprite);
int ParseHSpriteTriggers__10VIESFParseR9VIHSprite(void* parse, void* sprite);
int ParseCSpriteSkinList__10VIESFParseR9VICSprite(void* parse, void* sprite);
int ParseCSpritePlayList__10VIESFParseR9VICSprite(void* parse, void* sprite);
int ParseCSpriteNodeIDList__10VIESFParseR9VICSprite(void* parse, void* sprite);
int ParseCSpriteASlotList__10VIESFParseR9VICSprite(void* parse, void* sprite);
int ParseCSpriteTSlotList__10VIESFParseR9VICSprite(void* parse, void* sprite);
int ParseCSpriteContSound__10VIESFParseR9VICSprite(void* parse, void* sprite);
int ParseCSpritePartDefinitions__10VIESFParse(void* parse);
int ParseCSpritePartEmitters__10VIESFParseR9VICSprite(void* parse, void* sprite);
int SetDefaultEntries__9VICSprite(void* sprite);
int InitTextSlots__9VICSpriteR8VIRaster(void* sprite, void* raster);
int SetAnimPriorities__9VICSprite(void* sprite);
int SetAnimSoundChannels__9VICSprite(void* sprite);
int Share__9VICSpriteR7VISceneP8VIRaster(void* sprite, void* scene, void* raster);
int SetDefaults__9VICSpriteR7VIScene(void* sprite, void* scene);
int CalcLODOffDist__9VICSpriteR7VIScene(void* sprite, void* scene);
int CreateParticleEmitters__9VICSpriteR7VIScene(void* sprite, void* scene);

int ParseCSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    void* raster = NORRATH_WORD_ADDRESS(words[3]);
    CSpriteBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
    unsigned short tag;
    unsigned short second;
    unsigned id;
    int skeleton;
    float scale;
    int race;
    int sex;
    int flags;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRUi(file, &id);
    for (int i = 0; i < 6; ++i) Read__9VIObjFileRf(file, &box.v[i]);
    if (second != 0) Read__9VIObjFileRi(file, &skeleton);
    else skeleton = 0;
    if (second >= 2) Read__9VIObjFileRf(file, &scale);
    else scale = 1.0f;
    if (second >= 3) {
        Read__9VIObjFileRi(file, &race);
        Read__9VIObjFileRi(file, &sex);
    } else {
        race = 9;
        sex = 0;
    }
    if (second >= 4) Read__9VIObjFileRi(file, &flags);
    else flags = 1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 9) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 5, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi(sprite, skeleton, race, sex, scale, flags) < 0) return -1;
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 9, handle) < 0) return -1;
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
        if (ObjectVersion__9VIObjFile(file) < 3) {
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
        *reinterpret_cast<int*>(sprite + 416) = palette;
        if (palette < 0) return -1;
        int map = ParseRefMap__10VIESFParse(parse);
        *reinterpret_cast<int*>(sprite + 420) = map;
        if (map < 0) return -1;
    } else {
        if (ObjectVersion__9VIObjFile(file) < 3) {
            ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
            ReadEnd__9VIObjFile(file);
        }
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        ReadEnd__9VIObjFile(file);
        ReadBegin__9VIObjFileRUsT1(file, &tag, &second);
        ReadEnd__9VIObjFile(file);
    }
    if (ObjectVersion__9VIObjFile(file) >= 4) {
        if (ParseSoundArray__10VIESFParse(parse) < 0) return -1;
    }
    if (ParseCSpriteSpriteArray__10VIESFParse(parse) < 0) return -1;
    if (ParseHSpriteAnimArray__10VIESFParse(parse) < 0) return -1;
    if (ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(parse, sprite) < 0) return -1;
    int reference_map = ParseRefMap__10VIESFParse(parse);
    *reinterpret_cast<int*>(sprite + 384) = reference_map;
    if (reference_map < 0) return -1;
    if (ObjectVersion__9VIObjFile(file) >= 4) {
        if (ParseHSpriteTriggers__10VIESFParseR9VIHSprite(parse, sprite) < 0) return -1;
    }
    if (ParseCSpriteSkinList__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    if (ParseCSpritePlayList__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    if (ObjectVersion__9VIObjFile(file) >= 2) {
        if (ParseCSpriteNodeIDList__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    }
    if (ObjectVersion__9VIObjFile(file) != 0) {
        if (ParseCSpriteASlotList__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    }
    if (ObjectVersion__9VIObjFile(file) >= 5) {
        if (ParseCSpriteTSlotList__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    }
    if (ObjectVersion__9VIObjFile(file) >= 6) {
        if (ParseCSpriteContSound__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    }
    if (ObjectVersion__9VIObjFile(file) >= 7) {
        if (ParseCSpritePartDefinitions__10VIESFParse(parse) < 0) return -1;
    }
    if (ObjectVersion__9VIObjFile(file) >= 7) {
        if (ParseCSpritePartEmitters__10VIESFParseR9VICSprite(parse, sprite) < 0) return -1;
    }
    if (SetDefaultEntries__9VICSprite(sprite) < 0) return -1;
    if (words[3] != 0) InitTextSlots__9VICSpriteR8VIRaster(sprite, raster);
    if (SetAnimPriorities__9VICSprite(sprite) < 0) return -1;
    if (SetAnimSoundChannels__9VICSprite(sprite) < 0) return -1;
    if (Share__9VICSpriteR7VISceneP8VIRaster(sprite, scene, raster) < 0) return -1;
    if (SetDefaults__9VICSpriteR7VIScene(sprite, scene) < 0) return -1;
    if (CalcLODOffDist__9VICSpriteR7VIScene(sprite, scene) < 0) return -1;
    if (CreateParticleEmitters__9VICSpriteR7VIScene(sprite, scene) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
