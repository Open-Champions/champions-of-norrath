// Observed __tf10FrostShard, 80 bytes at 0x00319f10 in the reference image. It returns the node __ti10FrostShard (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __tf10Projectile, then registers
// "10FrostShard" with __rtti_si against __ti10Projectile.
// Inferred: single-base chain, from the same call shape as the other single-base getters.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti10FrostShard[12] __attribute__((aligned(4)));
extern unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
void* __tf10Projectile();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10FrostShard() {
    if (unset(__ti10FrostShard)) {
        __tf10Projectile();
        __rtti_si(__ti10FrostShard, "10FrostShard", __ti10Projectile);
    }
    return __ti10FrostShard;
}
