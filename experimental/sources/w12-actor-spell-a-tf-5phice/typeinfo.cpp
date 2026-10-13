// Observed __tf5PhIce at 0x0030de38, 176 bytes (weak).
// Returns the 12-byte node __ti5PhIce. The first word of a node gates its initialisation.
// Levels (outer to inner): 5PhIce > 6PhProp > 4Prop, base 10GameObject. When a level is unset, the innermost level first calls
// __tf10GameObject, then each unset level, innermost first, gets __rtti_si(level, its name text, the next level inward;
// the innermost uses __ti10GameObject). A set level skips its own work and the work inward of it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti5PhIce[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
extern unsigned char __ti4Prop[12] __attribute__((aligned(4)));
extern unsigned char __ti6PhProp[12] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf5PhIce() {
    if (unset(__ti5PhIce)) {
        if (unset(__ti6PhProp)) {
            if (unset(__ti4Prop)) {
                __tf10GameObject();
                __rtti_si(__ti4Prop, "4Prop", __ti10GameObject);
            }
            __rtti_si(__ti6PhProp, "6PhProp", __ti4Prop);
        }
        __rtti_si(__ti5PhIce, "5PhIce", __ti6PhProp);
    }
    return __ti5PhIce;
}
