// Observed __tf13LooseIceChunk at 0x0030fd28, 176 bytes (weak).
// Returns the 12-byte node __ti13LooseIceChunk. The first word of a node gates its initialisation.
// Levels (outer to inner): 13LooseIceChunk > 6PhProp > 4Prop, base 10GameObject. When a level is unset, the innermost level first calls
// __tf10GameObject, then each unset level, innermost first, gets __rtti_si(level, its name text, the next level inward;
// the innermost uses __ti10GameObject). A set level skips its own work and the work inward of it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti13LooseIceChunk[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
extern unsigned char __ti4Prop[12] __attribute__((aligned(4)));
extern unsigned char __ti6PhProp[12] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf13LooseIceChunk() {
    if (unset(__ti13LooseIceChunk)) {
        if (unset(__ti6PhProp)) {
            if (unset(__ti4Prop)) {
                __tf10GameObject();
                __rtti_si(__ti4Prop, "4Prop", __ti10GameObject);
            }
            __rtti_si(__ti6PhProp, "6PhProp", __ti4Prop);
        }
        __rtti_si(__ti13LooseIceChunk, "13LooseIceChunk", __ti6PhProp);
    }
    return __ti13LooseIceChunk;
}
