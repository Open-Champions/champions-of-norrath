// Observed __tf11AutoChestLP at 0x003150c8, 176 bytes (weak).
// Returns the 12-byte type_info node __ti11AutoChestLP. Only the first word of a node gates
// initialization. When the outer node is zero:
//   levels 11AutoChestLP > 9AutoChest > 10AutoObject (outer to inner), base 10GameObject. The innermost empty level first calls
//   __tf10GameObject, then each empty level, innermost first, gets __rtti_si(level, its
//   name text, the next level inward; the innermost uses __ti10GameObject).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti11AutoChestLP[12] __attribute__((aligned(4)));
extern unsigned char __ti10AutoObject[] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
extern unsigned char __ti9AutoChest[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf11AutoChestLP() {
    if (unset(__ti11AutoChestLP)) {
        if (unset(__ti9AutoChest)) {
            if (unset(__ti10AutoObject)) {
                __tf10GameObject();
                __rtti_si(__ti10AutoObject, "10AutoObject", __ti10GameObject);
            }
            __rtti_si(__ti9AutoChest, "9AutoChest", __ti10AutoObject);
        }
        __rtti_si(__ti11AutoChestLP, "11AutoChestLP", __ti9AutoChest);
    }
    return __ti11AutoChestLP;
}
