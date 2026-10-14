// Observed __tf16PoisonProjectile at 0x00314b60, 80 bytes (weak).
// Returns the 12-byte node __ti16PoisonProjectile. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 16PoisonProjectile. The innermost level calls __tf10Projectile when unset, then registers with __rtti_si
// against __ti10Projectile; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti16PoisonProjectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
void* __tf10Projectile();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf16PoisonProjectile() {
    if (unset(__ti16PoisonProjectile)) {
        __tf10Projectile();
        __rtti_si(__ti16PoisonProjectile, "16PoisonProjectile", __ti10Projectile);
    }
    return __ti16PoisonProjectile;
}
