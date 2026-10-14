// Observed __tf19ParticleMorphEffect at 0x00313150, 76 bytes (weak).
// Returns the 12-byte type_info node __ti19ParticleMorphEffect. Only the first word of the node
// gates initialization. When it is zero: __tf10GameObject is called, then
// __rtti_si(node, "19ParticleMorphEffect", __ti10GameObject). A nonzero first word skips both
// calls. The node is returned either way. __rtti_si takes the node, the class-name text and
// the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti19ParticleMorphEffect[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf19ParticleMorphEffect() {
    if (*reinterpret_cast<const AliasedWord*>(__ti19ParticleMorphEffect) == 0u) {
        __tf10GameObject();
        __rtti_si(__ti19ParticleMorphEffect, "19ParticleMorphEffect", __ti10GameObject);
    }
    return __ti19ParticleMorphEffect;
}
