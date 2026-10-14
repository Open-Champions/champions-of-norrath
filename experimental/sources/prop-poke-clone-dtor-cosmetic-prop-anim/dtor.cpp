// Observed _$_16CosmeticPropAnim at 0x30e1e8, 116 bytes.
// Stores the class table (norrath_factory_animation_vtable, 0x44cb08) at object+72 first.
// Then the animation state at object+100 is destroyed with flag 2, and the
// GameObject destructor runs on the object with flag 0. Bit 0 of
// the incoming flags selects blockFree on the object afterwards (a tail
// call in the image). Other bits are not read here.
// _$_14AnimationState is an import this package does not define.

extern "C" {
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
void animation_state_destroy(void* state, int flags) asm("_$_14AnimationState");
extern const unsigned char norrath_factory_animation_vtable[];
void destroy_16CosmeticPropAnim(void* object, int flags) asm("_$_16CosmeticPropAnim");
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

extern "C" void destroy_16CosmeticPropAnim(void* object, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    write_word(bytes + 72, low_pointer(norrath_factory_animation_vtable));
    animation_state_destroy(bytes + 100, 2);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
