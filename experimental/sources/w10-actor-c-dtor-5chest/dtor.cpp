// Observed _$_5Chest at 0x0030e120, 116 bytes.
// Stores the 5Chest vtable (0x44cb98) at object+72, destroys the AnimationState at object+92
// with flag 2, calls the GameObject destructor with flag 0, then blockFree on the object when
// bit 0 of the incoming flag is set. Other flag bits do not free, and the incoming flag is not
// passed on.
// Inferred: this is the class's destructor, from the symbol name. Not executed on the EE.

extern "C" {
void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_ctors_ant_lantern_view_44cb98[];
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

extern "C" void dtor_5Chest(void* object_storage, int flags) asm("_$_5Chest");
extern "C" void dtor_5Chest(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_ctors_ant_lantern_view_44cb98));
    animation_state_destructor(object + 92u, 2);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
