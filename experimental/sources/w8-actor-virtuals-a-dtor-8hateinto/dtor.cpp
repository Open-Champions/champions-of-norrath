// Observed _$_8HateInto at 0x00315390, 116 bytes.
// Stores the 8HateInto vtable (0x44f150) at object+72, then runs the FxCreatureState
// destructor on the member at object+408 with flag 2 (so it is not deleted),
// then calls the AutoProp destructor with a zero flag. Bit 0 of the incoming flag selects blockFree on the
// same object, after the base call. Other flag bits do not. The base destructor
// does not receive the incoming flag.

extern "C" {
void autoprop_destructor(void* object, int flags) asm("_$_8AutoProp");
void fx_creature_state_destructor(void* object, int flags) asm("_$_15FxCreatureState");
void norrath_block_free(void* object);
extern const unsigned char norrath_actors_b_view_44f150[];
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

extern "C" void dtor_8HateInto(void* object_storage, int flags) asm("_$_8HateInto");
extern "C" void dtor_8HateInto(void* object_storage, int flags) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 72u, low_pointer(norrath_actors_b_view_44f150));
    fx_creature_state_destructor(object + 408u, 2);
    autoprop_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
