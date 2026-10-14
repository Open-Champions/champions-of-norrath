// Observed __tf12GlowingShaft, 76 bytes at 0x00310788 in the reference image. It returns the node __ti12GlowingShaft (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __tf10GameObject, then registers
// "12GlowingShaft" with __rtti_si against __ti10GameObject.
// Inferred: single-base chain, from the same call shape as the other single-base getters.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti12GlowingShaft[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf12GlowingShaft() {
    if (unset(__ti12GlowingShaft)) {
        __tf10GameObject();
        __rtti_si(__ti12GlowingShaft, "12GlowingShaft", __ti10GameObject);
    }
    return __ti12GlowingShaft;
}
