// Observed __tf19SpellHornOmenEffect at 0x0031a900, 76 bytes (weak).
// Returns the 12-byte node __ti19SpellHornOmenEffect. The first word of a node gates its initialisation.
// When unset: calls __tf5Spell, then __rtti_si(this node, "19SpellHornOmenEffect", __ti5Spell). A set first word skips both.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti19SpellHornOmenEffect[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void* __tf5Spell();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf19SpellHornOmenEffect() {
    if (unset(__ti19SpellHornOmenEffect)) {
        __tf5Spell();
        __rtti_si(__ti19SpellHornOmenEffect, "19SpellHornOmenEffect", __ti5Spell);
    }
    return __ti19SpellHornOmenEffect;
}
