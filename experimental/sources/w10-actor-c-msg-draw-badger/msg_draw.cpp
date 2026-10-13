// Observed msg_draw__6Badger at 0x001efe40, 100 bytes.
// Compares the float at object+560 with a quarter (0.25f) of the float at object+564. When
// the first is strictly smaller (an unordered compare is false) the Creature draw routine gets
// the object, the word at +12 and the word at +28 of the structure pointed to by the 32-bit
// pointer at object+36 (a tail call); otherwise the word at +12 and the word at +20 of the
// same structure. The fourth argument is null in both cases. Returns nothing.
// Unknown: what the two floats are and what the +20/+28 words select (model and texture are
// from the callee's declared parameter types).
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
void* pointer_from_word(unsigned value) { return norrath_w10ac_host_base + value; }
#else
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
#endif
float read_float(const unsigned char* p) {
    union { unsigned u; float f; } v;
    v.u = read_word(p);
    return v.f;
}
}

extern "C" void msg_draw__6Badger(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    float limit = read_float(object + 564) * 0.25f;
    unsigned char* shared = static_cast<unsigned char*>(pointer_from_word(read_word(object + 36)));
    unsigned texture_offset = read_float(object + 560) < limit ? 28u : 20u;
    draw__8CreatureP12_modelHeaderP8_textureT1(object, pointer_from_word(read_word(shared + 12)),
                                               pointer_from_word(read_word(shared + texture_offset)), nullptr);
}
