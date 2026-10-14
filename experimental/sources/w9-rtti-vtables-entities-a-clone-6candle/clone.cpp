// Observed clone__6Candle at 0x00123318, 448 bytes.
// Allocates 148 bytes. A null allocation returns null and does not write.
// Otherwise stores the GameObject vtable at the new object+72, then calls the
// ListHead copy constructor for the three heads at 0, 4 and 8 (the callee zeros
// the destination word and does not read the source). Source bytes copied: the
// GameObject part 12..41, 44..57, 60..71; then the 6Candle vtable replaces the
// word at 72; then the 60-byte member at 76..135 (a light, from the ~Candle
// destructor which calls _$_5Light on object+76). If either of the first two words
// of that copied member is nonzero, both are cleared and lightAdd runs on the
// member. Then 136..141 and 144..147 are copied.
// Bytes left as the allocator returned them: 42..43, 58..59, 142..143.
// Inferred: the member at +76 is a light, from the destructor's callee name.
// Unknown: what the words at 76 and 80 mean, and the fields after the light.
extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
void lightAdd__FP5Light(void* light);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actors_a_view_44a9c0[];
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
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
}

extern "C" void* clone__6Candle(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(148);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 30);
    copy_bytes(created + 44, source + 44, 14);
    copy_bytes(created + 60, source + 60, 12);
    write_word(created + 72u, low_pointer(norrath_actors_a_view_44a9c0));
    copy_bytes(created + 76, source + 76, 60);
    if (read_word(created + 76) != 0 || read_word(created + 80) != 0) {
        write_word(created + 76, 0);
        write_word(created + 80, 0);
        lightAdd__FP5Light(created + 76);
    }
    copy_bytes(created + 136, source + 136, 6);
    copy_bytes(created + 144, source + 144, 4);
    return created;
}
