// Observed __tf10bad_typeid, 80 bytes in the image. It returns the node __ti10bad_typeid (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls
// __tf9exception, then registers "10bad_typeid" with __rtti_si against __ti9exception.
// Inferred: single-base chain, from the registration call shape shared with the other nodes.
// Unknown: the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti10bad_typeid[12] __attribute__((aligned(4)));
extern unsigned char __ti9exception[8] __attribute__((aligned(4)));
void* __tf9exception();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10bad_typeid() {
    if (unset(__ti10bad_typeid)) {
        __tf9exception();
        __rtti_si(__ti10bad_typeid, "10bad_typeid", __ti9exception);
    }
    return __ti10bad_typeid;
}
