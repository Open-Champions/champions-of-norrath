// Observed _$_5Demon at 0x00314018, 100 bytes.
// Stores the _vt$5Demon vtable (0x44dd68) at object+72, calls the Creature destructor with flag 0,
// then blockFree when incoming flag bit 0 is set. Other flag bits do not free.
// The observed free callee is blockFree__FPv.

#if defined(NORRATH_VSLOTS_A_HOST)
extern unsigned long long norrath_vslots_a_host_base;
#endif

extern "C" {
extern const unsigned char norrath_ctors_ant_lantern_view_44dd68[];
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

extern "C" void dtor_5Demon(void* self, int flags) asm("_$_5Demon");
extern "C" void dtor_5Demon(void* self, int flags) {
    auto* object = static_cast<unsigned char*>(self);
    write_word(object + 72, to_word(norrath_ctors_ant_lantern_view_44dd68));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
