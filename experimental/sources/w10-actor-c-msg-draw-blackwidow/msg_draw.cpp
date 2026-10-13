// Observed msg_draw__10BlackWidow at 0x0020f990, 40 bytes.
// Reads the 32-bit pointer at object+36 and passes the words at +12 and +20 of what it points
// to (model and texture, from the callee's declared parameter types) to the Creature draw
// routine together with the object and a null fourth argument. Returns nothing.
// Unknown: what the structure at +36 is called.
extern "C" {
void draw__8CreatureP12_modelHeaderP8_textureT1(void* creature, void* model, void* texture, void* second_texture);
}

#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
#endif

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
#if defined(NORRATH_W10AC_HOST)
// Host check only: pointer words are offsets from a base chosen by the check.
void* pointer_from_word(unsigned value) {
    return norrath_w10ac_host_base + value;
}
#else
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
#endif
}

extern "C" void msg_draw__10BlackWidow(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    unsigned char* shared = static_cast<unsigned char*>(pointer_from_word(read_word(object + 36)));
    draw__8CreatureP12_modelHeaderP8_textureT1(object, pointer_from_word(read_word(shared + 12)),
                                               pointer_from_word(read_word(shared + 20)), nullptr);
}
