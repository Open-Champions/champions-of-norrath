// Host contract for eventCurRPM__11LavaTractor. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventCurRPM__11LavaTractor(void* object);
static int handles[4], count = 0;
static float volumes[4];
extern "C" void soundChangeSequenceVolume__Fif(int h, float v) { if (count < 4) { handles[count] = h; volumes[count] = v; } ++count; }
static unsigned char object[2400];
static void put(int off, int v) { std::memcpy(object + off, &v, 4); }
static void putf(int off, float v) { std::memcpy(object + off, &v, 4); }
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)
int main() {
    put(2360, 41); put(2364, 42); putf(740, 0.25f);
    put(956, -1);
    eventCurRPM__11LavaTractor(object);
    EXPECT(count == 0, "-1 does nothing");
    put(956, 0);
    eventCurRPM__11LavaTractor(object);
    EXPECT(count == 2, "two calls");
    EXPECT(handles[0] == 41 && volumes[0] == 0.75f, "idle gets 1 - rpm");
    EXPECT(handles[1] == 42 && volumes[1] == 0.25f, "move gets rpm");
    count = 0; put(956, 7); putf(740, 0.0f);
    eventCurRPM__11LavaTractor(object);
    EXPECT(count == 2 && volumes[0] == 1.0f && volumes[1] == 0.0f, "rpm zero");
    count = 0; put(956, 0x7fffffff);
    eventCurRPM__11LavaTractor(object);
    EXPECT(count == 2, "only -1 skips");
    return 0;
}
