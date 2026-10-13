// Observed __tf13MissileWeapon at 0x00311228, 132 bytes (weak). The 12-byte typeinfo object __ti13MissileWeapon
// is zero in the file image and is defined here. Two nested levels (outer to inner) 13MissileWeapon >
// 10Projectile, base 10GameObject; only the first word of a node gates its level. When __ti13MissileWeapon is
// zero: if __ti10Projectile is zero, __tf10GameObject runs and __rtti_si registers "10Projectile" on
// __ti10GameObject; then "13MissileWeapon" is registered on __ti10Projectile. A nonzero first word at a level
// skips that level's registration. The object is returned either way. __ti10Projectile is owned by another
// source. Inferred: the base chain, from the registration calls alone. Nothing here ran on the EE.
extern "C" {
unsigned char __ti13MissileWeapon[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[];
extern unsigned char __ti10Projectile[];
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf13MissileWeapon() {
    if (unset(__ti13MissileWeapon)) {
        if (unset(__ti10Projectile)) {
            __tf10GameObject();
            __rtti_si(__ti10Projectile, "10Projectile", __ti10GameObject);
        }
        __rtti_si(__ti13MissileWeapon, "13MissileWeapon", __ti10Projectile);
    }
    return __ti13MissileWeapon;
}
