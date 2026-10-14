// Observed _$_9WorldPart at 0x0021bdb0, 140 bytes.
// Stores the WorldPart vtable (0x44d0a8) at object+72. With the world pointer
// word at 0x4844a0 and the signed halfword at +76 (a door index), it calls
// worldSetDoorHeight with the signed halfword at +78 converted to a single as
// the height, then worldReleaseDoor with the same index. The world word is
// loaded again for the second call. Then the GameObject destructor runs with
// a zero flag, and bit 0 of the incoming flag selects blockFree on the same
// object. Other flag bits do not. Unknown: what the height halfword means
// (the callee takes a float) and the layout of the world header.

extern "C" {
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
int worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height);
int worldReleaseDoor__FP12_worldHeaderi(void* world, int index);
extern const unsigned char norrath_actors_a_view_44d0a8[];
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
int read_short(const unsigned char* p) {
    const unsigned v = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return v < 0x8000u ? static_cast<int>(v) : static_cast<int>(v) - 0x10000;
}
void* world() {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(read_word(norrath_actors_b_view_4844a0)));
}
}

extern "C" void dtor_9WorldPart(void* object_storage, int flags) asm("_$_9WorldPart");
extern "C" void dtor_9WorldPart(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_actors_a_view_44d0a8));
    worldSetDoorHeight__FP12_worldHeaderif(world(), read_short(object + 76u),
                                           static_cast<float>(read_short(object + 78u)));
    worldReleaseDoor__FP12_worldHeaderi(world(), read_short(object + 76u));
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
