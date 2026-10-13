// Observed _$_11AutoChestLP at 0x002252e0. Stores the 11AutoChestLP vtable (0x44ed78) at object+72, then calls
// the destructor _$_14AnimationState (animation state) at object+152 with flag 2; then the GameObject destructor with a zero flag. Bit 0 of the incoming flag selects blockFree on the object, after those calls.
// Other flag bits do not. The incoming flag is never passed on.
// Inferred: the slot at +72 is the vtable word, as in the neighbouring destructors.

extern "C" {
void animation_state_destructor(void* object, int flags) asm("_$_14AnimationState");
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_rtti_entities_a_view_44ed78[];
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

extern "C" void dtor_11AutoChestLP(void* object_storage, int flags) asm("_$_11AutoChestLP");
extern "C" void dtor_11AutoChestLP(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_rtti_entities_a_view_44ed78));
    animation_state_destructor(object + 152u, 2);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
