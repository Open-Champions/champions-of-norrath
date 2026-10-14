// Observed __tf7AntTrap at 0x00315340, 80 bytes (weak).
// Returns the 12-byte type_info node __ti7AntTrap. Only the first word of a node gates
// initialization. When the outer node is zero:
//   if empty: call __tf8AutoProp, then __rtti_si(node, "7AntTrap", __ti8AutoProp).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti7AntTrap[12] __attribute__((aligned(4)));
extern unsigned char __ti8AutoProp[] __attribute__((aligned(4)));
void* __tf8AutoProp();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf7AntTrap() {
    if (unset(__ti7AntTrap)) {
        __tf8AutoProp();
        __rtti_si(__ti7AntTrap, "7AntTrap", __ti8AutoProp);
    }
    return __ti7AntTrap;
}
