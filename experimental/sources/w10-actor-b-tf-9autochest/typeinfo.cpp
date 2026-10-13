// Observed __tf9AutoChest at 0x314e68, 132 bytes (weak).
// Returns the 12-byte type_info node __ti9AutoChest (the node itself is defined with the other inline-initialised nodes).
// Only the first word of a node gates initialization. Levels 9AutoChest > 10AutoObject (outer to inner), base 10GameObject. The innermost empty level first calls __tf10GameObject, then each empty level, innermost first, gets __rtti_si(level, its name text, the next level inward; the innermost uses __ti10GameObject).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
extern "C" {
extern unsigned char __ti9AutoChest[] __attribute__((aligned(4)));
extern unsigned char __ti10AutoObject[] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf9AutoChest() {
    if (unset(__ti9AutoChest)) {
        if (unset(__ti10AutoObject)) {
            __tf10GameObject();
            __rtti_si(__ti10AutoObject, "10AutoObject", __ti10GameObject);
        }
        __rtti_si(__ti9AutoChest, "9AutoChest", __ti10AutoObject);
    }
    return __ti9AutoChest;
}
