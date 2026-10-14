// Observed __tf19__pointer_type_info at 0x0031ae98, 80 bytes (weak). Returns the 12-byte type_info
// node __ti19__pointer_type_info (0x008ec8d0, zero-initialised, defined here). Only the first word of
// the node gates initialization: when it is zero, __tf9type_info() runs and then
// __rtti_si(node, "19__pointer_type_info", __ti9type_info) (the name text is at 0x0047e100). A
// nonzero first word skips both calls. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti19__pointer_type_info[12] __attribute__((aligned(4)));
extern unsigned char __ti9type_info[] __attribute__((aligned(4)));
void* __tf9type_info();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf19__pointer_type_info() {
    if (unset(__ti19__pointer_type_info)) {
        __tf9type_info();
        __rtti_si(__ti19__pointer_type_info, "19__pointer_type_info", __ti9type_info);
    }
    return __ti19__pointer_type_info;
}
