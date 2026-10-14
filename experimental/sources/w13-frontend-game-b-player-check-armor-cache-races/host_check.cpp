// Host contract for playerCheckArmorCacheRaces__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
int nmPlayers;
signed char coreForceSmallCharacters;
unsigned char player[23984 * 4 + 128];
unsigned char armorCache[3520] __asm__("norrath_player_armor_cache_view_7cb770");
int playerCheckArmorCacheRaces__Fv();
}
static signed char* entry(int i) { return reinterpret_cast<signed char*>(armorCache + 80 + 88 * i); }
static void setrace(int i, int v) { std::memcpy(player + 84 + 23984 * i, &v, 4); }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    nmPlayers = 0;
    expect(playerCheckArmorCacheRaces__Fv() == 1, "no players");
    nmPlayers = 1; coreForceSmallCharacters = 0;
    setrace(0, 4); entry(0)[5] = 5; entry(0)[6] = 0;
    expect(playerCheckArmorCacheRaces__Fv() == 1, "one player, size class 0");
    entry(0)[6] = 1;
    expect(playerCheckArmorCacheRaces__Fv() == 0, "one player, size class 1 mismatch");
    coreForceSmallCharacters = 3;
    expect(playerCheckArmorCacheRaces__Fv() == 1, "forced small characters expect 1");
    coreForceSmallCharacters = 0; entry(0)[6] = 0;
    entry(0)[5] = 6;
    expect(playerCheckArmorCacheRaces__Fv() == 0, "race byte mismatch");
    nmPlayers = 3; entry(0)[5] = 5;
    for (int i = 0; i < 3; ++i) { setrace(i, 10 + i); entry(i)[5] = 11 + i; entry(i)[6] = 1; }
    expect(playerCheckArmorCacheRaces__Fv() == 1, "three players, all agree");
    entry(2)[6] = 0;
    expect(playerCheckArmorCacheRaces__Fv() == 0, "third player disagrees");
    entry(2)[6] = 1; entry(1)[5] = -1; setrace(1, -2);
    expect(playerCheckArmorCacheRaces__Fv() == 1, "negative race bytes compare signed");
    if (failures) return 1;
    std::printf("playerCheckArmorCacheRaces contract passed\n");
    return 0;
}
