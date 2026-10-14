// Observed __tf7AntWave at 0x00312ad0, 80 bytes (weak).
// Returns the 12-byte node __ti7AntWave. The first word of a node gates its initialisation; a set node skips all work.
// Levels (outer to inner): 7AntWave. The innermost level calls __tf8Creature when unset, then registers with __rtti_si
// against __ti8Creature; each unset outer level is registered against the level inside it.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti7AntWave[12] __attribute__((aligned(4)));
extern unsigned char __ti8Creature[12] __attribute__((aligned(4)));
void* __tf8Creature();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf7AntWave() {
    if (unset(__ti7AntWave)) {
        __tf8Creature();
        __rtti_si(__ti7AntWave, "7AntWave", __ti8Creature);
    }
    return __ti7AntWave;
}
