// Observed getModel__8Creature at 0x0030d570, 12 bytes.
// Loads the word at creature+36 and returns the word 12 bytes into that object.
// A null model pointer is not tested by the original body.

extern "C" unsigned getModel__8Creature(void* self) {
    const auto* creature = static_cast<const unsigned char*>(self);
    unsigned word = unsigned(creature[36]) | (unsigned(creature[37]) << 8) |
                    (unsigned(creature[38]) << 16) | (unsigned(creature[39]) << 24);
#if defined(NORRATH_VSLOTS_A_HOST)
    extern unsigned long long norrath_vslots_a_host_base;
    const auto* model = reinterpret_cast<const unsigned char*>(norrath_vslots_a_host_base + word);
#else
    const auto* model = reinterpret_cast<const unsigned char*>(static_cast<unsigned long long>(word));
#endif
    return unsigned(model[12]) | (unsigned(model[13]) << 8) |
           (unsigned(model[14]) << 16) | (unsigned(model[15]) << 24);
}
