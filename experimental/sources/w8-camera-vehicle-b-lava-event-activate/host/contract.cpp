// Host contract for eventActivate__11LavaTractor. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventActivate__11LavaTractor(void* object);
static unsigned char object[2400];
static int order = 0;
static char tokens[2];
static char names[2][40];
static int plays = 0, play_at[2], find_at[2], handle_given[2];
static void* header_of[2];
static int vol_handle = 0, vol_at = 0, persist[2], persist_at[2], persists = 0;
static float vol_value = -1.0f, play_volume[2];
static int seen_at_second_find_a = 0;
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    static int finds = 0; const int i = finds++ & 1;
    if (std::strcmp(lump, "tractor.lmp") != 0) std::fprintf(stderr, "bad lump\n");
    std::strncpy(names[i], name, 39); find_at[i] = ++order;
    if (i == 1) std::memcpy(&seen_at_second_find_a, object + 2360, 4);
    return &tokens[i];
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    const int i = plays++;
    play_at[i] = ++order; header_of[i] = h; play_volume[i] = v;
    if (o != object) std::fprintf(stderr, "bad object\n");
    handle_given[i] = 1000 + i * 111;
    return handle_given[i];
}
extern "C" void soundChangeSequenceVolume__Fif(int h, float v) { vol_handle = h; vol_value = v; vol_at = ++order; }
extern "C" void soundMakePersistant__Fi(int h) { persist[persists] = h; persist_at[persists] = ++order; ++persists; }
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)
int main() {
    eventActivate__11LavaTractor(object);
    EXPECT(plays == 2 && persists == 2, "call counts");
    EXPECT(std::strcmp(names[0], "lavatractor_idle.anm") == 0 && std::strcmp(names[1], "lavatractor_move.anm") == 0, "names");
    EXPECT(header_of[0] == &tokens[0] && header_of[1] == &tokens[1], "headers");
    EXPECT(play_volume[0] == 1.0f && play_volume[1] == 1.0f, "play volumes");
    EXPECT(find_at[0] < play_at[0] && play_at[0] < find_at[1] && find_at[1] < play_at[1], "find/play interleave");
    int a, b; std::memcpy(&a, object + 2360, 4); std::memcpy(&b, object + 2364, 4);
    EXPECT(a == 1000 && b == 1111, "handles stored");
    EXPECT(seen_at_second_find_a == 1000, "handle A stored before second lookup");
    EXPECT(vol_handle == 1111 && vol_value == 0.0f && vol_at == play_at[1] + 1, "volume of move sequence set to zero");
    EXPECT(persist[0] == 1000 && persist[1] == 1111 && persist_at[0] == vol_at + 1 && persist_at[1] == vol_at + 2, "persistence order");
    return 0;
}
