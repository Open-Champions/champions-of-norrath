// Observed __tf11MaleDarkElf at 0x00316938, 76 bytes (weak).
// Returns the 12-byte type_info node __ti11MaleDarkElf. When its first word is zero, calls __tf8Creature
// and then __rtti_si with this node, the class-name text "11MaleDarkElf" and the __ti8Creature node. A
// nonzero first word skips both calls. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Creature is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti11MaleDarkElf[12] __attribute__((aligned(4)));
extern unsigned char __ti8Creature[] __attribute__((aligned(4)));
void* __tf8Creature();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf11MaleDarkElf() {
    if (unset(__ti11MaleDarkElf)) {
        __tf8Creature();
        __rtti_si(__ti11MaleDarkElf, "11MaleDarkElf", __ti8Creature);
    }
    return __ti11MaleDarkElf;
}
