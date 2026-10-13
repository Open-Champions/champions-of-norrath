// Observed __tf12VINameSprite at 0x01207d68, 80 bytes (weak).
// Returns the 12-byte node __ti12VINameSprite. A set first word skips all work. When it is zero the function calls
// __tf8VISprite, then registers 12VINameSprite with __rtti_si under the name "12VINameSprite" against __ti8VISprite.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti12VINameSprite[12] __attribute__((aligned(4)));
extern unsigned char __ti8VISprite[12] __attribute__((aligned(4)));
void* __tf8VISprite();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf12VINameSprite() {
    if (unset(__ti12VINameSprite)) {
        __tf8VISprite();
        __rtti_si(__ti12VINameSprite, "12VINameSprite", __ti8VISprite);
    }
    return __ti12VINameSprite;
}
