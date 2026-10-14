// Observed clone__11FloorSwitch at 0x00127b28, 288 bytes.
// Allocates 88 bytes. A null allocation returns null and does not write.
// Otherwise stores the GameObject vtable at the new object+72, then calls the
// ListHead copy constructor for the three heads at 0, 4 and 8 (the callee zeros
// the destination word and does not read the source). The source bytes copied
// are 12..41, 44..57, 60..71, 76..80, 84..87.
// Bytes left as the allocator returned them: 42..43, 58..59, 81..83.
// Finally object+72 is replaced with the 11FloorSwitch vtable (the data view at 449790).
// Unknown: what the copied fields mean, and what the allocator leaves in the gap bytes.
extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actors_a_view_449790[];
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

extern "C" void* clone__11FloorSwitch(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(88);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 30);
    copy_bytes(created + 44, source + 44, 14);
    copy_bytes(created + 60, source + 60, 12);
    copy_bytes(created + 76, source + 76, 5);
    copy_bytes(created + 84, source + 84, 4);
    write_word(created + 72u, low_pointer(norrath_actors_a_view_449790));
    return created;
}
