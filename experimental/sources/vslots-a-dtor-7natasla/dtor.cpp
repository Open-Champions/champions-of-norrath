// Observed _$_7Natasla at 0x00315a38, 100 bytes.
// Stores the Natasla vtable at object+72, calls the Creature destructor with flag 0,
// then blockFree when incoming flag bit 0 is set. Other flag bits do not free.
// The observed free callee is blockFree__FPv.

#if defined(NORRATH_VSLOTS_A_HOST)
extern unsigned long long norrath_vslots_a_host_base;
#endif

extern "C" {
extern const unsigned char norrath_actor_view_44f4c8[];
void creature_destructor(void* self, int flags) asm("_$_8Creature");
void norrath_block_free(void* object);
}

namespace {
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned to_word(const void* pointer) {
#if defined(NORRATH_VSLOTS_A_HOST)
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(pointer) - norrath_vslots_a_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<unsigned long long>(pointer));
#endif
}
}

extern "C" void natasla_destructor(void* self, int flags) asm("_$_7Natasla");
extern "C" void natasla_destructor(void* self, int flags) {
    auto* object = static_cast<unsigned char*>(self);
    write_word(object + 72, to_word(norrath_actor_view_44f4c8));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
