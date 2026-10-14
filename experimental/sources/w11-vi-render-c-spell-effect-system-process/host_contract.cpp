#define NORRATH_HOST_CONTRACT 1
#include "process.cpp"

#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }

static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

struct Event { int kind; unsigned a; unsigned b; };
static std::vector<Event> events;
static char* first;
static unsigned driver_result[4];   // result by driver address /0x100 - 0x30
static unsigned playback_for[64];   // playback record offset returned for an id (0 = none)
static long long now;

extern "C" char* FirstPlayback__19VISpellEffectSystem(void*) { events.push_back({1, 0, 0}); return first; }
extern "C" char* NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void*, char* p) {
    events.push_back({2, off_of(p), 0});
    unsigned link = get(off_of(p) + 36);   // test-only link word inside the record
    return link ? reinterpret_cast<char*>(arena) + link : nullptr;
}
extern "C" int Process__19VISpellEffectDriver(void* d) {
    events.push_back({3, off_of(d), 0});
    return static_cast<int>(driver_result[(off_of(d) >> 8) - 0x30]);
}
extern "C" char* Playback__19VISpellEffectSystemi(void*, int id) {
    events.push_back({4, static_cast<unsigned>(id), 0});
    return playback_for[id] ? reinterpret_cast<char*>(arena) + playback_for[id] : nullptr;
}
extern "C" void _$_19VISpellEffectDriver(void* d, int flags) { events.push_back({5, off_of(d), static_cast<unsigned>(flags)}); }
extern "C" void DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void*, char* q) {
    events.push_back({6, off_of(q), get(0x104 + 4 * 0)});
}
extern "C" long long GetTime__7VIScene(void* scene) { events.push_back({7, off_of(scene), 0}); return now; }

static void record(unsigned at, unsigned id, long long start, unsigned driver, unsigned link) {
    put(at + 28, id);
    std::memcpy(arena + at + 16, &start, 8);
    put(at + 32, driver);
    put(at + 36, link);
}

int main() {
    const unsigned sys = 0x100, scene = 0x1000;
    std::memset(arena, 0, sizeof arena);
    put(sys, scene);
    for (int i = 0; i < 24; ++i) put(sys + 4 + 4 * i, 0xdead0000u + i);
    // r0: driver finished (result 0), looked up, retired. r1: running, young -> untouched.
    // r2: running, old (> 60000 ticks) -> retired. r3: finished but Playback finds nothing.
    record(0x2000, 5, 0, 0x3000, 0x2040);
    record(0x2040, 6, 100000, 0x3100, 0x2080);
    record(0x2080, 29, 10000, 0x3200, 0x20c0);   // 29 % 24 == 5
    record(0x20c0, 7, 0, 0x3300, 0);
    driver_result[0] = 0; driver_result[1] = 1; driver_result[2] = 1; driver_result[3] = 0;
    playback_for[5] = 0x2000; playback_for[29] = 0x2080; playback_for[7] = 0;
    first = reinterpret_cast<char*>(arena) + 0x2000;
    now = 100000 + 60000;                 // r1 age 60000 (not > 60000), r2 age 150000
    if (Process__19VISpellEffectSystem(arena + sys) != 0) return 1;
    // expected event stream
    struct { int kind; unsigned a; unsigned b; } want[] = {
        {1, 0, 0},
        {2, 0x2000, 0}, {3, 0x3000, 0}, {4, 5, 0}, {5, 0x3000, 3}, {6, 0x2000, 0},
        {2, 0x2040, 0}, {3, 0x3100, 0}, {7, scene, 0},
        {2, 0x2080, 0}, {3, 0x3200, 0}, {7, scene, 0}, {4, 29, 0}, {5, 0x3200, 3}, {6, 0x2080, 0},
        {2, 0x20c0, 0}, {3, 0x3300, 0}, {4, 7, 0},
    };
    if (events.size() != sizeof want / sizeof want[0]) return 2;
    for (unsigned i = 0; i < events.size(); ++i)
        if (events[i].kind != want[i].kind || events[i].a != want[i].a) return 3;
    // the slot of id 5 (and of 29, which is the same slot) is zero afterwards; the others are untouched
    if (get(sys + 4 + 4 * 5) != 0) return 4;
    for (int i = 0; i < 24; ++i)
        if (i != 5 && get(sys + 4 + 4 * i) != 0xdead0000u + i) return 5;
    // boundary: age exactly 60000 keeps the record, 60001 retires it
    events.clear();
    for (int i = 0; i < 24; ++i) put(sys + 4 + 4 * i, 1);
    record(0x2040, 6, 100000, 0x3100, 0);
    playback_for[6] = 0x2040;
    first = reinterpret_cast<char*>(arena) + 0x2040;
    now = 100000 + 60001;
    Process__19VISpellEffectSystem(arena + sys);
    if (events.size() != 7 || events[4].kind != 4 || events[6].kind != 6) return 6;
    if (get(sys + 4 + 4 * 6) != 0) return 7;
    // no playbacks at all
    events.clear();
    first = nullptr;
    if (Process__19VISpellEffectSystem(arena + sys) != 0 || events.size() != 1) return 8;
    return 0;
}
