// Observed __tf14VISimpleSprite at 0x01226770, 80 bytes (weak).
// Returns the 12-byte node __ti14VISimpleSprite. A set first word skips all work. When it is zero the function calls
// __tf8VISprite, then registers 14VISimpleSprite with __rtti_si under the name "14VISimpleSprite" against __ti8VISprite.
// Inferred: the nodes are runtime type descriptors and the chain is single inheritance, from the symbol names and the
// registration call shape. Nothing here was executed on the EE.
extern "C" {
unsigned char __ti14VISimpleSprite[12] __attribute__((aligned(4)));
extern unsigned char __ti8VISprite[12] __attribute__((aligned(4)));
void* __tf8VISprite();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf14VISimpleSprite() {
    if (unset(__ti14VISimpleSprite)) {
        __tf8VISprite();
        __rtti_si(__ti14VISimpleSprite, "14VISimpleSprite", __ti8VISprite);
    }
    return __ti14VISimpleSprite;
}
