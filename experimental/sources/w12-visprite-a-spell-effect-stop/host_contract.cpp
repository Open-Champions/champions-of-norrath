#define NORRATH_HOST_CONTRACT 1
#include "stop.cpp"
#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

struct Event { int kind; unsigned a; unsigned b; };
static std::vector<Event> events;
static unsigned found;
extern "C" char* Playback__19VISpellEffectSystemi(void*, int id) {
    events.push_back({1, static_cast<unsigned>(id), 0});
    return found ? reinterpret_cast<char*>(arena) + found : nullptr;
}
extern "C" void _$_19VISpellEffectDriver(void* d, int flags) { events.push_back({2, off_of(d), static_cast<unsigned>(flags)}); }
extern "C" void DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* s, char* q) {
    events.push_back({3, off_of(q), get(0x100 + 4 + 4 * 5)});   // slot word is already cleared here
}

int main() {
    const unsigned sys = 0x100;
    std::memset(arena, 0, sizeof arena);
    for (int i = 0; i < 24; ++i) put(sys + 4 + 4 * i, 0xdead0000u + i);
    // not found: only the lookup happens and the table is untouched
    found = 0;
    Stop__19VISpellEffectSystemi(arena + sys, 29);
    if (events.size() != 1 || events[0].kind != 1 || events[0].a != 29) return 1;
    if (get(sys + 4 + 4 * 5) != 0xdead0005u) return 2;
    // found with a driver; id 29 lives in slot 29 % 24 == 5 (record byte at +28 = 29)
    events.clear();
    found = 0x2000; put(0x2000 + 28, 29); put(0x2000 + 32, 0x3000);
    Stop__19VISpellEffectSystemi(arena + sys, 29);
    if (events.size() != 3) return 3;
    if (events[1].kind != 2 || events[1].a != 0x3000 || events[1].b != 3) return 4;
    if (events[2].kind != 3 || events[2].a != 0x2000 || events[2].b != 0) return 5;
    for (int i = 0; i < 24; ++i)
        if (get(sys + 4 + 4 * i) != (i == 5 ? 0u : 0xdead0000u + i)) return 6;
    // found without a driver: no driver call; slot comes from the byte at +28 only
    events.clear();
    put(sys + 4 + 4 * 1, 0xdead0001u);
    put(0x2000 + 28, 0x1ff); put(0x2000 + 32, 0);   // low byte 0xff % 24 == 15
    put(sys + 4 + 4 * 15, 0xdead000fu);
    Stop__19VISpellEffectSystemi(arena + sys, 7);
    if (events.size() != 2 || events[1].kind != 3) return 7;
    if (get(sys + 4 + 4 * 15) != 0 || get(sys + 4 + 4 * 1) != 0xdead0001u) return 8;
    return 0;
}
