// Observed __tf10SpawnMummy at 0x00315e08, 132 bytes (weak).
// Returns the 12-byte node __ti10SpawnMummy. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 10SpawnMummy > 9DustDevil. The innermost level calls __tf10GameObject when unset, then registers with __rtti_si
// against __ti10GameObject; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti10SpawnMummy[12] __attribute__((aligned(4)));
extern unsigned char __ti9DustDevil[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10SpawnMummy() {
    if (unset(__ti10SpawnMummy)) {
        if (unset(__ti9DustDevil)) {
            __tf10GameObject();
            __rtti_si(__ti9DustDevil, "9DustDevil", __ti10GameObject);
        }
        __rtti_si(__ti10SpawnMummy, "10SpawnMummy", __ti9DustDevil);
    }
    return __ti10SpawnMummy;
}
