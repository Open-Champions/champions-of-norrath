// Observed: ParseEffectVolumeSpriteObj__10VIESFParse at 0x01178b50 is 948 bytes. Argument: parse
// (a0, the ESF parser record: creation-argument word at +0, a flag word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1 (the tag is not
//      examined); reads an unsigned id and six floats into a local 24-byte box; ReadEnd__9VIObjFile
//      (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 26 returns -1, and type 26 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 14, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 26, handle)
//      (negative returns -1); then Init__20VIEffectVolumeSpriteRC6VIBBox(sprite, &box);
//   4. reads a header (negative returns -1). A tag 0xc320 block: when the flag word at +12 is
//      non-zero it takes n = NumSubObjects__9VIObjFile(file) and calls ParseObject__10VIESFParse
//      (parse) n times (negative returns -1); then ReadEnd (negative returns -1) and another
//      ReadBegin (negative returns -1). A tag other than 0xc320 skips this block;
//   5. when the tag is now 0xc330 it reads a signed volume type, five floats (f0..f4), two signed
//      ints (surface ids a, b), and, only when the second output of that ReadBegin is at least 2,
//      an unsigned flags word (otherwise 0). When the flag word at +12 is non-zero it looks up
//      the surface ids a and b with Find (each hit must have type 1, else -1; a miss leaves the
//      slot at -1), then on the volume at sprite+88 calls
//      SetType__14VIEffectVolume18VIEffectVolumeTypef(volume, volume type, f4), stores f3 at
//      volume+40, f0 at +52, f1 at +56 and f2 at +48, calls AttachSurface__14VIEffectVolumeii
//      (volume, 0, slot0) and (volume, 1, slot1), then SetFlags__14VIEffectVolumeUi(volume, flags).
//      When the flag word is zero the values are read and dropped;
//   6. finally ReadEnd (negative returns -1), a second ReadEnd and the handle (-1 when the second
//      ReadEnd is negative).
// Inferred: sprite type 14 and resource type 26 mean "effect volume sprite"; the two ids select
// surfaces (resource type 1) attached to the volume; 0xc320 holds nested resources and 0xc330 the
// volume parameters. Unknown: the five float roles (three are a vector at +52/+56/+48), the first
// slot meaning of a and b, and the flag word's bits.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct EffectBox { float v[6]; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int NumSubObjects__9VIObjFile(void* file);
int ParseObject__10VIESFParse(void* parse);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__20VIEffectVolumeSpriteRC6VIBBox(void* sprite, const EffectBox* box);
void SetType__14VIEffectVolume18VIEffectVolumeTypef(void* volume, int type, float value);
void AttachSurface__14VIEffectVolumeii(void* volume, int slot, int surface);
void SetFlags__14VIEffectVolumeUi(void* volume, unsigned flags);

int ParseEffectVolumeSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    EffectBox box = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
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
        if (type != 26) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 14, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 26, handle) < 0) return -1;
    Init__20VIEffectVolumeSpriteRC6VIBBox(sprite, &box);
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag == 0xc320) {
        if (words[3] != 0) {
            int count = NumSubObjects__9VIObjFile(file);
            for (int i = 0; i < count; ++i) {
                if (ParseObject__10VIESFParse(parse) < 0) return -1;
            }
        }
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    }
    if (tag == 0xc330) {
        int volume_type;
        float values[5];
        int surface_ids[2];
        unsigned flags = 0;
        Read__9VIObjFileRi(file, &volume_type);
        for (int i = 0; i < 5; ++i) Read__9VIObjFileRf(file, &values[i]);
        Read__9VIObjFileRi(file, &surface_ids[0]);
        Read__9VIObjFileRi(file, &surface_ids[1]);
        if (second >= 2) Read__9VIObjFileRUi(file, &flags);
        if (words[3] != 0) {
            int slots[2] = {-1, -1};
            for (int i = 0; i < 2; ++i) {
                if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, static_cast<unsigned>(surface_ids[i]), &type, &slots[i]) != 0 && type != 1) return -1;
            }
            unsigned char* volume = sprite + 88;
            SetType__14VIEffectVolume18VIEffectVolumeTypef(volume, volume_type, values[4]);
            *reinterpret_cast<float*>(volume + 40) = values[3];
            *reinterpret_cast<float*>(volume + 52) = values[0];
            *reinterpret_cast<float*>(volume + 56) = values[1];
            *reinterpret_cast<float*>(volume + 48) = values[2];
            AttachSurface__14VIEffectVolumeii(volume, 0, slots[0]);
            AttachSurface__14VIEffectVolumeii(volume, 1, slots[1]);
            SetFlags__14VIEffectVolumeUi(volume, flags);
        }
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
