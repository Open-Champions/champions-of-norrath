// Observed clone__16CosmeticPropAnim at 0x1158c0, 340 bytes.
// Allocates 152 bytes. A null allocation returns null and writes nothing.
// Otherwise the base object table is stored at the new object+72 before the
// three ListHead copy-constructor calls (each zeroes its destination word and
// does not read the source). The remaining stores copy source fields at the
// same offsets, and the class table replaces the word at +72. One animation
// state copy-constructor call, `__14AnimationStateRC14AnimationState`,
// copies the state at +100 from the source state at +100. The word at +88 is stored
// in that call's delay slot, so it is already written; the fields from +140 up
// follow the call. This package does not define the state copy. Bytes between
// the copied ranges keep whatever the allocator left.

extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void __14AnimationStateRC14AnimationState(void* destination, const void* source);
extern const unsigned char norrath_factory_animation_vtable[];
extern const unsigned char norrath_game_object_vtable_view[];
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
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
}

extern "C" void* clone__16CosmeticPropAnim(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(152);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 4);
    copy_bytes(created + 16, source + 16, 8);
    copy_bytes(created + 24, source + 24, 4);
    copy_bytes(created + 28, source + 28, 4);
    copy_bytes(created + 36, source + 36, 4);
    copy_bytes(created + 32, source + 32, 4);
    copy_bytes(created + 40, source + 40, 1);
    write_word(created + 72u, low_pointer(norrath_factory_animation_vtable));
    copy_bytes(created + 41, source + 41, 1);
    copy_bytes(created + 52, source + 52, 4);
    copy_bytes(created + 48, source + 48, 4);
    copy_bytes(created + 76, source + 76, 4);
    copy_bytes(created + 60, source + 60, 4);
    copy_bytes(created + 80, source + 80, 4);
    copy_bytes(created + 44, source + 44, 4);
    copy_bytes(created + 56, source + 56, 2);
    copy_bytes(created + 64, source + 64, 4);
    copy_bytes(created + 68, source + 68, 4);
    copy_bytes(created + 84, source + 84, 4);
    copy_bytes(created + 92, source + 92, 4);
    copy_bytes(created + 96, source + 96, 4);
    copy_bytes(created + 88, source + 88, 4);
    __14AnimationStateRC14AnimationState(created + 100, source + 100);
    copy_bytes(created + 140, source + 140, 1);
    copy_bytes(created + 144, source + 144, 4);
    copy_bytes(created + 148, source + 148, 4);
    return created;
}
