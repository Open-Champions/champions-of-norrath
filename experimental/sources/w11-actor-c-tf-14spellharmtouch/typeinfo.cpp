// Observed __tf14SpellHarmTouch at 0x003178a8, 76 bytes (weak).
// Returns the 12-byte type_info node __ti14SpellHarmTouch (a zero-initialized object in BSS). When its
// first word is zero, calls __tf5Spell and then __rtti_si with this node, the class-name text
// "14SpellHarmTouch" and the __ti5Spell node. A nonzero first word skips both calls. The node is
// returned either way.
// Inferred: the node is the class's runtime type descriptor and Spell is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti14SpellHarmTouch[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void* __tf5Spell();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf14SpellHarmTouch() {
    if (unset(__ti14SpellHarmTouch)) {
        __tf5Spell();
        __rtti_si(__ti14SpellHarmTouch, "14SpellHarmTouch", __ti5Spell);
    }
    return __ti14SpellHarmTouch;
}
