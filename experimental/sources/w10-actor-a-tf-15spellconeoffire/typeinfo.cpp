// Observed __tf15SpellConeOfFire at 0x00319918, 112 bytes (weak).
// Returns the 12-byte node __ti15SpellConeOfFire. When its first word is zero it first makes sure
// the 8-byte node __ti5Spell is set up, doing inline what __tf5Spell does (when that node's first
// word is zero, __rtti_user(node, "5Spell")), and then calls __rtti_si with the outer node, the
// class-name text "15SpellConeOfFire" and the __ti5Spell node. A nonzero first word on the outer
// node skips everything. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Spell is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti15SpellConeOfFire[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf15SpellConeOfFire() {
    if (unset(__ti15SpellConeOfFire)) {
        if (unset(__ti5Spell)) __rtti_user(__ti5Spell, "5Spell");
        __rtti_si(__ti15SpellConeOfFire, "15SpellConeOfFire", __ti5Spell);
    }
    return __ti15SpellConeOfFire;
}
