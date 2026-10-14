// Observed: ParsePointLightObj__10VIESFParse at 0x01177e78 is 492 bytes. Argument: parse (a0, the
// ESF parser record: creation-argument word at +0, scene word at +24, dictionary word at +32,
// object file word at +36). In order it:
//   1. reads an unsigned id, then one float (radius) and four floats into a local four-float
//      vector that starts as zeros;
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (id, &type,
//      &handle); a hit with a type other than 16 returns -1, and type 16 calls
//      ReadEnd__9VIObjFile (result ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 8, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 16, handle)
//      (negative returns -1);
//   4. it then fills the new sprite in place: sprite+32..40 = 0 (a centre), sprite+44 = radius,
//      sprite+8..16 = centre - radius with the x lane computed as -radius, sprite+20..28 = centre +
//      radius, and the four-float vector is copied to sprite+64..79;
//   5. ReadEnd, and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 8 and resource type 16 mean "point light"; the sprite's +8..28 range is an
// axis-aligned box around a sphere (centre at +32, radius at +44), and the vector at +64 is the
// light colour/intensity. Unknown: the field names, and why the x lane is negated rather than
// subtracted from the zero centre (the result is the same except for the sign of a zero radius,
// which this code reproduces).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);

int ParsePointLightObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    float vector[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float radius;
    unsigned id;
    int type;
    int handle;
    Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRf(file, &radius);
    Read__9VIObjFileRf(file, &vector[0]);
    Read__9VIObjFileRf(file, &vector[1]);
    Read__9VIObjFileRf(file, &vector[2]);
    Read__9VIObjFileRf(file, &vector[3]);
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
        if (type != 16) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 8, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    float* sprite = static_cast<float*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, id, 16, handle) < 0) return -1;
    sprite[11] = radius;
    sprite[9] = 0.0f;
    sprite[10] = 0.0f;
    sprite[8] = 0.0f;
    sprite[2] = -sprite[11];
    sprite[3] = sprite[9] - sprite[11];
    sprite[4] = sprite[10] - sprite[11];
    sprite[5] = sprite[8] + sprite[11];
    sprite[6] = sprite[9] + sprite[11];
    sprite[7] = sprite[10] + sprite[11];
    sprite[16] = vector[0];
    sprite[17] = vector[1];
    sprite[18] = vector[2];
    sprite[19] = vector[3];
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
