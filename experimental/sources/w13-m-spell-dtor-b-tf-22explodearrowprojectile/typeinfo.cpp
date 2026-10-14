// Observed __tf22ExplodeArrowProjectile at 0x00318170, 80 bytes (weak).
// Returns the 12-byte node __ti22ExplodeArrowProjectile. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 22ExplodeArrowProjectile. The innermost level calls __tf10Projectile when unset, then registers with __rtti_si
// against __ti10Projectile; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti22ExplodeArrowProjectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
void* __tf10Projectile();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf22ExplodeArrowProjectile() {
    if (unset(__ti22ExplodeArrowProjectile)) {
        __tf10Projectile();
        __rtti_si(__ti22ExplodeArrowProjectile, "22ExplodeArrowProjectile", __ti10Projectile);
    }
    return __ti22ExplodeArrowProjectile;
}
