// Observed __tf8LostSoul, 132 bytes at 0x00310eb0 in the reference image. It returns the node __ti8LostSoul (12 bytes).
// When that node's first word is zero, the function first checks the Projectile node __ti10Projectile. If that word is zero it
// calls __tf10GameObject, then registers "10Projectile" with __rtti_si against __ti10GameObject. It then registers "8LostSoul"
// with __rtti_si against __ti10Projectile. A nonzero first word on either node skips its own registration.
// Inferred: three-level chain (GameObject -> 10Projectile -> 8LostSoul) from the registration calls; the calls are inline in the image.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti8LostSoul[12] __attribute__((aligned(4)));
extern unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf8LostSoul() {
    if (unset(__ti8LostSoul)) {
        if (unset(__ti10Projectile)) {
            __tf10GameObject();
            __rtti_si(__ti10Projectile, "10Projectile", __ti10GameObject);
        }
        __rtti_si(__ti8LostSoul, "8LostSoul", __ti10Projectile);
    }
    return __ti8LostSoul;
}
