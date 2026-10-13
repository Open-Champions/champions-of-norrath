// Observed __tf4Lamp at 0x003103e8, 76 bytes (weak).
// Returns the 12-byte type_info node __ti4Lamp. When its first word is zero, calls __tf10GameObject
// and then __rtti_si with this node, the class-name text "4Lamp" and the __ti10GameObject node. A
// nonzero first word skips both calls. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and GameObject is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti4Lamp[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf4Lamp() {
    if (unset(__ti4Lamp)) {
        __tf10GameObject();
        __rtti_si(__ti4Lamp, "4Lamp", __ti10GameObject);
    }
    return __ti4Lamp;
}
