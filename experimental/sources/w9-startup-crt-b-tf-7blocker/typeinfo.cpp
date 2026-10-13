// Observed __tf7Blocker at 0x0030e608, 76 bytes (weak).
// Returns the 12-byte type_info node __ti7Blocker. Only the first word of a node gates
// initialization. When the outer node is zero:
//   if empty: call __tf10GameObject, then __rtti_si(node, "7Blocker", __ti10GameObject).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti7Blocker[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf7Blocker() {
    if (unset(__ti7Blocker)) {
        __tf10GameObject();
        __rtti_si(__ti7Blocker, "7Blocker", __ti10GameObject);
    }
    return __ti7Blocker;
}
