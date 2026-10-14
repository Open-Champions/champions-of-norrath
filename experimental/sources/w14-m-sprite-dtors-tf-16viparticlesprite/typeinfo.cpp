// Observed __tf16VIParticleSprite at 0x01207fe0, 80 bytes (weak).
// Returns the 12-byte node __ti16VIParticleSprite. A set first word skips all work. When it is zero the function calls
// __tf8VISprite, then registers 16VIParticleSprite with __rtti_si under the name "16VIParticleSprite" against __ti8VISprite.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti16VIParticleSprite[12] __attribute__((aligned(4)));
extern unsigned char __ti8VISprite[12] __attribute__((aligned(4)));
void* __tf8VISprite();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf16VIParticleSprite() {
    if (unset(__ti16VIParticleSprite)) {
        __tf8VISprite();
        __rtti_si(__ti16VIParticleSprite, "16VIParticleSprite", __ti8VISprite);
    }
    return __ti16VIParticleSprite;
}
