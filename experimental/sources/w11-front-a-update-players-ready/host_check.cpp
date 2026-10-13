// Host check for updatePlayersReady__9Savepoint. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin -DW11_FRONT_A_HOST host_check.cpp
// The 32-bit pointer words in the player records are handles that hostWordAddress maps to
// host buffers. A player record whose pointer word is zero is not tested on the host: the
// original then reads a point at address 16.
#include "update_players_ready.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int nmPlayersAlive;
int playersAlive[4];
alignas(8) unsigned char player[4 * 23984];
}
alignas(8) static unsigned char target[4][64];
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word >= 0x1000u && word < 0x1004u) return reinterpret_cast<__UINTPTR_TYPE__>(target[word - 0x1000u]);
    return 0;
}

static std::string calls;
static int localMask;
extern "C" int netPlayerIsLocal__Fi(int index) {
    calls += "L" + std::to_string(index) + ";";
    return (localMask >> index) & 1;
}

namespace {
struct Case {
    int count;
    int idx[4];
    unsigned flags[4];
    float x[4];  // target point (y = z = 0); the save point sits at the origin
    unsigned char b198;
    int localMask;
};

void setup(const Case& c) {
    nmPlayersAlive = c.count;
    localMask = c.localMask;
    for (int i = 0; i < 4; ++i) {
        playersAlive[i] = c.idx[i];
        const unsigned handle = 0x1000u + static_cast<unsigned>(i);
        std::memcpy(player + 23984 * i + 21872, &handle, 4);
        std::memset(target[i], 0x33, sizeof target[i]);
        std::memcpy(target[i] + 12, &c.flags[i], 4);
        const float pt[3] = {c.x[i], 0.f, 0.f};
        std::memcpy(target[i] + 16, pt, 12);
    }
}

int run(const char* what, const Case& c, int want200, int want201, int want198, int want199, const char* wantCalls) {
    setup(c);
    alignas(8) unsigned char object[256];
    std::memset(object, 0x77, sizeof object);
    object[198] = c.b198;
    const float origin[3] = {0.f, 0.f, 0.f};
    std::memcpy(object + 16, origin, 12);
    calls.clear();
    updatePlayersReady__9Savepoint(object);
    if (object[200] != want200 || object[201] != want201 || object[198] != want198 || object[199] != want199 || calls != wantCalls) {
        std::printf("%s: got 200=%u 201=%u 198=%u 199=%u calls=%s\n", what, object[200], object[201], object[198], object[199], calls.c_str());
        return 1;
    }
    for (int i = 0; i < 256; ++i)
        if (i != 198 && i != 199 && i != 200 && i != 201 && object[i] != (i >= 16 && i < 28 ? object[i] : 0x77)) { std::printf("%s: stray write %d\n", what, i); return 2; }
    return 0;
}
}

int main() {
    int rc = 0;
    // nobody alive: counts are zero, so byte 198 becomes 1 and byte 199 is 0
    rc |= run("empty", {0, {0, 1, 2, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0}, 0, 0, 1, 0, "");
    // one player at 100 units: near, local
    rc |= run("near local", {1, {2, 0, 0, 0}, {0, 0, 0, 0}, {100, 0, 0, 0}, 0, 4}, 1, 1, 0, 1, "L2;");
    // near but not local
    rc |= run("near remote", {1, {1, 0, 0, 0}, {0, 0, 0, 0}, {100, 0, 0, 0}, 0, 0}, 1, 0, 0, 1, "L1;");
    // two near, one far (130) counted only for the 150 radius, one beyond
    rc |= run("mixed", {4, {0, 1, 2, 3}, {0, 0, 0, 0}, {10, 130, 119.99f, 200}, 0, 1}, 2, 1, 0, 2, "L0;L2;");
    // boundary: 120 exactly is not near; 119.9979 is
    rc |= run("edge 120", {1, {0, 0, 0, 0}, {0, 0, 0, 0}, {120.0f, 0, 0, 0}, 0, 1}, 0, 0, 0, 0, "");
    rc |= run("edge 119.9979", {1, {0, 0, 0, 0}, {0, 0, 0, 0}, {119.9979f, 0, 0, 0}, 0, 1}, 1, 1, 0, 1, "L0;");
    // 150 boundary decides byte 198 only
    rc |= run("edge 150", {1, {0, 0, 0, 0}, {0, 0, 0, 0}, {150.0f, 0, 0, 0}, 0, 0}, 0, 0, 1, 0, "");
    rc |= run("edge 149.9983", {1, {0, 0, 0, 0}, {0, 0, 0, 0}, {149.9983f, 0, 0, 0}, 0, 0}, 0, 0, 0, 0, "");
    // byte 198 already set (signed nonzero) stays
    rc |= run("198 kept", {1, {0, 0, 0, 0}, {0, 0, 0, 0}, {300, 0, 0, 0}, 0x80, 0}, 0, 0, 0x80, 0, "");
    // flag bit with a near player turns byte 199 into 0xff; without a near player it stays 0
    rc |= run("flag near", {1, {0, 0, 0, 0}, {0x400000, 0, 0, 0}, {50, 0, 0, 0}, 0, 0}, 1, 0, 0, 0xff, "L0;");
    rc |= run("flag far", {1, {0, 0, 0, 0}, {0x400000, 0, 0, 0}, {500, 0, 0, 0}, 0, 0}, 0, 0, 1, 0, "");
    rc |= run("other bit", {1, {0, 0, 0, 0}, {0xffbfffff, 0, 0, 0}, {50, 0, 0, 0}, 0, 0}, 1, 0, 0, 1, "L0;");
    // the flag counts even from a player that is not near, when another is
    rc |= run("flag from far player", {2, {0, 1, 0, 0}, {0, 0x400000, 0, 0}, {50, 500, 0, 0}, 0, 0}, 1, 0, 0, 0xff, "L0;");
    // the count is the table length, not the slot: only the first two entries are used
    rc |= run("count limits", {2, {0, 1, 2, 3}, {0, 0, 0, 0}, {50, 50, 50, 50}, 0, 0xf}, 2, 1, 0, 2, "L0;L1;");
    // 256 near players wrap the low byte only in theory: four players give 4
    rc |= run("four near", {4, {0, 1, 2, 3}, {0, 0, 0, 0}, {1, 2, 3, 4}, 0, 0}, 4, 0, 0, 4, "L0;L1;L2;L3;");
    // y and z contribute
    {
        Case c = {1, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0};
        setup(c);
        alignas(8) unsigned char object[256] = {};
        const float at[3] = {1.f, 2.f, 3.f};
        std::memcpy(object + 16, at, 12);
        const float far[3] = {1.f + 0.f, 2.f + 120.f, 3.f};  // 120 away in y exactly: not near
        std::memcpy(target[0] + 16, far, 12);
        updatePlayersReady__9Savepoint(object);
        if (object[200] != 0) return 20;
        const float nearpt[3] = {1.f, 2.f, 3.f + 119.f};
        std::memcpy(target[0] + 16, nearpt, 12);
        updatePlayersReady__9Savepoint(object);
        if (object[200] != 1) return 21;
    }
    if (rc) return rc;
    std::puts("update players ready passed");
    return 0;
}
