// Observed _$_8ItemProp at 0x30dbf0, 100 bytes.
// Stores _vt$4Prop (0x44d018), the Prop vtable and not the ItemProp one, at
// object+72, then calls the GameObject destructor with a zero flag. Bit 0 of
// the incoming flag selects blockFree on the object afterward. Other flag bits
// do nothing.

extern "C" {
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_player_view_44d018[];
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

extern "C" void _$_8ItemProp(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_player_view_44d018));
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
