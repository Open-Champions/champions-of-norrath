// Observed __tf8VISprite at 0x011f4d70, 80 bytes. It returns the node __ti8VISprite (0x012278d0).
// A nonzero first word of the node returns it at once. When that word is zero the function calls
// __tf10VIResource, then registers "8VISprite" with __rtti_si against __ti10VIResource (0x01228970).
// Inferred: the single-base chain follows the registration call shape used by the other sprite nodes.
// Unknown: the name string's exact contents, which are taken here from the class name only.
extern "C" {
extern unsigned char __ti8VISprite[12] __attribute__((aligned(4)));
extern unsigned char __ti10VIResource[8] __attribute__((aligned(4)));
void* __tf10VIResource();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf8VISprite() {
    if (unset(__ti8VISprite)) {
        __tf10VIResource();
        __rtti_si(__ti8VISprite, "8VISprite", __ti10VIResource);
    }
    return __ti8VISprite;
}
