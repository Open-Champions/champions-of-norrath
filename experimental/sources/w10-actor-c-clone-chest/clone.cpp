// Observed clone__5Chest at 0x00115780, 316 bytes.
// Allocates 136 bytes. A null allocation returns null and does not write.
// Otherwise stores the GameObject vtable at the new object+72, then calls the ListHead copy
// constructor for the three heads at 0, 4 and 8 (the callee zeros the destination word and does
// not read the source). The source bytes copied are 12..41, 44..57, 60..71, 76..77 and 80..89,
// then the 5Chest vtable (the data view at 44cb98) replaces the one at +72, then the
// AnimationState copy constructor builds the state at +92 from the source's +92, and finally
// the word at 132 is copied. Returns the new object.
// Bytes left as the allocator returned them by this function: 42..43, 58..59, 78..79, 90..91.
// Bytes 92..131 belong to the AnimationState copy constructor.
// Unknown: what the copied fields mean, and what the allocator leaves in the gap bytes.
extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void __14AnimationStateRC14AnimationState(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
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
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
}

extern "C" void* clone__5Chest(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(136);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 30);
    copy_bytes(created + 44, source + 44, 14);
    copy_bytes(created + 60, source + 60, 12);
    copy_bytes(created + 76, source + 76, 2);
    copy_bytes(created + 80, source + 80, 10);
    write_word(created + 72u, low_pointer(norrath_ctors_ant_lantern_view_44cb98));
    __14AnimationStateRC14AnimationState(created + 92, source + 92);
    copy_bytes(created + 132, source + 132, 4);
    return created;
}
