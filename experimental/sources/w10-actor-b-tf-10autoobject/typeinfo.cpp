// Observed __tf10AutoObject at 0x314d98, 76 bytes (weak).
// Returns the 12-byte type_info node __ti10AutoObject (the node itself is defined with the other inline-initialised nodes).
// Only the first word of a node gates initialization. When the node is zero: call __tf10GameObject, then __rtti_si(node, "10AutoObject", __ti10GameObject).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
extern "C" {
extern unsigned char __ti10AutoObject[] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10AutoObject() {
    if (unset(__ti10AutoObject)) {
        __tf10GameObject();
        __rtti_si(__ti10AutoObject, "10AutoObject", __ti10GameObject);
    }
    return __ti10AutoObject;
}
