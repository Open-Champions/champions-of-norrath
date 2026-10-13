#define NORRATH_HOST_CONTRACT 1
#include "release_all.cpp"
#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

struct Event { int kind; unsigned a; unsigned b; };
static std::vector<Event> events;
static std::vector<int> live;       // pool indices still present
static int destroy_result[8];       // by effect index (0x4000 + 0x40 * n)
static int destroy_calls[8];

extern "C" int Begin__Ct6VIPool1ZP13VISpellEffect(void* p) { events.push_back({1, off_of(p), 0}); return live.empty() ? 99 : live[0]; }
extern "C" int End__Ct6VIPool1ZP13VISpellEffect(void* p) { events.push_back({2, off_of(p), 0}); return 99; }
extern "C" void Erase__t6VIPool1ZP13VISpellEffecti(void* p, int i) {
    events.push_back({3, off_of(p), static_cast<unsigned>(i)});
    live.erase(live.begin());
}
extern "C" void Clear__t6VIPool1ZP13VISpellEffect(void* p) { events.push_back({4, off_of(p), 0}); }
extern "C" void Remove__12VIDictionaryR10VIResource(void* d, void* r) { events.push_back({5, off_of(d), off_of(r)}); }
extern "C" int Destroy__13VISpellEffect(void* e) {
    unsigned n = (off_of(e) - 0x4000) / 0x40;
    events.push_back({6, off_of(e), 0});
    return destroy_calls[n]++ == 0 ? destroy_result[n] : 0;
}
extern "C" void _$_13VISpellEffect(void* e, int f) { events.push_back({7, off_of(e), static_cast<unsigned>(f)}); }

int main() {
    const unsigned sys = 0x100, pool = sys + 1112, entries = 0x2000, dict = 0x3000;
    std::memset(arena, 0, sizeof arena);
    put(pool + 8, entries);
    // entry i (12 bytes) holds the effect pointer 0x4000 + 0x40 * i
    for (int i = 0; i < 4; ++i) put(entries + 12 * i, 0x4000 + 0x40 * i);
    live = {0, 1, 2, 3};
    // effect 0: count 1, registered, destroy ok. effect 1: count 3 (three passes), unregistered.
    // effect 2: count 1, registered, destroy fails once then succeeds. effect 3: count 0.
    put(0x4000 + 0, 1); put(0x4000 + 4, 1);
    put(0x4040 + 0, 0); put(0x4040 + 4, 3);
    put(0x4080 + 0, 1); put(0x4080 + 4, 1); destroy_result[2] = -1;
    put(0x40c0 + 0, 0); put(0x40c0 + 4, 0);
    if (ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary(arena + sys, arena + dict) != 0) return 1;
    // expected per-effect sequence, each iteration beginning with Begin and End
    struct Want { int kind; unsigned a; unsigned b; };
    std::vector<Want> w;
    auto iter = [&](){ w.push_back({1, pool, 0}); w.push_back({2, pool, 0}); };
    iter(); w.push_back({5, dict, 0x4000}); w.push_back({6, 0x4000, 0}); w.push_back({7, 0x4000, 3}); w.push_back({3, pool, 0});
    iter(); iter(); iter();     // count 3 -> 2 -> 1 -> 0 over three passes
    w.push_back({6, 0x4040, 0}); w.push_back({7, 0x4040, 3}); w.push_back({3, pool, 1});
    iter(); w.push_back({5, dict, 0x4080}); w.push_back({6, 0x4080, 0});    // destroy fails: restart
    iter(); w.push_back({5, dict, 0x4080}); w.push_back({6, 0x4080, 0}); w.push_back({7, 0x4080, 3}); w.push_back({3, pool, 2});
    iter(); w.push_back({6, 0x40c0, 0}); w.push_back({7, 0x40c0, 3}); w.push_back({3, pool, 3});
    iter(); w.push_back({4, pool, 0});
    if (events.size() != w.size()) return 2;
    for (unsigned i = 0; i < w.size(); ++i)
        if (events[i].kind != w[i].kind || events[i].a != w[i].a || events[i].b != w[i].b) return 3;
    // after the third pass of effect 1 the count word reads zero, effect 2 ends at -1 + ... see counts
    if (static_cast<int>(get(0x4040 + 4)) != 0) return 4;
    if (static_cast<int>(get(0x4080 + 4)) != -1) return 5;
    if (static_cast<int>(get(0x40c0 + 4)) != -1) return 6;
    return 0;
}
