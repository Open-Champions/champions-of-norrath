// Observed __tf9VICSprite at 0x012081f8, 80 bytes (weak).
// Returns the 12-byte node __ti9VICSprite. A set first word skips all work. When it is zero the function calls
// __tf9VIHSprite, then registers 9VICSprite with __rtti_si under the name "9VICSprite" against __ti9VIHSprite.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti9VICSprite[12] __attribute__((aligned(4)));
extern unsigned char __ti9VIHSprite[12] __attribute__((aligned(4)));
void* __tf9VIHSprite();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf9VICSprite() {
    if (unset(__ti9VICSprite)) {
        __tf9VIHSprite();
        __rtti_si(__ti9VICSprite, "9VICSprite", __ti9VIHSprite);
    }
    return __ti9VICSprite;
}
