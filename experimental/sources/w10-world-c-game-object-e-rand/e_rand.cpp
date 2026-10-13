// Observed eRand__10GameObject at 0x153fa8, 40 bytes; this unit supplies the
// import name the constructor sources use, norrath_game_object_e_rand, as its
// own complete routine rather than an alias of the original name.
// The word at object+68 is replaced by seed * 0x19660d + 0x3c6ef35f (32-bit
// wrap) and the result is the new seed shifted right by 16 bits (0..65535).
// Inferred: the word is a per-object random seed; no field name is claimed.
// Unknown: the rest of the object layout.
struct RandomObject {
    unsigned char before_seed[68];
    unsigned seed;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(RandomObject, seed) == 68, "observed seed offset");
#endif

extern "C" int norrath_game_object_e_rand(RandomObject* object) {
    object->seed = object->seed * 0x19660du + 0x3c6ef35fu;
    return static_cast<int>(object->seed >> 16);
}
