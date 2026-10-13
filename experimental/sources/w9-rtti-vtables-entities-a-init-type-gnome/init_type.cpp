// Observed initType__5Gnome at 0x002023e8, 160 bytes. Picks a single-bit mask
// for the word at +244 from a fresh eRand roll, depending on the word at +964
// (the variant the constructor stores: 0, 1 or 2):
//   0:      1 << (14 + roll % 9)   (unsigned remainder)
//   1:      1 << (12 + (roll & 1))
//   other:  1 << (roll % 12)       (unsigned remainder)
// Exactly one roll is taken. eRand receives the object as its argument.
// Unknown: what the +244 mask selects and what the variant words mean.
extern "C" {
unsigned eRand__10GameObject(void* self);
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void initType__5Gnome(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    const unsigned variant = read_word(object + 964u);
    unsigned shift;
    if (variant == 0u) {
        shift = 14u + eRand__10GameObject(object) % 9u;
    } else if (variant == 1u) {
        shift = 12u + (eRand__10GameObject(object) & 1u);
    } else {
        shift = eRand__10GameObject(object) % 12u;
    }
    write_word(object + 244u, 1u << shift);
}
