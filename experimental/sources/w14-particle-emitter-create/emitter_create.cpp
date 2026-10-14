// Observed: Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition at
// 0x010b61c8 is 792 bytes. In order it:
//   1. calls Destroy__17VIParticleEmitter(emitter);
//   2. stores the system pointer at emitter + 80 and the definition pointer at emitter + 84;
//   3. stores zero at emitter + 912, + 172 and + 76, then calls Identity__10VIMatrix33 on
//      emitter + 40 and Identity__10VIMatrix44 on emitter + 96;
//   4. stores 0.0f in the three floats at emitter + 16 and the three at emitter + 28, and
//      zero in the words at + 980 and + 984;
//   5. calls AddTail__7VILListP7VILNode(system + 52, emitter) (the system is re-read from
//      emitter + 80);
//   6. copies bytes 0..723 of the definition to emitter + 188 (724 bytes: the whole
//      attribute block, including its counter word at 716 and float at 720);
//   7. calls RecomputeStaticBBox__17VIParticleEmitter(emitter);
//   8. stores 0.0f in the three floats at emitter + 940 and the three at emitter + 952;
//   9. returns zero.
// Inferred: the block copied in step 6 is the VIParticleAttributes sub-object of the
// definition (its setters in this package write fields below offset 724, and
// VIParticleDefinitionEx's motif pool starts at 724). The emitter links itself into the
// list at system + 52. Names come from symbols; the emitter's other fields are untouched.
// Unknown: what the words at 76, 172 (see ParticleCount__C17VIParticleEmitter), 912, 984 and
// the float triples at 940 and 952 are for, and the meaning of the return value.
extern "C" {
void Destroy__17VIParticleEmitter(void* emitter);
void Identity__10VIMatrix33(void* matrix);
void Identity__10VIMatrix44(void* matrix);
void AddTail__7VILListP7VILNode(void* list, void* node);
void RecomputeStaticBBox__17VIParticleEmitter(void* emitter);

// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep pointers in
// 4-byte words; the words then hold arena offsets.
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
unsigned norrath_host_word(const void* pointer);
#define POINTER_OF(word) norrath_host_pointer(word)
#define WORD_OF(pointer) norrath_host_word(pointer)
#else
#define POINTER_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define WORD_OF(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

int Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(
        unsigned char* emitter, unsigned char* system, const unsigned char* definition) {
    Destroy__17VIParticleEmitter(emitter);
    *reinterpret_cast<unsigned*>(emitter + 80) = WORD_OF(system);
    *reinterpret_cast<unsigned*>(emitter + 84) = WORD_OF(definition);
    *reinterpret_cast<unsigned*>(emitter + 912) = 0;
    *reinterpret_cast<unsigned*>(emitter + 172) = 0;
    *reinterpret_cast<unsigned*>(emitter + 76) = 0;
    Identity__10VIMatrix33(emitter + 40);
    Identity__10VIMatrix44(emitter + 96);
    float* location = reinterpret_cast<float*>(emitter + 16);
    float* last = reinterpret_cast<float*>(emitter + 28);
    location[0] = 0.0f;
    location[1] = 0.0f;
    location[2] = 0.0f;
    last[0] = 0.0f;
    last[1] = 0.0f;
    last[2] = 0.0f;
    *reinterpret_cast<unsigned*>(emitter + 980) = 0;
    *reinterpret_cast<unsigned*>(emitter + 984) = 0;
    AddTail__7VILListP7VILNode(POINTER_OF(*reinterpret_cast<unsigned*>(emitter + 80)) + 52, emitter);
    __builtin_memcpy(emitter + 188, definition, 724);
    RecomputeStaticBBox__17VIParticleEmitter(emitter);
    float* bounds_a = reinterpret_cast<float*>(emitter + 940);
    float* bounds_b = reinterpret_cast<float*>(emitter + 952);
    bounds_a[0] = 0.0f;
    bounds_a[1] = 0.0f;
    bounds_a[2] = 0.0f;
    bounds_b[0] = 0.0f;
    bounds_b[1] = 0.0f;
    bounds_b[2] = 0.0f;
    return 0;
}
}
