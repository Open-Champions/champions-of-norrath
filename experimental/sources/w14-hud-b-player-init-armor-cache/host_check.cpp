// Host check for playerInitArmorCache__Fv. Not an EE plan input.
// Build as x86_64 with statics below 4 GiB (the group tables hold 32-bit addresses):
//   c++ -arch x86_64 -std=c++17 -Wall -Wextra -Werror -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
#include "player_init_armor_cache.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int nmPlayers;
signed char coreForceSmallCharacters;
void* eqLumpHeader;
void* wLumpHeader;
unsigned eqGroup;
unsigned wGroup;
int nmEqGroups;
int nmWGroups;
unsigned char armorCache[3520] __asm__("norrath_player_armor_cache_view_7cb770");
}
alignas(16) static unsigned char memory[2][4096];
static unsigned char files[2][4096];
static std::string trace;
static int allocs, loads, sizeFor[2];

extern "C" {
int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* size, int mode) {
    trace += std::string("dir(") + name + "," + std::to_string(sector == nullptr) + "," + std::to_string(mode) + ");";
    *size = sizeFor[allocs];
    return 1;
}
void* memAlloc__FiPc(int bytes, char* label) {
    trace += "alloc(" + std::to_string(bytes) + "," + label + ");";
    return memory[allocs++];
}
void cdLoad__FPcPvii(char* name, void* destination, int size, int offset) {
    trace += std::string("load(") + name + "," + std::to_string(size) + "," + std::to_string(offset) + ",hdr=" +
             std::to_string(eqLumpHeader == destination || wLumpHeader == destination) + ");";
    std::memcpy(destination, files[loads++], 4096);
}
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static unsigned w(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static unsigned wordIn(const unsigned char* base, int off) { unsigned v; std::memcpy(&v, base + off, 4); return v; }
static void setWord(unsigned char* base, int off, unsigned v) { std::memcpy(base + off, &v, 4); }

static void reset(int players, int force, int eqCount, int wCount) {
    trace.clear(); allocs = loads = 0;
    sizeFor[0] = 3000; sizeFor[1] = 2048;
    nmPlayers = players; coreForceSmallCharacters = static_cast<signed char>(force);
    std::memset(files, 0, sizeof files);
    setWord(files[0], 0, static_cast<unsigned>(eqCount));
    setWord(files[1], 0, static_cast<unsigned>(wCount));
    for (int g = 0; g < 4; ++g) {
        for (int k = 0; k < 3; ++k) {
            setWord(files[0], 4 + 12 * g + 4 * k, 100 * g + k + 1);
            setWord(files[1], 4 + 12 * g + 4 * k, 1000 + 100 * g + k + 1);
        }
    }
    std::memset(armorCache, 0, sizeof armorCache);
    eqGroup = wGroup = 0xdead;
    eqLumpHeader = wLumpHeader = nullptr;
    nmEqGroups = nmWGroups = -7;
}

int main() {
    // one player, no force: the big-character file; two groups of equipment, one of weapons
    reset(1, 0, 2, 1);
    playerInitArmorCache__Fv();
    CHECK(trace == "dir(eqcache.hdr,1,0);alloc(4096,eqCache header);load(eqcache.hdr,4096,0,hdr=1);"
                   "dir(wcache.hdr,1,0);alloc(2048,wCache header);load(wcache.hdr,2048,0,hdr=1);");
    CHECK(eqLumpHeader == memory[0] && wLumpHeader == memory[1]);
    CHECK(eqGroup == w(memory[0]) + 4 && wGroup == w(memory[1]) + 4);
    CHECK(nmEqGroups == 2 && nmWGroups == 1);
    // groups: words 0 and 1 gain the header address, word 2 does not; only the counted groups change
    for (int g = 0; g < 4; ++g) {
        unsigned add = g < 2 ? w(memory[0]) : 0;
        CHECK(wordIn(memory[0], 4 + 12 * g) == 100 * g + 1 + add);
        CHECK(wordIn(memory[0], 8 + 12 * g) == 100 * g + 2 + add);
        CHECK(wordIn(memory[0], 12 + 12 * g) == 100u * g + 3);
        unsigned addW = g < 1 ? w(memory[1]) : 0;
        CHECK(wordIn(memory[1], 4 + 12 * g) == 1000 + 100 * g + 1 + addW);
        CHECK(wordIn(memory[1], 8 + 12 * g) == 1000 + 100 * g + 2 + addW);
    }
    // armour cache: byte 1 at 1844 + 88 k for k = 0..19 and nothing else
    for (int i = 0; i < 3520; ++i) {
        bool set = i >= 1844 && (i - 1844) % 88 == 0;
        CHECK(armorCache[i] == (set ? 1 : 0));
    }
    // two players, or the force byte: the small file
    reset(2, 0, 0, 0);
    playerInitArmorCache__Fv();
    CHECK(trace.rfind("dir(eqcache2.hdr", 0) == 0 && nmEqGroups == 0 && nmWGroups == 0);
    CHECK(wordIn(memory[0], 4) == 1 && wordIn(memory[1], 4) == 1001);  // no group touched
    reset(1, 1, 0, 0);
    playerInitArmorCache__Fv();
    CHECK(trace.rfind("dir(eqcache2.hdr", 0) == 0);
    reset(1, -1, 0, 0);
    playerInitArmorCache__Fv();
    CHECK(trace.rfind("dir(eqcache2.hdr", 0) == 0);
    reset(0, 0, 0, 0);
    playerInitArmorCache__Fv();
    CHECK(trace.rfind("dir(eqcache.hdr", 0) == 0);
    // a negative count is stored but relocates nothing
    reset(1, 0, -3, 0);
    playerInitArmorCache__Fv();
    CHECK(nmEqGroups == -3 && wordIn(memory[0], 4) == 1);
    std::puts("ok");
    return 0;
}
