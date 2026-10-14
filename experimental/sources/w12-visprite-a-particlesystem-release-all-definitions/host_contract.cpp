#include "release_all.cpp"
#include <vector>

struct Event { int kind; const void* a; long b; const void* c; };
static std::vector<Event> events;
static std::vector<int> live;
extern "C" int Begin__Ct6VIPool1ZP20VIParticleDefinition(void* p) { events.push_back({1, p, 0, nullptr}); return live.empty() ? 99 : live[0]; }
extern "C" int End__Ct6VIPool1ZP20VIParticleDefinition(void* p) { events.push_back({2, p, 0, nullptr}); return 99; }
extern "C" void Clear__t6VIPool1ZP20VIParticleDefinition(void* p) { events.push_back({4, p, 0, nullptr}); }
extern "C" int ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(void* s, int i, void* d) {
    events.push_back({3, s, i, d});
    live.erase(live.begin());
    return 5;
}

int main() {
    alignas(8) static char object[256];
    static char dict[16];
    live = {7, 3, 9};
    if (ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary(object, dict) != 0) return 1;
    // three iterations of Begin, End, Release then a final Begin, End and Clear
    if (events.size() != 3 * 3 + 3) return 2;
    int ids[3] = {7, 3, 9};
    for (int k = 0; k < 3; ++k) {
        if (events[3 * k].kind != 1 || events[3 * k].a != object + 84) return 3;
        if (events[3 * k + 1].kind != 2 || events[3 * k + 1].a != object + 84) return 4;
        const Event& e = events[3 * k + 2];
        if (e.kind != 3 || e.a != object || e.b != ids[k] || e.c != dict) return 5;
    }
    if (events[9].kind != 1 || events[10].kind != 2) return 6;
    if (events[11].kind != 4 || events[11].a != object + 84) return 7;
    // empty pool: only Begin, End, Clear
    events.clear();
    if (ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary(object, dict) != 0 || events.size() != 3) return 8;
    return 0;
}
