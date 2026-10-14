// Observed __tf13WeatherFilter at 0x01204198, 80 bytes (weak).
// Returns the 12-byte node __ti13WeatherFilter. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 13WeatherFilter. The innermost level calls __tf13VISceneFilter when unset, then registers with __rtti_si
// against __ti13VISceneFilter; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti13WeatherFilter[12] __attribute__((aligned(4)));
extern unsigned char __ti13VISceneFilter[8] __attribute__((aligned(4)));
void* __tf13VISceneFilter();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf13WeatherFilter() {
    if (unset(__ti13WeatherFilter)) {
        __tf13VISceneFilter();
        __rtti_si(__ti13WeatherFilter, "13WeatherFilter", __ti13VISceneFilter);
    }
    return __ti13WeatherFilter;
}
