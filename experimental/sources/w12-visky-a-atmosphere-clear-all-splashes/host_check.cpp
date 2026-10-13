// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// Pool addresses are 32-bit words mapped onto an arena, as in the other host contracts.
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "clear_all_splashes.cpp"

static std::string trace;
extern "C" {
void Init__7VILList(void* list) { trace += "init:" + std::to_string(static_cast<unsigned char*>(list) - arena) + ";"; }
void AddTail__7VILListP7VILNode(void* list, void* node) {
    trace += "add:" + std::to_string(static_cast<unsigned char*>(list) - arena) + "," +
             std::to_string(static_cast<unsigned char*>(node) - arena) + ";";
}
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void arm(unsigned w1228, unsigned w32, unsigned w48, int count) {
    std::memset(arena, 0x6b, sizeof arena);
    sw(1228, w1228); sw(32, w32); sw(48, w48 ? arena_base + w48 : 0); sw(8, static_cast<unsigned>(count));
    trace.clear();
}

int main() {
    // Full path: object at arena+0, pool at arena+0x4000 (36-byte records), three of them.
    arm(1, 5, 0x4000, 3);
    unsigned char want[1 << 16];
    std::memcpy(want, arena, sizeof want);
    ClearAllSplashes__12VIAtmosphere(arena);
    if (trace != "init:20;init:36;add:36,16384;add:36,16420;add:36,16456;") return 1;
    unsigned zero = 0;
    std::memcpy(want + 32, &zero, 4);
    if (std::memcmp(arena, want, sizeof want) != 0) return 2;
    // The list heads are not touched by the routine itself, only passed on.
    arm(1, 5, 0x4000, 0); ClearAllSplashes__12VIAtmosphere(arena);
    if (trace != "init:20;init:36;" || rw(32) != 0) return 3;
    arm(1, 5, 0x4000, -4); ClearAllSplashes__12VIAtmosphere(arena);
    if (trace != "init:20;init:36;" || rw(32) != 0) return 4;
    // Each guard leaves the object and the lists alone.
    arm(0, 5, 0x4000, 3); ClearAllSplashes__12VIAtmosphere(arena);
    if (!trace.empty() || rw(32) != 5) return 5;
    arm(1, 0, 0x4000, 3); ClearAllSplashes__12VIAtmosphere(arena);
    if (!trace.empty() || rw(32) != 0) return 6;
    arm(1, 5, 0, 3); ClearAllSplashes__12VIAtmosphere(arena);
    if (!trace.empty() || rw(32) != 5) return 7;
    return 0;
}
