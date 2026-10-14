// Observed: BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes at 0x010b3410, 2648 bytes. Arguments: the definition B (this) and an
// attribute block O to fill. Both start with the same 724-byte attribute layout; the definition's motif pool is B+724 (the pool FirstMotif
// and NextMotif walk) and every motif is a pointer to another attribute block.
//  1. All 724 bytes of B are copied to O (the code does this field by field and by 16-byte groups; the pieces cover [0, 724) exactly).
//  2. Walking the motifs in pool order (the first via FirstMotif__C22VIParticleDefinitionEx, each later one via
//     NextMotif__C22VIParticleDefinitionExP15VIParticleMotif; the pool calls in the code are the inlined bodies of those two): let M be
//     the motif and w the float at M+712. A w equal to zero skips the motif. Otherwise, for every float at byte offset k in [0, 684) and for
//     the float at 720, O[k] = O[k] + (M[k] - B[k]) * w, computed as a subtraction, a multiplication and an addition in that order.
//  3. After the walk, for each of the five float triples at 612, 624, 636, 648 and 672 the word 72 bytes further on (684, 688, 692, 696, 700)
//     in O is set to 1 unless all three floats of the triple compare equal to zero, and to 0 when they do. Nothing is returned.
// Inferred: the definition holds base attributes, and each motif contributes a weighted offset from the base (a blend of the particle
// attributes); the flags at 684 to 700 say that the triple (an offset or a pattern vector) is in use. The word at M+712 is a blend weight.
// Unknown: the names of the individual attributes, why the integer words in 684 to 719 other than the five flags are copied but not
// blended, and what the float at 720 is.
extern "C" {
unsigned FirstMotif__C22VIParticleDefinitionEx(const unsigned char* definition);
unsigned NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(const unsigned char* definition, unsigned motif);

#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define MOTIF_OF(word) norrath_host_pointer(word)
#else
#define MOTIF_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

static inline float& At(unsigned char* base, int offset) { return *reinterpret_cast<float*>(base + offset); }

static inline void SetFlag(unsigned char* attributes, int first, int flag) {
    float x = At(attributes, first);
    float y = At(attributes, first + 4);
    float z = At(attributes, first + 8);
    bool allZero = x == 0.0f && y == 0.0f && z == 0.0f;
    *reinterpret_cast<unsigned*>(attributes + flag) = allZero ? 0 : 1;
}

void BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(unsigned char* definition, unsigned char* output) {
    for (int offset = 0; offset < 724; offset += 4)
        *reinterpret_cast<unsigned*>(output + offset) = *reinterpret_cast<unsigned*>(definition + offset);
    for (unsigned word = FirstMotif__C22VIParticleDefinitionEx(definition); word != 0;
         word = NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(definition, word)) {
        unsigned char* motif = MOTIF_OF(word);
        float weight = At(motif, 712);
        if (weight == 0.0f) continue;
        for (int offset = 0; offset < 684; offset += 4)
            At(output, offset) = At(output, offset) + (At(motif, offset) - At(definition, offset)) * weight;
        At(output, 720) = At(output, 720) + (At(motif, 720) - At(definition, 720)) * weight;
    }
    SetFlag(output, 612, 684);
    SetFlag(output, 624, 688);
    SetFlag(output, 636, 692);
    SetFlag(output, 648, 696);
    SetFlag(output, 672, 700);
}
}
