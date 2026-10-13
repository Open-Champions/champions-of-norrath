// Observed __tf10AutoPropLP at 0x00315178, 176 bytes.
// Returns the 12-byte __ti10AutoPropLP node (BSS at 0x008eb4d8). Only the first word of a
// node gates its initialization. When the outer node is zero, three levels are checked,
// outermost first, and the first nonzero one stops the descent:
//   10AutoPropLP > 8AutoProp > 10AutoObject, base 10GameObject.
// The innermost empty level (10AutoObject) first calls __tf10GameObject, then each empty
// level, innermost first, gets __rtti_si(level, its name text, the next level inward); the
// innermost uses __ti10GameObject. The 8AutoProp accessor is not called; its check is inline.
// A nonzero first word at a level skips that level's work and the work below it; levels
// above it still run. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti10AutoPropLP[12] __attribute__((aligned(4)));
extern unsigned char __ti8AutoProp[] __attribute__((aligned(4)));
extern unsigned char __ti10AutoObject[] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10AutoPropLP() {
    if (unset(__ti10AutoPropLP)) {
        if (unset(__ti8AutoProp)) {
            if (unset(__ti10AutoObject)) {
                __tf10GameObject();
                __rtti_si(__ti10AutoObject, "10AutoObject", __ti10GameObject);
            }
            __rtti_si(__ti8AutoProp, "8AutoProp", __ti10AutoObject);
        }
        __rtti_si(__ti10AutoPropLP, "10AutoPropLP", __ti8AutoProp);
    }
    return __ti10AutoPropLP;
}
