// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW12_HUD_HOST host_check.cpp
// The record's 32-bit pointer word is a handle mapped by hostWordAddress; handle 0 maps to a
// zeroed buffer so the unconditional read at p + 16 stays defined on the host.
#include "update_players_ready.cpp"
#include <cstdio>
#include <cstring>

extern "C" {
int nmPlayersAlive;
int playersAlive[4];
alignas(8) unsigned char player[4 * 23984 + 64];
}
alignas(8) static unsigned char targets[4][64];
alignas(8) static unsigned char zeroTarget[64];
static int localAnswer[4], calls, shrinkTo = -1;
extern "C" int netPlayerIsLocal__Fi(int i) {
    ++calls;
    if (shrinkTo >= 0) nmPlayersAlive = shrinkTo;
    return localAnswer[i];
}
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word == 0) return reinterpret_cast<__UINTPTR_TYPE__>(zeroTarget) - 16;
    return reinterpret_cast<__UINTPTR_TYPE__>(targets[word - 0x7000u]);
}
static void setPos(int t, float x, float y, float z, unsigned flags) {
    float v[3] = {x, y, z};
    std::memcpy(targets[t] + 16, v, 12);
    std::memcpy(targets[t] + 12, &flags, 4);
}
static void link(int index, unsigned word) { std::memcpy(player + index * 23984 + 21872, &word, 4); }

struct Obj { alignas(8) unsigned char b[256]; };
static int bad;
static void expect(const char* what, const Obj& o, int b198, int b199, int b200, int b201) {
    if (o.b[198] != b198 || o.b[199] != b199 || o.b[200] != b200 || o.b[201] != b201) {
        std::printf("FAIL %s: got %d %d %d %d\n", what, o.b[198], o.b[199], o.b[200], o.b[201]);
        bad = 1;
    }
}
static Obj fresh(float x, float y, float z) {
    Obj o;
    std::memset(o.b, 0xee, sizeof o.b);
    float v[3] = {x, y, z};
    std::memcpy(o.b + 16, v, 12);
    o.b[198] = 0;
    return o;
}

int main() {
    // No players: the wide count is zero so byte 198 becomes 1; counters are cleared.
    nmPlayersAlive = 0;
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("empty", o, 1, 0, 0, 0); }
    // Byte 198 already set stays set.
    { Obj o = fresh(0, 0, 0); o.b[198] = 5; updatePlayersReady__9Savepoint(o.b); expect("198 kept", o, 5, 0, 0, 0); }
    // Distance squared 119.99^2 is near; 120 is not (14400 not below 14400); 149.9 wide only; 150+ none.
    struct Case { float x; int near, wide; } cases[] = {{119.0f, 1, 1}, {120.0f, 0, 1}, {149.0f, 0, 1}, {150.0f, 0, 0}};
    for (auto& c : cases) {
        nmPlayersAlive = 1; playersAlive[0] = 2; link(2, 0x7000);
        setPos(0, c.x, 0, 0, 0);
        Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b);
        expect("x distance", o, c.wide == 0 ? 1 : 0, c.near, c.near, 0);
    }
    // z-only and y-only separation use the same squared sum.
    nmPlayersAlive = 1; playersAlive[0] = 0; link(0, 0x7000);
    setPos(0, 0, 0, 100.0f, 0);
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("z only", o, 0, 1, 1, 0); }
    setPos(0, 0, 100.0f, 100.0f, 0);  // 20000: wide only
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("yz wide", o, 0, 0, 0, 0); }
    // Self position matters: subtract mine.
    setPos(0, 500, 500, 500, 0);
    { Obj o = fresh(500, 500, 450); updatePlayersReady__9Savepoint(o.b); expect("relative", o, 0, 1, 1, 0); }
    // Local flag sets 201 and stays set when a later player is not local.
    nmPlayersAlive = 2; playersAlive[0] = 1; playersAlive[1] = 3;
    link(1, 0x7000); link(3, 0x7001); setPos(0, 1, 0, 0, 0); setPos(1, 2, 0, 0, 0);
    localAnswer[1] = 7; localAnswer[3] = 0;
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("local", o, 0, 2, 2, 1); }
    localAnswer[1] = 0; localAnswer[3] = 1;
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("local second", o, 0, 2, 2, 1); }
    localAnswer[3] = 0;
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("not local", o, 0, 2, 2, 0); }
    // Flag bit gives 0xff when anyone is near, even if the flagged player is far (the flag ignores distance).
    setPos(0, 1, 0, 0, 0x400000u);
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("flag near", o, 0, 0xff, 2, 0); }
    setPos(0, 1000, 0, 0, 0x400000u);
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("flag far", o, 0, 0xff, 1, 0); }
    setPos(0, 1000, 0, 0, 0xffbfffffu);  // every other bit set
    setPos(1, 1000, 0, 0, 0);
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("far none", o, 1, 0, 0, 0); }
    // Zero pointer word: read as address 16 (zeroed here), distance is 0 so it counts as near.
    link(1, 0); link(3, 0);
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("null p", o, 0, 2, 2, 0); }
    { Obj o = fresh(300, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("null p far", o, 1, 0, 0, 0); }
    // Four players with an 8-bit count: all near.
    nmPlayersAlive = 4;
    for (int i = 0; i < 4; ++i) { playersAlive[i] = i; link(i, 0x7000 + i); setPos(i, 1, 0, 0, 0); localAnswer[i] = 0; }
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("four", o, 0, 4, 4, 0); }
    // nmPlayersAlive is re-read after netPlayerIsLocal: shrinking it there ends the loop early.
    shrinkTo = 1; calls = 0;
    { Obj o = fresh(0, 0, 0); updatePlayersReady__9Savepoint(o.b); expect("shrunk", o, 0, 1, 1, 0); }
    if (calls != 1) { std::printf("FAIL calls %d\n", calls); bad = 1; }
    shrinkTo = -1;
    std::printf(bad ? "FAIL\n" : "ok: update players ready\n");
    return bad;
}
