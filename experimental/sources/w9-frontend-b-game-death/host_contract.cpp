#include "game_death.cpp"

#include <cstdio>
#include <cstring>

signed char deathState, deathNetHack, netNetworkMode;
int engineDisplayDisable;
char levelName[64];
static int fade_ret, fade_arg, fades, loads, removed;
static void (*removed_fn)();
static char load_name[64] = "temple";
static char full_name[64] = "temple";
extern "C" int hudFadeScreen__Fi(int m) { ++fades; fade_arg = m; return fade_ret; }
extern "C" void MC_LoadRamSlot__Fv() { ++loads; }
extern "C" void gameGetFullLevelName__FPc(char* o) { std::strcpy(o, full_name); }
extern "C" char* MC_GetLoadName__Fv() { return load_name; }
extern "C" void engineRemoveTask__FPFv_v(void (*t)()) { ++removed; removed_fn = t; }
extern "C" int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}

int main() {
    deathState = 0; fade_ret = 127;
    gameDeath__Fv();
    if (deathState != 0 || fades != 1 || fade_arg != 4) return 1;
    fade_ret = 128;
    gameDeath__Fv();
    if (deathState != 1) return 2;
    deathNetHack = 0;
    gameDeath__Fv();
    if (deathState != 2 || loads != 1 || engineDisplayDisable != 0 || netNetworkMode != 0) return 3;
    deathState = 1; deathNetHack = 1;
    gameDeath__Fv();
    if (engineDisplayDisable != 1 || netNetworkMode != 1 || deathState != 2 || loads != 2) return 4;
    levelName[0] = 'x';
    gameDeath__Fv();
    if (removed != 0) return 5;
    levelName[0] = 0;
    std::strcpy(full_name, "cave");
    gameDeath__Fv();
    if (removed != 0) return 6;
    std::strcpy(full_name, "temple");
    gameDeath__Fv();
    if (removed != 1 || removed_fn != gameDeath__Fv || deathState != 2) return 7;
    deathState = 3;
    gameDeath__Fv();
    deathState = -1;
    gameDeath__Fv();
    if (removed != 1 || loads != 2 || fades != 2 || deathState != -1) return 8;
    std::puts("game death passed");
    return 0;
}
