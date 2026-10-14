// Observed __tf5Spell at 0x00314950, 60 bytes (weak).
// Returns the 8-byte node __ti5Spell. When its first word is zero it calls __rtti_user once with the node and
// the class-name text "5Spell"; a nonzero first word makes no call. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor, from the symbol names, and Spell has no base in this
// function. Nothing here was executed on the EE.
extern "C" {
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf5Spell() {
    if (unset(__ti5Spell)) __rtti_user(__ti5Spell, "5Spell");
    return __ti5Spell;
}
