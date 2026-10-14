// Observed __tf9exception, 64 bytes at 0x0031b338 in the reference image. It returns the node __ti9exception (8 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __rtti_user with the node
// and the name "9exception", then returns the node. It does not call __rtti_si; this is the root-class form.
// Inferred: the lazy getter shape shared with the other type-info getters; the registration happens inside __rtti_user.
// Unknown: what __rtti_user writes beyond the first word, and the node fields after the first word.
extern "C" {
extern unsigned char __ti9exception[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf9exception() {
    if (unset(__ti9exception)) __rtti_user(__ti9exception, "9exception");
    return __ti9exception;
}
