// Observed __tf16beetleProjectile at 0x00311560, 176 bytes (weak).
// Returns the 12-byte node __ti16beetleProjectile. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 16beetleProjectile > 13MissileWeapon > 10Projectile. The innermost level calls __tf10GameObject when unset, then registers with __rtti_si
// against __ti10GameObject; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti16beetleProjectile[12] __attribute__((aligned(4)));
extern unsigned char __ti13MissileWeapon[12] __attribute__((aligned(4)));
extern unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf16beetleProjectile() {
    if (unset(__ti16beetleProjectile)) {
        if (unset(__ti13MissileWeapon)) {
            if (unset(__ti10Projectile)) {
                __tf10GameObject();
                __rtti_si(__ti10Projectile, "10Projectile", __ti10GameObject);
            }
            __rtti_si(__ti13MissileWeapon, "13MissileWeapon", __ti10Projectile);
        }
        __rtti_si(__ti16beetleProjectile, "16beetleProjectile", __ti13MissileWeapon);
    }
    return __ti16beetleProjectile;
}
