// Observed __tf9type_info at 0x0031a950, 64 bytes (weak). Returns the type_info node
// __ti9type_info, an 8-byte word pair at 0x00486de0 in zero-filled storage. Only the first
// word gates initialization: when it is zero, __rtti_user(node, "9type_info") is called.
// The node is returned either way. __rtti_user takes the node and the class-name text.
// Inferred: the node is the runtime type descriptor of the root type_info class, from the
// symbol names; the node is defined here as this accessor is its only writer in the image.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti9type_info[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf9type_info() {
    if (*reinterpret_cast<const AliasedWord*>(__ti9type_info) == 0u) {
        __rtti_user(__ti9type_info, "9type_info");
    }
    return __ti9type_info;
}
