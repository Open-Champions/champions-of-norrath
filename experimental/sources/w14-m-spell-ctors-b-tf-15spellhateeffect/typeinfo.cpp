// Observed __tf15SpellHateEffect at 0x003146b8, 76 bytes (weak).
// Returns the 12-byte node __ti15SpellHateEffect. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 15SpellHateEffect. The innermost level calls __tf5Spell when unset, then registers with __rtti_si
// against __ti5Spell; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti15SpellHateEffect[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void* __tf5Spell();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf15SpellHateEffect() {
    if (unset(__ti15SpellHateEffect)) {
        __tf5Spell();
        __rtti_si(__ti15SpellHateEffect, "15SpellHateEffect", __ti5Spell);
    }
    return __ti15SpellHateEffect;
}
