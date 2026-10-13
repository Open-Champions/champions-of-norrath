// Observed __tf11AddParticle at 0x00315000, 76 bytes (weak).
// Returns the 12-byte type_info node __ti11AddParticle. Only the first word of the node gates
// initialization. When it is zero: __tf10GameObject is called (the base class's own accessor), then
// __rtti_si(node, "11AddParticle", __ti10GameObject). A nonzero first word skips both calls. The node is
// returned either way. __rtti_si takes the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti11AddParticle[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf11AddParticle() {
    if (unset(__ti11AddParticle)) {
        __tf10GameObject();
        __rtti_si(__ti11AddParticle, "11AddParticle", __ti10GameObject);
    }
    return __ti11AddParticle;
}
