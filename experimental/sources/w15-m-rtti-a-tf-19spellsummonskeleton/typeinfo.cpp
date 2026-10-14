// Observed __tf19SpellSummonSkeleton at 0x00318950, 112 bytes. It returns the node __ti19SpellSummonSkeleton
// (0x008eba58). A nonzero first word returns it at once. When that word is zero the function first registers the
// base node __ti5Spell (0x00486dd8) with __rtti_user when that node is unset, under the name "5Spell". It then
// registers "19SpellSummonSkeleton" with __rtti_si against __ti5Spell.
// Inferred: the base node is inlined here rather than reached through __tf5Spell (the getter's own call is absent).
// Unknown: the exact base name string contents beyond the names above.
extern "C" {
extern unsigned char __ti19SpellSummonSkeleton[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf19SpellSummonSkeleton() {
    if (unset(__ti19SpellSummonSkeleton)) {
        if (unset(__ti5Spell)) __rtti_user(__ti5Spell, "5Spell");
        __rtti_si(__ti19SpellSummonSkeleton, "19SpellSummonSkeleton", __ti5Spell);
    }
    return __ti19SpellSummonSkeleton;
}
