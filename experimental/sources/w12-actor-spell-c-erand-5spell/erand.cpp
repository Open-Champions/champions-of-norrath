// Observed eRand__5Spell at 0x0015e948, 40 bytes. The word at +16 of the object is a linear-congruential
// state: it is replaced by state * 0x19660d + 0x3c6ef35f (32-bit wraparound), stored back, and the new
// state shifted right by 16 (so 0..65535) is returned. Reads and writes only that word.
// Inferred: this is the Spell class's own random-number step (the name, and the constants match the
// shared eRand step used elsewhere). Unknown: how the state is seeded. Nothing here ran on the EE.
extern "C" unsigned eRand__5Spell(void* self) {
    unsigned& state = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 16);
    state = state * 0x19660du + 0x3c6ef35fu;
    return state >> 16;
}
