// Observed clone__3Cat at 0x1234d8, 1404 bytes.
// Allocates 992 bytes. A null allocation returns null and writes nothing.
// Stores the base object table at the new object+72, then walks the source
// in the order of the image: list-head copy constructors (each zeroes its
// destination word and does not read the source), word, halfword, byte and
// doubleword copies at the same offsets, and counted copy loops (written here
// as byte-range copies). The class table replaces the word at +72 where a
// later store appears. Animation-state and further list-head copies are
// calls this package does not define. Bytes between the copied ranges keep
// whatever the allocator left.

extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void __15AnimationState8RC15AnimationState8(void* destination, const void* source);
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_actor_view_44c170[];
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

extern "C" void* clone__3Cat(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(992);
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
    write_word(created + 72u, low_pointer(norrath_actor_view_4488f0));
    copy_bytes(created + 41, source + 41, 1);
    copy_bytes(created + 48, source + 48, 4);
    copy_bytes(created + 60, source + 60, 4);
    copy_bytes(created + 68, source + 68, 4);
    copy_bytes(created + 52, source + 52, 4);
    copy_bytes(created + 44, source + 44, 4);
    copy_bytes(created + 56, source + 56, 2);
    copy_bytes(created + 64, source + 64, 4);
    copy_bytes(created + 76, source + 76, 1);
    copy_bytes(created + 80, source + 80, 4);
    copy_bytes(created + 84, source + 84, 1);
    copy_bytes(created + 85, source + 85, 1);
    copy_bytes(created + 88, source + 88, 20);
    copy_bytes(created + 108, source + 108, 16);
    copy_bytes(created + 124, source + 124, 16);
    copy_bytes(created + 140, source + 140, 8);
    copy_bytes(created + 148, source + 148, 8);
    copy_bytes(created + 156, source + 156, 1);
    copy_bytes(created + 168, source + 168, 4);
    copy_bytes(created + 160, source + 160, 8);
    copy_bytes(created + 172, source + 172, 1);
    copy_bytes(created + 173, source + 173, 1);
    copy_bytes(created + 176, source + 176, 4);
    copy_bytes(created + 180, source + 180, 8);
    copy_bytes(created + 188, source + 188, 2);
    copy_bytes(created + 192, source + 192, 4);
    copy_bytes(created + 200, source + 200, 2);
    copy_bytes(created + 212, source + 212, 4);
    copy_bytes(created + 202, source + 202, 2);
    copy_bytes(created + 196, source + 196, 4);
    copy_bytes(created + 204, source + 204, 2);
    copy_bytes(created + 208, source + 208, 4);
    copy_bytes(created + 216, source + 216, 4);
    copy_bytes(created + 244, source + 244, 4);
    copy_bytes(created + 220, source + 220, 4);
    copy_bytes(created + 248, source + 248, 4);
    copy_bytes(created + 236, source + 236, 4);
    copy_bytes(created + 252, source + 252, 4);
    copy_bytes(created + 224, source + 224, 4);
    copy_bytes(created + 232, source + 232, 2);
    copy_bytes(created + 228, source + 228, 4);
    copy_bytes(created + 240, source + 240, 4);
    copy_bytes(created + 256, source + 256, 4);
    copy_bytes(created + 260, source + 260, 8);
    copy_bytes(created + 268, source + 268, 4);
    copy_bytes(created + 272, source + 272, 8);
    copy_bytes(created + 280, source + 280, 4);
    copy_bytes(created + 284, source + 284, 2);
    __15AnimationState8RC15AnimationState8(created + 288, source + 288);
    copy_bytes(created + 552, source + 552, 8);
    copy_bytes(created + 560, source + 560, 4);
    copy_bytes(created + 564, source + 564, 4);
    copy_bytes(created + 568, source + 568, 1);
    copy_bytes(created + 569, source + 569, 1);
    copy_bytes(created + 570, source + 570, 1);
    copy_bytes(created + 572, source + 572, 2);
    norrath_list_head_copy_construct(created + 576, source + 576);
    copy_bytes(created + 580, source + 580, 128);
    copy_bytes(created + 708, source + 708, 4);
    copy_bytes(created + 712, source + 712, 4);
    copy_bytes(created + 716, source + 716, 4);
    copy_bytes(created + 720, source + 720, 8);
    copy_bytes(created + 728, source + 728, 4);
    copy_bytes(created + 732, source + 732, 4);
    copy_bytes(created + 748, source + 748, 2);
    copy_bytes(created + 740, source + 740, 4);
    copy_bytes(created + 736, source + 736, 4);
    copy_bytes(created + 744, source + 744, 4);
    norrath_list_head_copy_construct(created + 752, source + 752);
    copy_bytes(created + 756, source + 756, 4);
    copy_bytes(created + 760, source + 760, 8);
    copy_bytes(created + 768, source + 768, 4);
    copy_bytes(created + 772, source + 772, 4);
    copy_bytes(created + 776, source + 776, 8);
    copy_bytes(created + 784, source + 784, 4);
    copy_bytes(created + 792, source + 792, 2);
    copy_bytes(created + 788, source + 788, 4);
    copy_bytes(created + 794, source + 794, 2);
    copy_bytes(created + 796, source + 796, 2);
    copy_bytes(created + 798, source + 798, 2);
    copy_bytes(created + 800, source + 800, 1);
    copy_bytes(created + 804, source + 804, 4);
    copy_bytes(created + 808, source + 808, 4);
    copy_bytes(created + 812, source + 812, 4);
    copy_bytes(created + 816, source + 816, 8);
    copy_bytes(created + 824, source + 824, 8);
    copy_bytes(created + 832, source + 832, 8);
    copy_bytes(created + 840, source + 840, 8);
    copy_bytes(created + 848, source + 848, 8);
    copy_bytes(created + 856, source + 856, 8);
    copy_bytes(created + 864, source + 864, 8);
    copy_bytes(created + 872, source + 872, 8);
    copy_bytes(created + 880, source + 880, 2);
    copy_bytes(created + 882, source + 882, 2);
    copy_bytes(created + 884, source + 884, 2);
    copy_bytes(created + 886, source + 886, 2);
    norrath_list_head_copy_construct(created + 888, source + 888);
    copy_bytes(created + 892, source + 892, 4);
    copy_bytes(created + 896, source + 896, 64);
    copy_bytes(created + 960, source + 960, 4);
    copy_bytes(created + 980, source + 980, 4);
    copy_bytes(created + 964, source + 964, 4);
    write_word(created + 72u, low_pointer(norrath_actor_view_44c170));
    copy_bytes(created + 984, source + 984, 4);
    copy_bytes(created + 968, source + 968, 4);
    copy_bytes(created + 972, source + 972, 4);
    copy_bytes(created + 976, source + 976, 4);
    return created;
}
