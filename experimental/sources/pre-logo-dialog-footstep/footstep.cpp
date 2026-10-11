// Observed gameFootstepSound__FG6Point3iii at 0x001cdc58, 472 bytes.
// The 12 point bytes are copied before any test. A nonzero second argument
// advances eRandState by state * 0x19660d + 0x3c6ef35f, formats
// "fs_water%d.anm" with ((state >> 16) & 1) + 1, loads that name from
// "fx.lmp", and plays the sequence on the copied point at 1.0. A zero second
// argument returns when the fourth argument has none of bits 0x00780000.
// Bit 0x00400000 selects "foly_plate%d.anm", bit 0x00200000 selects
// "foly_chainmail%d.anm", and bit 0x00100000 selects "foly_leather%d.anm".
// Those three use ((state >> 16) % 5) + 1. Any other set bit in 0x00780000
// selects "foly_cloth%d.anm" with ((state >> 16) & 3) + 1. The third
// argument is not read. eRandState, sprintf, lumpFindResource, and
// soundPlaySequence are not defined here.
extern "C" int eRandState;
extern "C" int sprintf(char* dest, const char* format, ...);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, const unsigned char* point, float volume);

static unsigned advance_state() {
    const unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(state);
    return state;
}

static void play_named(const char* format, int index, const unsigned char* point) {
    char name[64];
    sprintf(name, format, index);
    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(header, point, 1.0f);
}

extern "C" void gameFootstepSound__FG6Point3iii(const unsigned char* point, int selector, int unused, int surface) {
    (void)unused;
    unsigned char local[12];
    for (int i = 0; i < 12; ++i) local[i] = point[i];
    if (selector != 0) {
        const unsigned state = advance_state();
        play_named("fs_water%d.anm", static_cast<int>(((state >> 16) & 1u) + 1u), local);
        return;
    }
    const unsigned bits = static_cast<unsigned>(surface);
    if ((bits & 0x00780000u) == 0u) return;
    const unsigned state = advance_state();
    if ((bits & 0x00400000u) != 0u) {
        play_named("foly_plate%d.anm", static_cast<int>((state >> 16) % 5u) + 1, local);
    } else if ((bits & 0x00200000u) != 0u) {
        play_named("foly_chainmail%d.anm", static_cast<int>((state >> 16) % 5u) + 1, local);
    } else if ((bits & 0x00100000u) != 0u) {
        play_named("foly_leather%d.anm", static_cast<int>((state >> 16) % 5u) + 1, local);
    } else {
        play_named("foly_cloth%d.anm", static_cast<int>(((state >> 16) & 3u) + 1u), local);
    }
}
