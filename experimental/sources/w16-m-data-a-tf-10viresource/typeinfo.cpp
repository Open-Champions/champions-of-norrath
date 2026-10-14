// Observed __tf10VIResource, 64 bytes at 0x011f5060 in the reference image. It returns the node __ti10VIResource (8 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __rtti_user with the node
// and the name "10VIResource", then returns the node. It does not call __rtti_si; this is the root-class form.
// Inferred: the lazy getter shape shared with the other type-info getters; the registration happens inside __rtti_user.
// Unknown: what __rtti_user writes beyond the first word, and the node fields after the first word.
extern "C" {
extern unsigned char __ti10VIResource[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10VIResource() {
    if (unset(__ti10VIResource)) __rtti_user(__ti10VIResource, "10VIResource");
    return __ti10VIResource;
}
