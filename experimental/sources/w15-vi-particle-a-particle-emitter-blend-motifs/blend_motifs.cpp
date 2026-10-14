// Observed: BlendMotifs__17VIParticleEmitter at 0x010b7218 is 236 bytes. E is the emitter and D = the word at E+84 (the definition).
//  1. For every motif M from FirstMotif__C22VIParticleDefinitionEx(D), then NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(D, M)
//     until it returns null, it stores zero in the word at M+712.
//  2. N starts as the word at E+176 (a node). While the word at N+0 (its next link) is non-zero, N is processed and then replaced by
//     that link, so the last node, whose link is zero, is never processed. Processing: if the word at N+24 is non-zero it is cleared and
//     a changed flag is set. With M the word at N+16, the float at N+20 is stored in M+712; when that float is not equal to 0.0f the
//     word at M+716 is added to a running total T.
//  3. The word at D+716 is added to T. If the changed flag is clear and T equals the word at E+912 the routine returns. Otherwise it
//     calls BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(D, E + 188), then RecomputeStaticBBox__17VIParticleEmitter(E),
//     and stores T in the word at E+912.
// Nothing is returned.
// Inferred: the nodes at E+176 are motif blenders (weight at +20, motif at +16, changed flag at +24); the total at +912 caches the sum so
// the blend is skipped while nothing changed; the attributes written by the definition's blend live at E+188.
// Unknown: what M+716 and D+716 are, and the chain type of the node list (the sentinel reading is from the loop shape).
extern "C" {
void* FirstMotif__C22VIParticleDefinitionEx(void* definition);
void* NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(void* definition, void* motif);
void BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(void* definition, void* attributes);
void RecomputeStaticBBox__17VIParticleEmitter(void* emitter);

#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define POINTER_OF(word) norrath_host_pointer(word)
#else
#define POINTER_OF(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void BlendMotifs__17VIParticleEmitter(unsigned char* emitter) {
    int total = 0;
    int changed = 0;
    unsigned char* definition = POINTER_OF(*reinterpret_cast<unsigned*>(emitter + 84));
    unsigned char* motif = static_cast<unsigned char*>(FirstMotif__C22VIParticleDefinitionEx(definition));
    while (motif != 0) {
        *reinterpret_cast<unsigned*>(motif + 712) = 0;
        motif = static_cast<unsigned char*>(NextMotif__C22VIParticleDefinitionExP15VIParticleMotif(POINTER_OF(*reinterpret_cast<unsigned*>(emitter + 84)), motif));
    }
    unsigned char* node = POINTER_OF(*reinterpret_cast<unsigned*>(emitter + 176));
    unsigned next = *reinterpret_cast<unsigned*>(node);
    while (next != 0) {
        if (*reinterpret_cast<unsigned*>(node + 24) != 0) {
            *reinterpret_cast<unsigned*>(node + 24) = 0;
            changed = 1;
        }
        unsigned char* target = POINTER_OF(*reinterpret_cast<unsigned*>(node + 16));
        *reinterpret_cast<float*>(target + 712) = *reinterpret_cast<float*>(node + 20);
        if (*reinterpret_cast<float*>(node + 20) != 0.0f) total += *reinterpret_cast<int*>(target + 716);
        node = POINTER_OF(next);
        next = *reinterpret_cast<unsigned*>(node);
    }
    definition = POINTER_OF(*reinterpret_cast<unsigned*>(emitter + 84));
    total += *reinterpret_cast<int*>(definition + 716);
    if (changed != 0 || total != *reinterpret_cast<int*>(emitter + 912)) {
        BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes(definition, emitter + 188);
        RecomputeStaticBBox__17VIParticleEmitter(emitter);
        *reinterpret_cast<int*>(emitter + 912) = total;
    }
}
}
