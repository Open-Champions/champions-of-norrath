// Observed __tf4Boat at 0x0031a1a0, 80 bytes (weak).
// Returns the 12-byte type_info node __ti4Boat. Only the first word of a node gates
// initialization. When the outer node is zero:
//   if empty: call __tf7Vehicle, then __rtti_si(node, "4Boat", __ti7Vehicle).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti4Boat[12] __attribute__((aligned(4)));
extern unsigned char __ti7Vehicle[] __attribute__((aligned(4)));
void* __tf7Vehicle();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf4Boat() {
    if (unset(__ti4Boat)) {
        __tf7Vehicle();
        __rtti_si(__ti4Boat, "4Boat", __ti7Vehicle);
    }
    return __ti4Boat;
}
