// Observed __tf19BlindingLightEffect, 76 bytes at 0x00319cd0 in the reference image. It returns the node __ti19BlindingLightEffect (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __tf10GameObject, then registers
// "19BlindingLightEffect" with __rtti_si against __ti10GameObject.
// Inferred: single-base chain, from the same call shape as the other single-base getters.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti19BlindingLightEffect[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf19BlindingLightEffect() {
    if (unset(__ti19BlindingLightEffect)) {
        __tf10GameObject();
        __rtti_si(__ti19BlindingLightEffect, "19BlindingLightEffect", __ti10GameObject);
    }
    return __ti19BlindingLightEffect;
}
