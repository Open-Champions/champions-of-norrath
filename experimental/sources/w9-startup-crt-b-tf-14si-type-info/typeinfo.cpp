// Observed __tf14__si_type_info at 0x0031acf0, 80 bytes (weak).
// Returns the 12-byte type_info node __ti14__si_type_info. Only the first word of a node gates
// initialization. When the outer node is zero:
//   if empty: call __tf16__user_type_info, then __rtti_si(node, "14__si_type_info", __ti16__user_type_info).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti14__si_type_info[12] __attribute__((aligned(4)));
extern unsigned char __ti16__user_type_info[] __attribute__((aligned(4)));
void* __tf16__user_type_info();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf14__si_type_info() {
    if (unset(__ti14__si_type_info)) {
        __tf16__user_type_info();
        __rtti_si(__ti14__si_type_info, "14__si_type_info", __ti16__user_type_info);
    }
    return __ti14__si_type_info;
}
