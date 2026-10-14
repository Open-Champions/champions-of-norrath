// Observed __tf9bad_alloc, 80 bytes in the image. It returns the node __ti9bad_alloc (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls
// __tf9exception, then registers "9bad_alloc" with __rtti_si against __ti9exception.
// Inferred: single-base chain, from the registration call shape shared with the other nodes.
// Unknown: the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti9bad_alloc[12] __attribute__((aligned(4)));
extern unsigned char __ti9exception[8] __attribute__((aligned(4)));
void* __tf9exception();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf9bad_alloc() {
    if (unset(__ti9bad_alloc)) {
        __tf9exception();
        __rtti_si(__ti9bad_alloc, "9bad_alloc", __ti9exception);
    }
    return __ti9bad_alloc;
}
