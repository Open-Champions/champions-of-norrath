// Observed __tf19__builtin_type_info, 80 bytes in the image. It returns the node __ti19__builtin_type_info (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls
// __tf9type_info, then registers "19__builtin_type_info" with __rtti_si against __ti9type_info.
// Inferred: single-base chain, from the registration call shape shared with the other nodes.
// Unknown: the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti19__builtin_type_info[12] __attribute__((aligned(4)));
extern unsigned char __ti9type_info[8] __attribute__((aligned(4)));
void* __tf9type_info();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf19__builtin_type_info() {
    if (unset(__ti19__builtin_type_info)) {
        __tf9type_info();
        __rtti_si(__ti19__builtin_type_info, "19__builtin_type_info", __ti9type_info);
    }
    return __ti19__builtin_type_info;
}
