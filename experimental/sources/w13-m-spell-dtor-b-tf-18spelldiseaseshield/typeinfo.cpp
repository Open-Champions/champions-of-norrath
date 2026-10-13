// Observed __tf18SpellDiseaseShield at 0x00316fe0, 112 bytes (weak).
// Returns the 12-byte node __ti18SpellDiseaseShield. The first word of a node gates its initialisation; a set node skips all work.
// When unset: if __ti5Spell is also unset it first gets __rtti_user(__ti5Spell, "5Spell") (done inline), then
// __rtti_si(this node, "18SpellDiseaseShield", __ti5Spell).
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti18SpellDiseaseShield[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf18SpellDiseaseShield() {
    if (unset(__ti18SpellDiseaseShield)) {
        if (unset(__ti5Spell)) __rtti_user(__ti5Spell, "5Spell");
        __rtti_si(__ti18SpellDiseaseShield, "18SpellDiseaseShield", __ti5Spell);
    }
    return __ti18SpellDiseaseShield;
}
