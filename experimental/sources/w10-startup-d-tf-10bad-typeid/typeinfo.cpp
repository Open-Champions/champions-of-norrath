// Observed __tf10bad_typeid at 0x0031aa68, 80 bytes (weak). Returns the 12-byte type_info node
// __ti10bad_typeid (0x008ec840, zero-filled storage). Only the first word of the node gates
// initialization. When it is zero: call __tf9exception, then
// __rtti_si(node, "10bad_typeid", __ti9exception). The node is returned either way.
// __rtti_si takes the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor and 9exception is its base, from
// the symbol names and the call; the node is defined here as this accessor is its only
// writer in the image.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti10bad_typeid[12] __attribute__((aligned(4)));
extern unsigned char __ti9exception[] __attribute__((aligned(4)));
void* __tf9exception();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf10bad_typeid() {
    if (*reinterpret_cast<const AliasedWord*>(__ti10bad_typeid) == 0u) {
        __tf9exception();
        __rtti_si(__ti10bad_typeid, "10bad_typeid", __ti9exception);
    }
    return __ti10bad_typeid;
}
