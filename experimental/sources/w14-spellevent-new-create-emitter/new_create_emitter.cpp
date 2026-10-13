// Observed: NewCreateEmitter__18VISpellEventDriverP12VISpellEvent at 0x0117d700 is 208 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Always returns 0 (the image loads zero into the
// result register at the end; no caller here reads it).
//   1. a = __builtin_new(184); when non-null it calls __18VISpellActiveEventP12VISpellEventP7VIScene
//      (a, record, the word at this+20).
//   2. Stores the 64-bit result of GetTime__7VIScene(the word at this+20) at a+176 (the store happens
//      even when a is null).
//   3. e = __builtin_new(992); when non-null it calls __17VIParticleEmitter(e).
//   4. sys = the word at this+20 plus 1352. Stores e at a+120. def = ParticleDefinition__16
//      VIParticleSystemi(sys, the int at record+8). Then it calls Create__17VIParticleEmitterP16
//      VIParticleSystemP20VIParticleDefinition(the word at a+120, sys, def) (also when null), then
//      AddTail__7VILListP7VILNode(this+76, a), and finally stores 0.0f at a+156 and a+160.
// Inferred: record type 3 of the event table (the driver's Process routine dispatches it here): it
// makes an active event with a particle emitter from the particle definition numbered by record+8 and
// links it into the driver's list at +76. 1352 is the offset of the particle system inside the scene.
// Unknown: what the two floats at +156/+160 are, the return value's meaning, and the particle
// definition numbering. Nothing here ran on the EE.
extern "C" {
void* __builtin_new(unsigned size);
void __18VISpellActiveEventP12VISpellEventP7VIScene(void* self, void* event, void* scene);
long long GetTime__7VIScene(void* scene);
void __17VIParticleEmitter(void* emitter);
void* ParticleDefinition__16VIParticleSystemi(void* system, int index);
void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* emitter, void* system, void* definition);
void AddTail__7VILListP7VILNode(void* list, void* node);

int NewCreateEmitter__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    unsigned char* a = static_cast<unsigned char*>(__builtin_new(184));
    if (a != 0) {
        __18VISpellActiveEventP12VISpellEventP7VIScene(a, record, *reinterpret_cast<void**>(self + 20));
    }
    *reinterpret_cast<long long*>(a + 176) = GetTime__7VIScene(*reinterpret_cast<void**>(self + 20));
    void* emitter = __builtin_new(992);
    if (emitter != 0) {
        __17VIParticleEmitter(emitter);
    }
    unsigned char* system = *reinterpret_cast<unsigned char**>(self + 20) + 1352;
    *reinterpret_cast<void**>(a + 120) = emitter;
    void* definition = ParticleDefinition__16VIParticleSystemi(system, *reinterpret_cast<int*>(record + 8));
    Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(*reinterpret_cast<void**>(a + 120), system, definition);
    AddTail__7VILListP7VILNode(self + 76, a);
    *reinterpret_cast<float*>(a + 156) = 0.0f;
    *reinterpret_cast<float*>(a + 160) = 0.0f;
    return 0;
}
}
