// Host contract for playerDrawWeaponEffects__6Playerf; not an EE input. Build with the host compiler and
// -fno-builtin (strlen is called by name). Words are offsets from w11_actor_b_host_base.
#define W11_ACTOR_B_HOST 1
#include "weapon_effects.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
signed char netNetworkMode;
signed char netPredictionFastForward;
int gamePlayersPresent;
unsigned char armorCache[3520];
unsigned char player[4 * 23984];
unsigned char globalItem[0x6a0];
static unsigned char arena[0x10000] __attribute__((aligned(16)));
static std::string calls;
static int localResult;
static int checkResult[2];
static int checkIndex;
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
int netPlayerIsLocal__Fi(int i) { calls += "local" + std::to_string(i) + ";"; return localResult; }
int inventoryItemCheckSlot__FiPQ24item11description(int p, void* d) { calls += "check" + std::to_string(p) + "," + off(d) + ";"; return checkResult[checkIndex++ & 1]; }
void getPicName__4itemPciPQ24item11description(char* b, int v, void* d) { std::strcpy(b, "abcdefgh.xyz"); calls += "pic" + std::to_string(v) + "," + off(d) + ";"; }
unsigned text_length(const char* t) __asm__("strlen");
unsigned text_length(const char* t) { return static_cast<unsigned>(std::strlen(t)); }
void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* n, void* e, void** m, void** t) {
    calls += std::string("cache(") + n + ")" + std::to_string(static_cast<unsigned char*>(e) - armorCache) + ";";
    *m = arena + 0x5000; *t = arena + 0x5100;
}
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short angle, void* again, void* anim, float a, int flag, int zero, float b, void* matrix) {
    calls += "fx" + off(d) + "," + off(m) + "," + std::to_string(static_cast<unsigned char*>(s) - player) + "," + f(p.x) + "," + f(p.y) + "," + f(p.z) + "," + std::to_string(angle) + "," + off(again) +
             "," + off(anim) + "," + f(a) + "," + std::to_string(flag) + "," + std::to_string(zero) + "," + f(b) + (matrix ? ",matrix" : "") + ";";
}
}
static void set_word(unsigned o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void set_half(unsigned o, short v) { std::memcpy(arena + o, &v, 2); }
static void set_real(unsigned o, float v) { std::memcpy(arena + o, &v, 4); }
static void set_player_word(int index, unsigned o, unsigned v) { std::memcpy(player + index * 23984 + o, &v, 4); }
static const unsigned DESC0 = 0x1000, DESC1 = 0x2000, DEF = 0x3000;

static void reset() {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player); std::memset(armorCache, 0, sizeof armorCache);
    std::memset(globalItem, 0, sizeof globalItem);
    w11_actor_b_host_base = arena;
    netNetworkMode = 0; netPredictionFastForward = 0; gamePlayersPresent = 1; localResult = 0; checkResult[0] = checkResult[1] = 3; checkIndex = 0;
    set_word(960, 2);            // player index 2
    set_word(1400, 1);
    set_real(16, 1.0f); set_real(20, 2.0f); set_real(24, 3.0f); set_half(56, 77); set_word(1332, 0x4800);
    calls.clear();
}
#define CHECK(cond, msg) do { if (!(cond)) { std::puts(calls.c_str()); std::puts("fail: " msg); return 1; } } while (0)

int main() {
    // slot 0 uses table entry k, slot 1 the fixed word; check results 3 do nothing
    reset();
    player[2 * 23984 + 23572] = 1;
    set_player_word(2, 21960 + 4, DESC0); set_player_word(2, 21968, DESC1);
    playerDrawWeaponEffects__6Playerf(arena, 0.5f);
    CHECK(calls == "check2,4096;check2,8192;", "unknown check results do nothing");
    // a zero description skips the slot without a check
    reset(); player[2 * 23984 + 23572] = 0;
    set_player_word(2, 21968, DESC1);
    playerDrawWeaponEffects__6Playerf(arena, 0.5f);
    CHECK(calls == "check2,8192;", "zero description skipped");
    // negative selector byte
    reset(); player[2 * 23984 + 23572] = 0xff; set_player_word(2, 21960 - 4, DESC0);
    playerDrawWeaponEffects__6Playerf(arena, 0.5f);
    CHECK(calls == "check2,4096;", "signed selector");
    // case 0 with the global item table (no pic name): model/texture words 8+16h and 12+16h
    reset(); player[2 * 23984 + 23572] = 0; set_player_word(2, 21960, DESC0);
    set_word(DESC0, DEF); set_half(DEF + 52, 3);
    { unsigned m = 0x4100, t = 0x4200; std::memcpy(globalItem + 8 + 48, &m, 4); std::memcpy(globalItem + 12 + 48, &t, 4); }
    checkResult[0] = 0;
    playerDrawWeaponEffects__6Playerf(arena, 0.5f);
    CHECK(calls == "check2,4096;fx4096,16640,71552,1,2,3,77,18432,288,0.5,1,0,1;", "case 0 with the item table") ;
    // case 1: second cache, state +23648, second integer 1; type 6 sets the flag; pic name path
    reset(); player[2 * 23984 + 23572] = 0; set_player_word(2, 21960, DESC0); set_player_word(2, 84, 99);
    set_word(DESC0, DEF); set_word(DEF + 48, 1); set_word(1400, 6);
    checkResult[0] = 1;
    playerDrawWeaponEffects__6Playerf(arena, 2.0f);
    CHECK(calls == "check2,4096;pic99,4096;cache(abcdefgh)2992;fx4096,20480,71616,1,2,3,77,18432,288,2,1,1,1;", "case 1 with the pic name");
    // case 2: third cache, state +23584, second integer 0; type 7 clears the flag
    reset(); player[2 * 23984 + 23572] = 0; set_player_word(2, 21960, DESC0);
    set_word(DESC0, DEF); set_word(DEF + 48, 1); set_word(1400, 7);
    checkResult[0] = 2;
    playerDrawWeaponEffects__6Playerf(arena, 2.0f);
    CHECK(calls == "check2,4096;pic0,4096;cache(abcdefgh)2288;fx4096,20480,71552,1,2,3,77,18432,288,2,0,0,1;", "case 2");
    // no players present: model and texture are null (offset text "null")
    reset(); gamePlayersPresent = 0; player[2 * 23984 + 23572] = 0; set_player_word(2, 21960, DESC0);
    set_word(DESC0, DEF); set_word(DEF + 48, 1); checkResult[0] = 0;
    playerDrawWeaponEffects__6Playerf(arena, 1.0f);
    CHECK(calls.find("fx4096,null,71552") != std::string::npos && calls.find("cache") == std::string::npos, "no players present");
    // network gates
    reset(); netNetworkMode = 1; localResult = 1; set_word(12, 0);
    player[2 * 23984 + 23572] = 0; set_player_word(2, 21960, DESC0); checkResult[0] = 3;
    playerDrawWeaponEffects__6Playerf(arena, 1.0f);
    CHECK(calls == "local2;", "local without the flag bit returns");
    reset(); netNetworkMode = 1; localResult = 1; set_word(12, 0x20000); netPredictionFastForward = 1;
    set_player_word(2, 21960, DESC0);
    playerDrawWeaponEffects__6Playerf(arena, 1.0f);
    CHECK(calls == "local2;", "fast forward returns");
    reset(); netNetworkMode = 1; localResult = 1; set_word(12, 0x20000);
    set_player_word(2, 21960, DESC0);
    playerDrawWeaponEffects__6Playerf(arena, 1.0f);
    CHECK(calls == "local2;check2,4096;", "local with the flag bit continues");
    reset(); netNetworkMode = 1; localResult = 0; set_player_word(2, 21960, DESC0);
    playerDrawWeaponEffects__6Playerf(arena, 1.0f);
    CHECK(calls.find("check2,4096;") != std::string::npos, "not local continues");
    reset(); netNetworkMode = 0; set_player_word(2, 21960, DESC0);
    playerDrawWeaponEffects__6Playerf(arena, 1.0f);
    CHECK(calls.find("local") == std::string::npos && calls.find("check2,4096;") != std::string::npos, "no network mode");
    return 0;
}
