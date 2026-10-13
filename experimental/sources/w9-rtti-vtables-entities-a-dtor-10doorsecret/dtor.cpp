// Observed _$_10DoorSecret at 0x00311a90. Stores the 10DoorSecret vtable (0x4498b0) at object+72, then calls
// the destructor _$_9WorldPart (worldpart) on the object with flag 0. Bit 0 of the incoming flag selects blockFree on the object, after those calls.
// Other flag bits do not. The incoming flag is never passed on.
// Inferred: the slot at +72 is the vtable word, as in the neighbouring destructors.

extern "C" {
void worldpart_destructor(void* object, int flags) asm("_$_9WorldPart");
void norrath_block_free(void* object);
extern const unsigned char norrath_actors_a_view_4498b0[];
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
}

extern "C" void dtor_10DoorSecret(void* object_storage, int flags) asm("_$_10DoorSecret");
extern "C" void dtor_10DoorSecret(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_actors_a_view_4498b0));
    worldpart_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
