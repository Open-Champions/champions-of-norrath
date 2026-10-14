// Observed __tfv at 0x002fe838, 52 bytes, and its node __tiv, an 8-byte object at 0x004480f0 in the
// image's initialised data (all zero in the image).
// Returns the node. When its first word is zero it stores the address of the text "v" (0x0047e068,
// a one-character name) in word 0 and the address of the vtable _vt$19__builtin_type_info in word 1,
// without calling anything. A nonzero first word skips the stores.
// Inferred: this is the descriptor for the built-in type void, from the name text; the 19__builtin_type_info
// vtable is shared with the neighbouring accessors (__tfi, __tfc) which are not part of this unit.
// Not authored here: _vt$19__builtin_type_info.
extern "C" {
unsigned char __tiv[2 * sizeof(void*)] __attribute__((aligned(4)));
extern unsigned char _vt$19__builtin_type_info[];
}

typedef __UINTPTR_TYPE__ __attribute__((may_alias)) AliasedWord;

extern "C" void* __tfv() {
    AliasedWord* node = reinterpret_cast<AliasedWord*>(__tiv);
    if (node[0] == 0) {
        node[0] = reinterpret_cast<__UINTPTR_TYPE__>("v");
        node[1] = reinterpret_cast<__UINTPTR_TYPE__>(_vt$19__builtin_type_info);
    }
    return __tiv;
}
