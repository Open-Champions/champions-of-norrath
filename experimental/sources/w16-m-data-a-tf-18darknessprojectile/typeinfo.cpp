// Observed __tf18DarknessProjectile, 76 bytes at 0x00317348 in the reference image. It returns the node __ti18DarknessProjectile (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __tf10GameObject, then registers
// "18DarknessProjectile" with __rtti_si against __ti10GameObject.
// Inferred: single-base chain, from the same call shape as the other single-base getters.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti18DarknessProjectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf18DarknessProjectile() {
    if (unset(__ti18DarknessProjectile)) {
        __tf10GameObject();
        __rtti_si(__ti18DarknessProjectile, "18DarknessProjectile", __ti10GameObject);
    }
    return __ti18DarknessProjectile;
}
