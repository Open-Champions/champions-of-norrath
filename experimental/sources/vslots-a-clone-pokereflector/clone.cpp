// Observed clone__13PokeReflector at 0x00128bf8, 256 bytes.
// Allocates 76 bytes. A null allocation returns null and does not write.
// Otherwise stores the GameObject vtable at the new object+72 before the three
// ListHead copy-constructor calls. That callee zeros the destination word and
// does not read the source. The remaining stores are the source fields at
// 12..41, 44..57, and 60..71. Bytes 42, 43, 58, and 59 are left untouched.
// After those copies, object+72 is replaced with the PokeReflector vtable.
extern "C" {
unsigned char* norrath_block_alloc(int requested_size);
void norrath_list_head_copy_construct(void* destination, const void* source);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_next_view_449280[];
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

extern "C" void* clone__13PokeReflector(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = norrath_block_alloc(76);
    if (created == nullptr) return nullptr;
    write_word(created + 72u, low_pointer(norrath_game_object_vtable_view));
    norrath_list_head_copy_construct(created, source);
    norrath_list_head_copy_construct(created + 4, source + 4);
    norrath_list_head_copy_construct(created + 8, source + 8);
    copy_bytes(created + 12, source + 12, 4);
    copy_bytes(created + 16, source + 16, 12);
    copy_bytes(created + 28, source + 28, 4);
    copy_bytes(created + 36, source + 36, 4);
    copy_bytes(created + 32, source + 32, 4);
    created[40] = source[40];
    created[41] = source[41];
    copy_bytes(created + 48, source + 48, 4);
    copy_bytes(created + 44, source + 44, 4);
    copy_bytes(created + 60, source + 60, 4);
    copy_bytes(created + 52, source + 52, 4);
    created[56] = source[56];
    created[57] = source[57];
    copy_bytes(created + 64, source + 64, 8);
    write_word(created + 72u, low_pointer(norrath_next_view_449280));
    return created;
}
