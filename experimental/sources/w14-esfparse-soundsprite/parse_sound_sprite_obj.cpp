// Observed: ParseSoundSpriteObj__10VIESFParse at 0x01178068 is 580 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, sound device word at +16, scene word at +24,
// dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1 (the tag is not
//      examined); reads a signed id; when the 16-bit second output is non-zero it reads two floats
//      (first, second), otherwise both are 1.0; then ReadEnd__9VIObjFile (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type,
//      &handle); a hit with a type other than 17 returns -1, and type 17 calls ReadEnd (ignored)
//      and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 9, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 17, handle)
//      (negative returns -1);
//   4. it fills the sprite in place like the point light: sprite+44 = first float, sprite+32..40 =
//      0, sprite+8..16 = -first, 0 - first, 0 - first and sprite+20..28 = 0 + first (three lanes);
//      sprite+84 = second float;
//   5. when the sound device word is non-zero it calls ParseSound__10VIESFParse(parse), stores the
//      result in sprite+64 even when negative (a negative result returns -1), and calls
//      ShareSound__13VISoundDevicei(device, result); when the device word is zero it instead reads
//      and closes the next sub-object header (ReadBegin then ReadEnd, each negative returning -1)
//      and skips the sound;
//   6. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 9 and resource type 17 mean "sound sprite"; the first float is the audible
// radius of the box, the second a scale kept at sprite+84; the sound handle in sprite+64 comes from
// the embedded sound sub-object. Unknown: the field names and what the second float means.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int ParseSound__10VIESFParse(void* parse);
void ShareSound__13VISoundDevicei(void* device, int sound);

int ParseSoundSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned short tag;
    unsigned short second;
    int id;
    float first;
    float other;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRi(file, &id);
    if (second != 0) {
        Read__9VIObjFileRf(file, &first);
        Read__9VIObjFileRf(file, &other);
    } else {
        first = 1.0f;
        other = 1.0f;
    }
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, static_cast<unsigned>(id), &type, &handle) != 0) {
        if (type != 17) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 9, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    float* sprite = static_cast<float*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, static_cast<unsigned>(id), 17, handle) < 0) return -1;
    sprite[11] = first;
    sprite[9] = 0.0f;
    sprite[10] = 0.0f;
    sprite[8] = 0.0f;
    sprite[2] = -sprite[11];
    sprite[3] = sprite[9] - sprite[11];
    sprite[4] = sprite[10] - sprite[11];
    sprite[5] = sprite[8] + sprite[11];
    sprite[6] = sprite[9] + sprite[11];
    sprite[7] = sprite[10] + sprite[11];
    sprite[21] = other;
    void* device = NORRATH_WORD_ADDRESS(words[4]);
    if (device != 0) {
        int sound = ParseSound__10VIESFParse(parse);
        reinterpret_cast<int*>(sprite)[16] = sound;
        if (sound < 0) return -1;
        ShareSound__13VISoundDevicei(device, sound);
    } else {
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
