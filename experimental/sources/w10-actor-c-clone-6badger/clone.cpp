// Observed clone__6Badger at 0x00117cd8, 1364 bytes.
// Allocates 968 bytes. A null allocation returns null and writes nothing.
// Order of the observed work (offsets into the new object N and the source S):
//   - the GameObject vtable is stored at N+72;
//   - the ListHead copy constructor runs for the heads at 0, 4 and 8 (each callee zeroes the
//     destination word and does not read the source);
//   - byte ranges 12..41, 44..57, 60..71, 76, 80..85, 88..156, 160..173, 176..189, 192..205,
//     208..233 and 236..285 are copied from S to N (the copy loops and the field copies of the
//     image are written here as range copies; ranges were established by running the original
//     on a patterned source and comparing the result);
//   - the Creature vtable (the data view at 4488f0) replaces the word at N+72;
//   - the 15AnimationState8 copy constructor builds the state at +288 from S+288;
//   - ranges 552..570 and 572..573, then the ListHead copy constructor at +576;
//   - range 580..749, then the ListHead copy constructor at +752;
//   - ranges 756..800 and 804..887, then the ListHead copy constructor at +888;
//   - range 892..964, then the Badger vtable (the data view at 44c350) replaces N+72; the new object is returned.
// Bytes not listed keep whatever the allocator left. Unknown: what the copied fields mean.
extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void __15AnimationState8RC15AnimationState8(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_ctors_ant_lantern_view_44c350[];
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

extern "C" void* clone__6Badger(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(968);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 30);
    copy_bytes(created + 44, source + 44, 14);
    copy_bytes(created + 60, source + 60, 12);
    copy_bytes(created + 76, source + 76, 1);
    copy_bytes(created + 80, source + 80, 6);
    copy_bytes(created + 88, source + 88, 69);
    copy_bytes(created + 160, source + 160, 14);
    copy_bytes(created + 176, source + 176, 14);
    copy_bytes(created + 192, source + 192, 14);
    copy_bytes(created + 208, source + 208, 26);
    copy_bytes(created + 236, source + 236, 50);
    write_word(created + 72u, low_pointer(norrath_actor_view_4488f0));
    __15AnimationState8RC15AnimationState8(created + 288, source + 288);
    copy_bytes(created + 552, source + 552, 19);
    copy_bytes(created + 572, source + 572, 2);
    norrath_list_head_copy_construct(created + 576, source + 576);
    copy_bytes(created + 580, source + 580, 170);
    norrath_list_head_copy_construct(created + 752, source + 752);
    copy_bytes(created + 756, source + 756, 45);
    copy_bytes(created + 804, source + 804, 84);
    norrath_list_head_copy_construct(created + 888, source + 888);
    copy_bytes(created + 892, source + 892, 73);
    write_word(created + 72u, low_pointer(norrath_ctors_ant_lantern_view_44c350));
    return created;
}
