// Observed _$_8FireBomb at 0x00312100, 100 bytes.
// Stores the 8FireBomb vtable (0x449430) at object+72, then calls the GameObject
// destructor with a zero flag. Bit 0 of the incoming flag selects blockFree on the
// same object, after the base call. Other flag bits do not. The base destructor
// does not receive the incoming flag.

extern "C" {
void norrath_vt_dtor_10GameObject(void* object, int flags);
void norrath_block_free(void* object);
extern const unsigned char norrath_actors_a_view_449430[];
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

extern "C" void dtor_8FireBomb(void* object_storage, int flags) asm("_$_8FireBomb");
extern "C" void dtor_8FireBomb(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_actors_a_view_449430));
    norrath_vt_dtor_10GameObject(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
