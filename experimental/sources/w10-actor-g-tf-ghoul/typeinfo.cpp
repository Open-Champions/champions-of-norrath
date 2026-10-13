// Observed __tf5Ghoul at 0x0030f110, 80 bytes (weak).
// Returns the 12-byte type_info node __ti5Ghoul. When its first word is zero, calls __tf8Creature
// and then __rtti_si with this node, the class-name text "5Ghoul" and the __ti8Creature node. A
// nonzero first word skips both calls. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Creature is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti5Ghoul[12] __attribute__((aligned(4)));
extern unsigned char __ti8Creature[] __attribute__((aligned(4)));
void* __tf8Creature();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf5Ghoul() {
    if (unset(__ti5Ghoul)) {
        __tf8Creature();
        __rtti_si(__ti5Ghoul, "5Ghoul", __ti8Creature);
    }
    return __ti5Ghoul;
}
