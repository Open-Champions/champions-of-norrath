// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// Node addresses are 32-bit words mapped onto an arena, as in the other host contracts.
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 18];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "clear_all_fireballs.cpp"

static std::string trace;
static unsigned heads[8];
static unsigned head_count, head_next;
extern "C" {
void Init__7VILList(void* list) { trace += "init:" + std::to_string(static_cast<unsigned char*>(list) - arena) + ";"; }
void AddTail__7VILListP7VILNode(void* list, void* node) {
    trace += "add:" + std::to_string(static_cast<unsigned char*>(list) - arena) + "," +
             std::to_string(static_cast<unsigned char*>(node) - arena) + ";";
}
unsigned char* RemHead__7VILList(void* list) {
    trace += "rem:" + std::to_string(static_cast<unsigned char*>(list) - arena) + ";";
    return head_next < head_count ? arena + heads[head_next++] : nullptr;
}
void Destroy__17VIParticleEmitter(void* e) { trace += "dst:" + std::to_string(static_cast<unsigned char*>(e) - arena) + ";"; }
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void arm(unsigned w1228, unsigned w4460, unsigned pool, unsigned n_heads) {
    std::memset(arena, 0x6b, sizeof arena);
    sw(1228, w1228); sw(4460, w4460); sw(4476, pool ? arena_base + pool : 0);
    heads[0] = 0x10000; heads[1] = 0x12000; head_count = n_heads; head_next = 0;
    trace.clear();
}

int main() {
    arm(1, 9, 0x20000, 2);
    ClearAllFireballs__12VIAtmosphere(arena);
    std::string want = "rem:4448;dst:65600;dst:66592;rem:4448;dst:73792;dst:74784;rem:4448;init:4448;init:4464;";
    for (int i = 0; i < 32; ++i) want += "add:4464," + std::to_string(0x20000 + 2048 * i) + ";";
    if (trace != want) return 1;
    if (rw(4460) != 0) return 2;
    // Empty active list: no destroys, still rebuilds the free list.
    arm(1, 9, 0x20000, 0);
    ClearAllFireballs__12VIAtmosphere(arena);
    want = "rem:4448;init:4448;init:4464;";
    for (int i = 0; i < 32; ++i) want += "add:4464," + std::to_string(0x20000 + 2048 * i) + ";";
    if (trace != want || rw(4460) != 0) return 3;
    // Each guard stops everything.
    arm(0, 9, 0x20000, 2); ClearAllFireballs__12VIAtmosphere(arena);
    if (!trace.empty() || rw(4460) != 9) return 4;
    arm(1, 0, 0x20000, 2); ClearAllFireballs__12VIAtmosphere(arena);
    if (!trace.empty()) return 5;
    arm(1, 9, 0, 2); ClearAllFireballs__12VIAtmosphere(arena);
    if (!trace.empty() || rw(4460) != 9) return 6;
    return 0;
}
