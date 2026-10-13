// Observed eRand__10GameObject at 0x153fa8, extent 40.
// A per-object linear congruential generator. The seed is the word at
// object+68. It is replaced by seed * 0x19660d + 0x3c6ef35f (32-bit wrap), and
// the result is the new seed shifted right by 16 bits, a value from 0 to 65535.
// The multiplier and increment are the constants in the instruction stream.
// norrath_game_object_e_rand is the name the constructor sources import this by;
// it is defined here as a second symbol for the same code, not a second routine.
struct RandomObject {
    unsigned char before_seed[68];
    unsigned seed;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(RandomObject, seed) == 68, "observed seed offset");
#endif

extern "C" int eRand__10GameObject(RandomObject* object) {
    object->seed = object->seed * 0x19660du + 0x3c6ef35fu;
    return static_cast<int>(object->seed >> 16);
}

#if defined(__mips__)
extern "C" int norrath_game_object_e_rand(void* object) __attribute__((alias("eRand__10GameObject")));
#endif
