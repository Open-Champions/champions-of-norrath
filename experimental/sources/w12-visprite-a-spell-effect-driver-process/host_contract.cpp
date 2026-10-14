#define NORRATH_HOST_CONTRACT 1
#include "process.cpp"
#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

struct Event { int kind; unsigned a; unsigned b; };
static std::vector<Event> events;
static int result_for[8];   // by node offset / 0x40 - 0x40
extern "C" int Process__18VISpellEventDriver(void* n) {
    events.push_back({1, off_of(n), 0});
    return result_for[off_of(n) / 0x40 - 0x40];
}
extern "C" void Remove__7VILNode(void* n) {
    events.push_back({2, off_of(n), 0});
    // unlink like the real list would: the node after it stays reachable
}
extern "C" void _$_18VISpellEventDriver(void* n, int f) { events.push_back({3, off_of(n), static_cast<unsigned>(f)}); }

int main() {
    const unsigned sys = 0x100;
    std::memset(arena, 0, sizeof arena);
    // three nodes 0x1000, 0x1040, 0x1080 then the end marker 0x10c0 (first link zero)
    put(sys + 72, 0x1000); put(0x1000, 0x1040); put(0x1040, 0x1080); put(0x1080, 0x10c0); put(0x10c0, 0);
    result_for[0] = 1; result_for[1] = 0; result_for[2] = 0;
    // the stub does not unlink, so the final emptiness test sees the first node's link
    int r = Process__19VISpellEffectDriver(arena + sys);
    if (r != 1) return 1;
    Event want[] = {{1, 0x1000, 0}, {1, 0x1040, 0}, {2, 0x1040, 0}, {3, 0x1040, 3}, {1, 0x1080, 0}, {2, 0x1080, 0}, {3, 0x1080, 3}};
    if (events.size() != sizeof want / sizeof want[0]) return 2;
    for (unsigned i = 0; i < events.size(); ++i)
        if (events[i].kind != want[i].kind || events[i].a != want[i].a || events[i].b != want[i].b) return 3;
    // empty list: the first node is the end marker; result 0 and no calls
    events.clear();
    put(sys + 72, 0x10c0);
    if (Process__19VISpellEffectDriver(arena + sys) != 0 || !events.empty()) return 4;
    // single node, finished: the real Remove would unlink it, emulate by repointing the first word
    events.clear();
    put(sys + 72, 0x1080);
    result_for[2] = 1;
    if (Process__19VISpellEffectDriver(arena + sys) != 1 || events.size() != 1) return 5;
    return 0;
}
