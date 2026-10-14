// Observed _$_8AutoProp at 0x00224c70, 140 bytes. In order:
//   1. store the 8AutoProp vtable (0x44ec40) at object+72;
//   2. worldReleaseDoor(world word at 0x4844a0, signed word at object+396);
//   3. store the 10AutoObject vtable (0x44ed78) at object+72;
//   4. the AnimationState destructor on object+152 with flag 2;
//   5. the GameObject destructor on the object with flag 0;
//   6. when bit 0 of the incoming flag is set, blockFree on the object (tail call).
// Other flag bits do not free, and the incoming flag is not passed on. The word
// at +396 is passed whatever its value (the constructors set it to -1 first).
// Inferred: the +396 word is a door index, from the callee's name. Unknown: its
// field name.

extern "C" {
int worldReleaseDoor__FP12_worldHeaderi(void* world, int index);
void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_ctors_ant_lantern_view_44ec40[];
extern const unsigned char norrath_rtti_entities_a_view_44ed78[];
extern unsigned char norrath_actors_b_view_4844a0[];
}

namespace {
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void dtor_8AutoProp(void* object_storage, int flags) asm("_$_8AutoProp");
extern "C" void dtor_8AutoProp(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_ctors_ant_lantern_view_44ec40));
    worldReleaseDoor__FP12_worldHeaderi(
        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(read_word(norrath_actors_b_view_4844a0))),
        static_cast<int>(read_word(object + 396u)));
    write_word(object + 72u, low_pointer(norrath_rtti_entities_a_view_44ed78));
    animation_state_destructor(object + 152u, 2);
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
