// Observed __tf17VehicleBodySphere at 0x00312c90, 76 bytes (weak).
// Returns the 12-byte node __ti17VehicleBodySphere. The first word of a node gates its initialisation.
// Levels (outer to inner): 17VehicleBodySphere, base 10GameObject. When a level is unset, the innermost level first calls
// __tf10GameObject, then each unset level, innermost first, gets __rtti_si(level, its name text, the next level inward;
// the innermost uses __ti10GameObject). A set level skips its own work and the work inward of it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti17VehicleBodySphere[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf17VehicleBodySphere() {
    if (unset(__ti17VehicleBodySphere)) {
        __tf10GameObject();
        __rtti_si(__ti17VehicleBodySphere, "17VehicleBodySphere", __ti10GameObject);
    }
    return __ti17VehicleBodySphere;
}
