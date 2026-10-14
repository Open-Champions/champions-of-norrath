// Observed: CreateParticleEmitters__9VICSpriteR7VIScene at 0x010b01b8, 200 bytes. Arguments (this, scene). The array at this+4616 has a
// count at +0 and an element pointer at +4; its elements are 1056 bytes. For each index i from 0 while i < the count (re-read each
// pass): let e be element i; when the signed word at e+0 is -1 the element is skipped; otherwise it calls
// ParticleDefinition__16VIParticleSystemi(scene+1352, e[0]) and then Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition
// (e+16, scene+1352, definition); a negative result from Create returns -1 at once. It returns zero when the loop ends.
// Inferred: scene+1352 is the particle system embedded in the scene, e[0] is a particle definition id (-1 for none) and e+16 is an
// embedded emitter object that Create initialises; the emitter at e+16 is the only part of the element touched here.
// Unknown: the rest of the 1056-byte element and the definition's type beyond the pointer.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void* ParticleDefinition__16VIParticleSystemi(void* system, int id);
int Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(void* emitter, void* system, void* definition);

int CreateParticleEmitters__9VICSpriteR7VIScene(void* object, void* scene) {
    unsigned* array = reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + 4616);
    void* system = static_cast<unsigned char*>(scene) + 1352;
    for (int i = 0; i < static_cast<int>(array[0]); ++i) {
        unsigned char* element = NORRATH_WORD_TO_POINTER(array[1]) + i * 1056;
        int id = *reinterpret_cast<int*>(element);
        if (id == -1) continue;
        void* definition = ParticleDefinition__16VIParticleSystemi(system, id);
        if (Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(NORRATH_WORD_TO_POINTER(array[1]) + i * 1056 + 16, system, definition) < 0)
            return -1;
    }
    return 0;
}
}
