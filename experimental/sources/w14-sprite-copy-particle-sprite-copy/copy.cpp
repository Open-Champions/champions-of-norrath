// Copy__16VIParticleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x010aa4d8, 388 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is
// negative. Otherwise it fetches the new sprite N with Sprite__7VIScenei(scene, id) and copies the 44 bytes S+8..S+51 (eleven floats)
// and the word at S+52 to N. The base-class entry Copy__8VISprite only returns -1.
// Also copies the word at S+1072. Then, only if the word at S+64 is not -1: copies that word to N+64; calls
// Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(N + 80, word at S+160, word at S+164); then SetLocation(N + 80, Location(S + 80)) twice,
// SetDirection(N + 80, Direction(S + 80)) and SetSpace(N + 80, Space(S + 80)) (the image calls the location pair twice); and, when the address scene + 1352 is
// nonzero, calls ShareParticleDefinition__16VIParticleSystemi(scene + 1352, word at S+64). It returns the id. The raster, sound device and collide arguments are
// not used. The accessors Location, Direction and Space return this+16, this+40 and this+96 in the image.
// Inferred: S+80 begins an embedded particle emitter and scene+1352 an embedded particle system; the word at S+64 is a particle-definition id with -1 meaning none.
// Unknown: the emitter's layout beyond the accessors, why the location is set twice, what the two words passed to Create are (they are read at S+160 and S+164, i.e.
// emitter offsets 80 and 84), and the meaning of S+1072.
extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned char* Sprite__7VIScenei(unsigned char* scene, int index);
void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(unsigned char* emitter, unsigned system, unsigned definition);
const unsigned char* Location__C17VIParticleEmitter(const unsigned char* emitter);
void SetLocation__17VIParticleEmitterRC7VIVect3(unsigned char* emitter, const unsigned char* location);
const unsigned char* Direction__C17VIParticleEmitter(const unsigned char* emitter);
void SetDirection__17VIParticleEmitterRC10VIMatrix33(unsigned char* emitter, const unsigned char* direction);
const unsigned char* Space__C17VIParticleEmitter(const unsigned char* emitter);
void SetSpace__17VIParticleEmitterRC10VIMatrix44(unsigned char* emitter, const unsigned char* space);
void ShareParticleDefinition__16VIParticleSystemi(unsigned char* system, int definition);
}

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

extern "C" int Copy__16VIParticleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char*, unsigned char*, unsigned char*, int copyArgument) {
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = Sprite__7VIScenei(scene, id);
    *reinterpret_cast<int*>(copy + 1072) = *reinterpret_cast<int*>(self + 1072);
    copy_base(copy, self);
    int definition = *reinterpret_cast<int*>(self + 64);
    if (definition != -1) {
        unsigned char* emitter = copy + 80;
        const unsigned char* source = self + 80;
        *reinterpret_cast<int*>(copy + 64) = definition;
        Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(emitter, *reinterpret_cast<const unsigned*>(source + 80), *reinterpret_cast<const unsigned*>(source + 84));
        SetLocation__17VIParticleEmitterRC7VIVect3(emitter, Location__C17VIParticleEmitter(source));
        SetLocation__17VIParticleEmitterRC7VIVect3(emitter, Location__C17VIParticleEmitter(source));
        SetDirection__17VIParticleEmitterRC10VIMatrix33(emitter, Direction__C17VIParticleEmitter(source));
        SetSpace__17VIParticleEmitterRC10VIMatrix44(emitter, Space__C17VIParticleEmitter(source));
        unsigned char* system = scene + 1352;
        if (system != 0) ShareParticleDefinition__16VIParticleSystemi(system, *reinterpret_cast<int*>(self + 64));
    }
    return id;
}
