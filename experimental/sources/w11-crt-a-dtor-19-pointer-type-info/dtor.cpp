// Observed _$_19__pointer_type_info at 0x0031ae30, 100 bytes (weak). Arguments: the object and a
// flag word. It stores the address of _vt$19__pointer_type_info (0x004538f8) in the object's word
// at byte offset 4, calls the base destructor _$_9type_info(object, 0) and, when bit 0 of the flag
// is set, tail-calls __builtin_delete(object). Other flag bits do nothing; nothing is returned.
// The vtable is referenced through the view that the pointer type_info initialiser already uses
// (norrath_legacy_pointer_type_info_vtable_view), not through its original address.
extern "C" {
void _$_9type_info(void* object, int flags);
void __builtin_delete(void* object);
extern const unsigned char norrath_legacy_pointer_type_info_vtable_view[];
}

namespace {
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void _$_19__pointer_type_info(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 4u, low_pointer(norrath_legacy_pointer_type_info_vtable_view));
    _$_9type_info(object, 0);
    if ((flags & 1) != 0) __builtin_delete(object);
}
