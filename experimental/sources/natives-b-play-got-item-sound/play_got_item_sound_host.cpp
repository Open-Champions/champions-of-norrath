#include <cstdio>
#include <cstring>
extern "C" int amxPlayGotItemSound__FP5__amxPi(void*, int*);
extern "C" int activePlayer = 0;
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 5] = {};
static int sounds = 0;
static int sound_player = -1;
static int sound_id = -1;
extern "C" void playerPlaySound__Fii(int player, int sound) {
    sounds += 1;
    sound_player = player;
    sound_id = sound;
}
static void put_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    std::memcpy(bytes, &bits.word, 4);
}

int main() {
    activePlayer = 5;
    nmPlayers[0] = 1;
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (activePlayer != 1 || sound_player != 5 || sound_id != 9) return 2;
    activePlayer = -3;
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 3;
    if (activePlayer != 0 || sound_player != -3) return 4;
    nmPlayers[0] = 2;
    activePlayer = 0;
    put_float(norrath_player_progression_records_view + 172, 1.f);
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 5;
    if (sound_player != 0 || activePlayer != 0) return 6;
    put_float(norrath_player_progression_records_view + 172, 0.f);
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 7;
    if (sound_player != 1) return 8;
    activePlayer = 1;
    put_float(norrath_player_progression_records_view + 23984 + 172, -1.f);
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 9;
    if (sound_player != 0 || activePlayer != 1) return 10;
    activePlayer = 4;
    put_float(norrath_player_progression_records_view + 4 * 23984 + 172, 0.f);
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 11;
    if (activePlayer != 1 || sound_player != 0) return 12;
    put_float(norrath_player_progression_records_view + 4 * 23984 + 172, 2.f);
    activePlayer = 4;
    if (amxPlayGotItemSound__FP5__amxPi(nullptr, nullptr) != 0) return 13;
    if (activePlayer != 1 || sound_player != 4) return 14;
    std::puts("amxPlayGotItemSound passed");
    return 0;
}
