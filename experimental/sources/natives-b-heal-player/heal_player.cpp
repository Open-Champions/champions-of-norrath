// Observed amxHealPlayer at 0x0013bda0, 136 bytes. It looks up
// spel_cas_curelw.anm in fx.lmp and plays that sequence on the first
// player's creature at volume 1. Then, for each current nmPlayers
// record whose hit-point float at player+172 is above zero, it copies
// the float at player+176 over it. The record stride is 23984.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" void* lumpFindResource__FPcT0(char* archive, char* name);
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
    void* header, void* object, float volume);

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}
static float float_at(const unsigned char* bytes) {
    union Bits { unsigned word; float value; } bits;
    bits.word = static_cast<unsigned>(signed_word(bytes));
    return bits.value;
}
static void store_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    bytes[0] = static_cast<unsigned char>(bits.word);
    bytes[1] = static_cast<unsigned char>(bits.word >> 8);
    bytes[2] = static_cast<unsigned char>(bits.word >> 16);
    bytes[3] = static_cast<unsigned char>(bits.word >> 24);
}

extern "C" int amxHealPlayer__FP5__amxPi(void*, int*) {
    static char archive_name[] = "fx.lmp";
    static char sequence_name[] = "spel_cas_curelw.anm";
    void* resource = lumpFindResource__FPcT0(archive_name, sequence_name);
    void* creature = *reinterpret_cast<void**>(
        norrath_player_progression_records_view + 21872);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(resource, creature, 1.f);
    int count = signed_word(nmPlayers);
    if (count > 0) {
        unsigned char* stats = norrath_player_progression_records_view + 160;
        do {
            const float current = float_at(stats + 12);
            count -= 1;
            if (0.f < current) store_float(stats + 12, float_at(stats + 16));
            stats += 23984;
        } while (count != 0);
    }
    return 0;
}
