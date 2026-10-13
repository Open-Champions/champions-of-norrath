// Observed __tf14HateProjectile at 0x00314900, 76 bytes (weak).
// Returns the 12-byte node __ti14HateProjectile. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 14HateProjectile. The innermost level calls __tf10GameObject when unset, then registers with __rtti_si
// against __ti10GameObject; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti14HateProjectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf14HateProjectile() {
    if (unset(__ti14HateProjectile)) {
        __tf10GameObject();
        __rtti_si(__ti14HateProjectile, "14HateProjectile", __ti10GameObject);
    }
    return __ti14HateProjectile;
}
