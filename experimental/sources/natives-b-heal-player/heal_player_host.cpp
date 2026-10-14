#include <cstdio>
#include <cstring>
extern "C" int amxHealPlayer__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2] = {};
static char archive_seen[16];
static char sequence_seen[32];
static void* creature_seen;
static float volume_seen;
static int plays = 0;
static int marker = 1;

extern "C" void* lumpFindResource__FPcT0(char* archive, char* name) {
    std::strcpy(archive_seen, archive);
    std::strcpy(sequence_seen, name);
    return &marker;
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
    void* header, void* object, float volume) {
    plays += 1;
    if (header != &marker) plays = -100;
    creature_seen = object;
    volume_seen = volume;
}

static void put_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    std::memcpy(bytes, &bits.word, 4);
}
static float get_float(const unsigned char* bytes) {
    unsigned word;
    std::memcpy(&word, bytes, 4);
    union Bits { unsigned word; float value; } bits;
    bits.word = word;
    return bits.value;
}

int main() {
    std::memset(nmPlayers, 0, 4);
    std::memset(norrath_player_progression_records_view, 0, sizeof norrath_player_progression_records_view);
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = &marker;
    put_float(norrath_player_progression_records_view + 172, 3.f);
    put_float(norrath_player_progression_records_view + 176, 9.f);
    if (amxHealPlayer__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (plays != 1 || creature_seen != &marker || volume_seen != 1.f) return 2;
    if (std::strcmp(archive_seen, "fx.lmp") != 0) return 3;
    if (std::strcmp(sequence_seen, "spel_cas_curelw.anm") != 0) return 4;
    if (get_float(norrath_player_progression_records_view + 172) != 3.f) return 5;
    nmPlayers[0] = 2;
    put_float(norrath_player_progression_records_view + 172, 0.f);
    put_float(norrath_player_progression_records_view + 176, 9.f);
    put_float(norrath_player_progression_records_view + 23984 + 172, 1.f);
    put_float(norrath_player_progression_records_view + 23984 + 176, 4.f);
    if (amxHealPlayer__FP5__amxPi(nullptr, nullptr) != 0) return 6;
    if (get_float(norrath_player_progression_records_view + 172) != 0.f) return 7;
    if (get_float(norrath_player_progression_records_view + 23984 + 172) != 4.f) return 8;
    std::puts("amxHealPlayer passed");
    return 0;
}
