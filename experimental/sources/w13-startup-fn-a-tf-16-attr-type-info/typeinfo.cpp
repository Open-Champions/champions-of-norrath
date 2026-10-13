// Observed __tf16__attr_type_info at 0x0031af50, 80 bytes (weak).
// Returns the 12-byte type_info node __ti16__attr_type_info (a zeroed 12-byte object in the image's uninitialised data, 0x008ec8a0).
// Only the first word of the node gates initialization. When it is zero the routine calls __tf9type_info
// and then __rtti_si(node, "16__attr_type_info", __ti9type_info). A nonzero first word skips both calls. The node is
// returned either way.
// Inferred: the node is the class's runtime type descriptor and type_info its single base, from the symbol names.
// Not authored here: __tf9type_info, __rtti_si, __ti9type_info.
extern "C" {
unsigned char __ti16__attr_type_info[12] __attribute__((aligned(4)));
extern unsigned char __ti9type_info[] __attribute__((aligned(4)));
void* __tf9type_info();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf16__attr_type_info() {
    if (unset(__ti16__attr_type_info)) {
        __tf9type_info();
        __rtti_si(__ti16__attr_type_info, "16__attr_type_info", __ti9type_info);
    }
    return __ti16__attr_type_info;
}
