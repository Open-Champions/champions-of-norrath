// Host contract for eventGasOff__11LavaTractor. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventGasOff__11LavaTractor(void* object);
static unsigned char object[16];
static unsigned rand_value = 0;
static int order = 0, rand_at = 0, find_at = 0, play_at = 0;
static char lump_seen[32], name_seen[160];
static void* header_seen = nullptr;
static void* play_object = nullptr;
static float play_volume = 0.0f;
static char token;
extern "C" unsigned eRand__10GameObject(void* o) { rand_at = ++order; return o == object ? rand_value : 0xfffffffu; }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    find_at = ++order;
    std::strncpy(lump_seen, lump, 31);
    std::strncpy(name_seen, name, 159);
    return &token;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    play_at = ++order; header_seen = h; play_object = o; play_volume = v; return 1;
}
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)
int main() {
    const unsigned inputs[6] = {0u, 1u, 2u, 3u, 7u, 0xfffffffeu};
    for (int i = 0; i < 6; ++i) {
        rand_value = inputs[i];
        order = rand_at = find_at = play_at = 0;
        header_seen = play_object = nullptr; play_volume = 0.0f; lump_seen[0] = name_seen[0] = 0;
        eventGasOff__11LavaTractor(object);
        char want[160];
        std::snprintf(want, sizeof want, "lavatractor_decelerate%d.anm", inputs[i] % 3u + 1u);
        EXPECT(rand_at == 1 && find_at == 2 && play_at == 3, "call order");
        EXPECT(std::strcmp(lump_seen, "tractor.lmp") == 0, "lump");
        EXPECT(std::strcmp(name_seen, want) == 0, "name");
        EXPECT(header_seen == &token && play_object == object && play_volume == 1.0f, "play args");
    }
    return 0;
}
