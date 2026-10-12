// Observed _$_4Gold at 0x30dee8, 116 bytes.
// Stores _vt$4Gold (0x44cd48) at object+72, then calls the AnimationState
// destructor on object+88 with flag 2, then the GameObject destructor on the
// object with flag 0. Bit 0 of the incoming flag selects blockFree on the
// object after both. Other flag bits do nothing.

extern "C" {
void _$_14AnimationState(void* state, int flags);
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_player_view_44cd48[];
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

extern "C" void _$_4Gold(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_player_view_44cd48));
    _$_14AnimationState(object + 88, 2);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
