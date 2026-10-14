// Host contract for createRandom__4itemPQ24item11descriptioniiii. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char arena[1 << 16];
unsigned char* norrath_host_base = arena;
#include "create_random.cpp"
static std::string trace;
unsigned syncRandState;
int nmPlayers = 1;
unsigned char player[23984 * 4 + 128];
ItemRow itemTable[4] asm("_4item$itemTable");
PrefixRow prefixTable[3] asm("_4item$prefixTable");
SuffixRow suffixTable[4] asm("_4item$suffixTable");
static int createCalls;
static int valueFor(const std::string& text) {
    if (text.find("+pre0") != std::string::npos) return 900;
    if (text.find("+bb") != std::string::npos) return 200;
    if (text.find("+aa") != std::string::npos) return 300;
    return 100;
}
extern "C" void create__4itemPcPQ24item11descriptionii(char* name, Description* out, int kind, int flags) {
    ++createCalls;
    trace += std::string(name) + "," + std::to_string(kind) + "," + std::to_string(flags) + ";";
    std::memset(out, 0, sizeof *out);
    std::memcpy(arena + 0x100, &itemTable[0], sizeof itemTable[0]);
    *reinterpret_cast<unsigned*>(out) = 0x100;             // a copy of item row 0 in the arena
    bytes(*out)[4] = static_cast<unsigned char>(valueFor(name) / 100);
    for (int i = 0; i < 4; ++i) half(*out, 36 + 8 * i) = -1;
    bytes(*out)[32] = 0xff;
    bytes(*out)[33] = 0xff;
}
extern "C" int getValue__4itemPQ24item11description(Description* d) { return bytes(*d)[4] * 100; }
static unsigned model(unsigned& state) { state = state * 0x19660du + 0x3c6ef35fu; return state >> 16; }
static unsigned put(unsigned at, const char* s) { std::strcpy(reinterpret_cast<char*>(arena + at), s); return at; }
static void setup() {
    std::memset(itemTable, 0, sizeof itemTable);
    std::memset(prefixTable, 0, sizeof prefixTable);
    std::memset(suffixTable, 0, sizeof suffixTable);
    std::memset(player, 0, sizeof player);
    createCalls = 0; trace.clear();
    itemTable[0].name = put(0x200, "ironbar"); itemTable[0].level = 1; itemTable[0].category = 2;
    itemTable[0].flags = 4ull << 11;
    suffixTable[0].head = 1; suffixTable[0].level = 0; suffixTable[0].text[2] = put(0x300, "+aa");
    suffixTable[1].head = 1; suffixTable[1].level = 0; suffixTable[1].text[2] = put(0x310, "+bb");
    prefixTable[0].head = 1; prefixTable[0].level = 0; prefixTable[0].text[0] = put(0x320, "+pre0");
}
static unsigned char outBuf[sizeof(Description)];
int main() {
    Description out;
    // mode 2, one row: replays the attempts with an independent model of the random steps.
    setup();
    syncRandState = 12345;
    std::memset(&out, 0xee, sizeof out);
    *reinterpret_cast<unsigned*>(&out) = 0x100;
    int ret = createRandom__4itemPQ24item11descriptioniiii(&out, 100000, 2, -1, 10);
    unsigned state = 12345;
    int best = -1; unsigned char bestByte = 0;
    for (int a = 0; a < 10; ++a) {
        model(state);
        const int suffix = static_cast<int>(model(state) % 2);
        const int value = suffix == 0 ? 300 : 200;
        if (value >= best) { best = value; bestByte = static_cast<unsigned char>(value / 100); }
    }
    const unsigned field = model(state) % 4;
    if (ret != 1) return 1;
    if (syncRandState != state) return 2;
    if (bytes(out)[4] != bestByte) return 3;
    if (((*reinterpret_cast<unsigned long long*>(bytes(out) + 8) >> 11) & 0x1f) != field) return 4;
    if (createCalls != 10) return 5;
    if (trace.find("ironbar+aa,-1,0;") == std::string::npos || trace.find("ironbar+bb,-1,0;") == std::string::npos) return 6;
    // a limit under every value: nothing stored, 0 returned, the output untouched
    setup();
    syncRandState = 777;
    std::memset(&out, 0xee, sizeof out);
    ret = createRandom__4itemPQ24item11descriptioniiii(&out, 50, 2, -1, 10);
    if (ret != 0) return 7;
    if (bytes(out)[4] != 0xee) return 8;
    // no eligible row (the level argument is below the row's minimum): no step but the
    // count of players, no create call, result 0
    setup();
    syncRandState = 99;
    ret = createRandom__4itemPQ24item11descriptioniiii(&out, 100000, 2, -1, 0);
    if (ret != 0 || createCalls != 0 || syncRandState != 99) return 9;
    // mode 0, class 0 row: the first attempt uses the plain name, later ones the prefix text
    setup();
    itemTable[0].category = 0; itemTable[0].sub = 6;
    unsigned seed = 1;
    for (;; ++seed) { unsigned t = seed; if (model(t) < 1000) break; }
    syncRandState = seed;
    ret = createRandom__4itemPQ24item11descriptioniiii(&out, 100000, 0, 0, 10);
    if (ret != 1) return 10;
    if (createCalls != 15) return 11;
    if (trace.compare(0, 15, "ironbar,-1,0;ir") != 0) return 12;
    if (trace.find("ironbar+pre0,-1,0;") == std::string::npos) return 13;
    if (bytes(out)[4] != 9) return 14;                           // best value comes from the prefix text
    if (bytes(out)[32] != 0) return 15;                          // prefix index 0
    // the prefix word mask clears the byte when the row word has no matching bit
    setup();
    itemTable[0].category = 0; itemTable[0].sub = 6;
    prefixTable[0].word32 = 1;
    syncRandState = seed;
    ret = createRandom__4itemPQ24item11descriptioniiii(&out, 100000, 0, 0, 10);
    if (ret != 1 || bytes(out)[32] != 0xff) return 16;
    itemTable[0].word32 = 1;
    syncRandState = seed;
    ret = createRandom__4itemPQ24item11descriptioniiii(&out, 100000, 0, 0, 10);
    if (ret != 1 || bytes(out)[32] != 0) return 17;
    return 0;
}
