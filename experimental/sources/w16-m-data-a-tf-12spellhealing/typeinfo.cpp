// Observed __tf12SpellHealing, 112 bytes at 0x00319270 in the reference image. It returns the node __ti12SpellHealing (12 bytes).
// A nonzero first word returns the node at once. When it is zero, the base node __ti5Spell is registered first with
// __rtti_user under "5Spell" if its first word is zero; then the node is registered with __rtti_si as "12SpellHealing"
// against __ti5Spell.
// Inferred: two-level chain (5Spell -> 12SpellHealing), with the root registered inline rather than through a getter call.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti12SpellHealing[12] __attribute__((aligned(4)));
extern unsigned char __ti5Spell[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf12SpellHealing() {
    if (unset(__ti12SpellHealing)) {
        if (unset(__ti5Spell)) __rtti_user(__ti5Spell, "5Spell");
        __rtti_si(__ti12SpellHealing, "12SpellHealing", __ti5Spell);
    }
    return __ti12SpellHealing;
}
