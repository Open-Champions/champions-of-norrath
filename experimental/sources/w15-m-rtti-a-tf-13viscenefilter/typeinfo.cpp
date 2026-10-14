// Observed __tf13VISceneFilter, 64 bytes in the image. It returns the node __ti13VISceneFilter (8 bytes).
// A nonzero first word returns the node at once. When that word is zero the function registers
// "13VISceneFilter" with __rtti_user and no base. Inferred: a class with no base node.
// Unknown: the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti13VISceneFilter[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf13VISceneFilter() {
    if (unset(__ti13VISceneFilter)) __rtti_user(__ti13VISceneFilter, "13VISceneFilter");
    return __ti13VISceneFilter;
}
