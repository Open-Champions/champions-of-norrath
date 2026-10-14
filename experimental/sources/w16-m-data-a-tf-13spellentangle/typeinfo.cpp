// Observed __tf13SpellEntangle, 76 bytes at 0x0031a588 in the reference image. It returns the node __ti13SpellEntangle (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __tf5Spell, then registers
// "13SpellEntangle" with __rtti_si against __ti5Spell.
// Inferred: single-base chain, from the same call shape as the other single-base getters.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti13SpellEntangle[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void* __tf5Spell();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf13SpellEntangle() {
    if (unset(__ti13SpellEntangle)) {
        __tf5Spell();
        __rtti_si(__ti13SpellEntangle, "13SpellEntangle", __ti5Spell);
    }
    return __ti13SpellEntangle;
}
