// Observed _$_8GothTrap at 0x00315520, 100 bytes.
// Stores the 8GothTrap vtable (0x44f000) at object+72, then calls the AutoProp
// destructor with a zero flag. Bit 0 of the incoming flag selects blockFree on the
// same object, after the base call. Other flag bits do not. The base destructor
// does not receive the incoming flag.

extern "C" {
void autoprop_destructor(void* object, int flags) asm("_$_8AutoProp");
void norrath_block_free(void* object);
extern const unsigned char norrath_actors_b_view_44f000[];
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

extern "C" void dtor_8GothTrap(void* object_storage, int flags) asm("_$_8GothTrap");
extern "C" void dtor_8GothTrap(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_actors_b_view_44f000));
    autoprop_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
