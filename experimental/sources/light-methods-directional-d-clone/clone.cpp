// Observed clone__22charDirectional_LightD at 0x127480, 256 bytes.
// Allocates 76 bytes. A null allocation returns null and does not call
// the list-head copy. Otherwise the GameObject vtable is stored at the new
// object+72 before the three list-head calls. Those calls receive the new
// object and the source at offsets 0, 4, and 8. The later stores match the
// GameObject clone: words and bytes through offset 71, leaving bytes 42, 43,
// 58, and 59 as the allocator left them. The class vtable then replaces the
// GameObject vtable at offset 72.

extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_factory_directional_d_vtable[];
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

extern "C" void* norrath_vt_clone_22charDirectional_LightD(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(76);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 30);
    copy_bytes(created + 44, source + 44, 14);
    copy_bytes(created + 60, source + 60, 12);
    write_word(created + 72u, low_pointer(norrath_factory_directional_d_vtable));
    return created;
}
