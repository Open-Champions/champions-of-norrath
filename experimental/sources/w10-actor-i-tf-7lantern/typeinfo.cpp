// Observed __tf7Lantern at 0x00310320, 76 bytes (weak).
// Returns the 12-byte type_info node __ti7Lantern. When its first word is zero, calls __tf10GameObject
// and then __rtti_si with this node, the class-name text "7Lantern" and the __ti10GameObject node. A
// nonzero first word skips both calls. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and GameObject is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti7Lantern[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf7Lantern() {
    if (unset(__ti7Lantern)) {
        __tf10GameObject();
        __rtti_si(__ti7Lantern, "7Lantern", __ti10GameObject);
    }
    return __ti7Lantern;
}
