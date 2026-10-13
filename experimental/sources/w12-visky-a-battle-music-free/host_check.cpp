// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "free.cpp"
#include <cstring>
#include <string>

static std::string trace;
static unsigned char* music_seen;
static unsigned word_at_call;
extern "C" void Unload__13VIBattleMusic(unsigned char* self) {
    trace += "unload;";
    music_seen = self;
    std::memcpy(&word_at_call, self, 4);
}

static void put(unsigned char* m, unsigned off, int v) { std::memcpy(m + off, &v, 4); }
static int get(const unsigned char* m, unsigned off) { int v; std::memcpy(&v, m + off, 4); return v; }

int main() {
    alignas(8) unsigned char m[64];
    std::memset(m, 0x42, sizeof m);
    put(m, 0, 0x1000); put(m, 4, 3); put(m, 12, 9);
    Free__13VIBattleMusic(m);
    // Unload runs first and still sees the scene word; afterwards +0 and +12 are zero and +4 is -1.
    if (trace != "unload;" || music_seen != m || word_at_call != 0x1000) return 1;
    if (get(m, 0) != 0 || get(m, 4) != -1 || get(m, 12) != 0) return 2;
    if (m[8] != 0x42 || m[16] != 0x42 || m[63] != 0x42) return 3;
    // Not initialised: nothing is called and nothing changes.
    trace.clear();
    std::memset(m, 0x42, sizeof m);
    put(m, 0, 0);
    unsigned char before[64];
    std::memcpy(before, m, sizeof m);
    Free__13VIBattleMusic(m);
    if (!trace.empty() || std::memcmp(m, before, sizeof m) != 0) return 4;
    return 0;
}
