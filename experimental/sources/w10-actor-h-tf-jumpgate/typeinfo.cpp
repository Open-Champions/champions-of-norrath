// Observed __tf8JumpGate at 0x003120b0, 76 bytes (weak).
// Returns the 12-byte type_info node __ti8JumpGate. When its first word is zero, calls __tf10GameObject
// and then __rtti_si with this node, the class-name text "8JumpGate" and the __ti10GameObject node. A
// nonzero first word skips both calls. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and GameObject is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti8JumpGate[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf8JumpGate() {
    if (unset(__ti8JumpGate)) {
        __tf10GameObject();
        __rtti_si(__ti8JumpGate, "8JumpGate", __ti10GameObject);
    }
    return __ti8JumpGate;
}
