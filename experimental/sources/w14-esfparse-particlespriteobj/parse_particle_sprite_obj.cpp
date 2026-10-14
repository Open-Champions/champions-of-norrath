// Observed: ParseParticleSpriteObj__10VIESFParse at 0x011782b0 is 796 bytes. Argument: parse (a0,
// the ESF parser record: creation-argument word at +0, a flag word at +12, scene word at +24,
// dictionary word at +32, object file word at +36). The particle system is the sub-object at
// scene+1352. In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): negative returns -1 (the tag is not
//      examined); reads a signed id; when the 16-bit second output is non-zero it also reads a
//      signed int (extra), otherwise extra is 0; then ReadEnd__9VIObjFile (negative returns -1);
//   2. when the id is non-zero it calls Find__12VIDictionaryUiR14VIResourceTypeRi (dict, id,
//      &type, &handle); a hit with a type other than 19 returns -1, and type 19 calls ReadEnd
//      (ignored) and returns the found handle;
//   3. otherwise CreateSprite__7VIScene12VISpriteTypei(scene, 10, parse[0]) (negative returns -1),
//      Sprite__7VIScenei(scene, handle), and for a non-zero id
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, sprite, id, 19, handle)
//      (negative returns -1);
//   4. it fills the sprite: sprite+44 = 1.0, centre sprite+32..40 = 0, sprite+1072 = extra,
//      sprite+8..16 = -1, 0 - 1, 0 - 1 (the x lane negated) and sprite+20..28 = +1 around the centre;
//   5. when the system address (scene word + 1352) is zero or the flag word at +12 is zero it only
//      skips one sub-object (ReadBegin then ReadEnd, each negative returning -1);
//   6. otherwise it reads a header; if that read succeeded and the tag is 0xc000 the definition
//      handle is the result of ParseParticleDefinitionObj__10VIESFParse(parse), else ReadEnd is
//      called (ignored, only for a successful read with another tag) and the handle is -1. A
//      negative definition handle becomes -1; it is stored in sprite+64 (also when -1) and a
//      negative value returns -1. Then ShareParticleDefinition__16VIParticleSystemi(system, def),
//      ParticleDefinition__16VIParticleSystemi(system, def) for the record,
//      Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(sprite+80, system,
//      record), StaticBounds__C17VIParticleEmitter(&bounds, sprite+80) filling a local 24-byte box
//      (the original passes the result slot as the hidden first argument), which is copied to
//      sprite+8..28, and Init__8VISphereRC6VIBBox(sprite+32, sprite+8);
//   7. finally ReadEnd and the handle (-1 when ReadEnd is negative).
// Inferred: sprite type 10 and resource type 19 mean "particle sprite"; the emitter embedded at
// sprite+80 takes its bounds from the particle definition; the extra int is stored at +1072.
// Unknown: field names, the meaning of the flag at +12 and the exact emitter size.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct ParticleBounds { float v[6]; };

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateSprite__7VIScene12VISpriteTypei(void* scene, int type, int argument);
void* Sprite__7VIScenei(void* scene, int handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
void Init__8VISphereRC6VIBBox(void* sphere, const void* box);
int ParseParticleDefinitionObj__10VIESFParse(void* parse);
int ShareParticleDefinition__16VIParticleSystemi(void* system, int definition);
void* ParticleDefinition__16VIParticleSystemi(void* system, int definition);
void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* emitter, void* system, void* definition);
void StaticBounds__C17VIParticleEmitter(ParticleBounds* result, void* emitter);

int ParseParticleSpriteObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* scene = NORRATH_WORD_ADDRESS(words[6]);
    unsigned short tag;
    unsigned short second;
    int id;
    int extra;
    int type;
    int handle;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    Read__9VIObjFileRi(file, &id);
    if (second != 0) Read__9VIObjFileRi(file, &extra);
    else extra = 0;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    if (id != 0 && Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, static_cast<unsigned>(id), &type, &handle) != 0) {
        if (type != 19) return -1;
        ReadEnd__9VIObjFile(file);
        return handle;
    }
    handle = CreateSprite__7VIScene12VISpriteTypei(scene, 10, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    unsigned char* sprite = static_cast<unsigned char*>(Sprite__7VIScenei(scene, handle));
    if (id != 0 && Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, sprite, static_cast<unsigned>(id), 19, handle) < 0) return -1;
    float* body = reinterpret_cast<float*>(sprite);
    body[11] = 1.0f;
    body[9] = 0.0f;
    body[10] = 0.0f;
    body[8] = 0.0f;
    *reinterpret_cast<int*>(sprite + 1072) = extra;
    body[2] = -body[11];
    body[3] = body[9] - body[11];
    body[4] = body[10] - body[11];
    body[5] = body[8] + body[11];
    body[6] = body[9] + body[11];
    body[7] = body[10] + body[11];
    if (words[6] + 1352 != 0 && words[3] != 0) {
        void* system = NORRATH_WORD_ADDRESS(words[6] + 1352);
        int definition = -1;
        unsigned short definition_tag;
        unsigned short definition_second;
        if (ReadBegin__9VIObjFileRUsT1(file, &definition_tag, &definition_second) >= 0) {
            if (definition_tag == 0xc000) {
                int parsed = ParseParticleDefinitionObj__10VIESFParse(parse);
                if (parsed >= 0) definition = parsed;
            } else {
                ReadEnd__9VIObjFile(file);
            }
        }
        *reinterpret_cast<int*>(sprite + 64) = definition;
        if (definition < 0) return -1;
        ShareParticleDefinition__16VIParticleSystemi(system, definition);
        void* record = ParticleDefinition__16VIParticleSystemi(system, *reinterpret_cast<int*>(sprite + 64));
        Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(sprite + 80, system, record);
        ParticleBounds bounds;
        StaticBounds__C17VIParticleEmitter(&bounds, sprite + 80);
        body[2] = bounds.v[0];
        body[3] = bounds.v[1];
        body[4] = bounds.v[2];
        body[5] = bounds.v[3];
        body[6] = bounds.v[4];
        body[7] = bounds.v[5];
        Init__8VISphereRC6VIBBox(sprite + 32, sprite + 8);
    } else {
        if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
        if (ReadEnd__9VIObjFile(file) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
