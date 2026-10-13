// Host contract for newWind__10StaticFirei; not an EE input. 32-bit words are offsets from a host
// base. The authored function was also compared with the original in a private R5900 run (120
// random-state scenarios; every original word reached).
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "new_wind.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x100;
static std::string calls;
static float cosValue, sinValue;
static unsigned rollValue;
extern "C" {
float icos__Fi(int a) { calls += "cos" + std::to_string(a) + ";"; return cosValue; }
float isin__Fi(int a) { calls += "sin" + std::to_string(a) + ";"; return sinValue; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
}
static void put16(unsigned char* base, unsigned o, int v) { short s = static_cast<short>(v); std::memcpy(base + o, &s, 2); }
static short get16(unsigned char* base, unsigned o) { short v; std::memcpy(&v, base + o, 2); return v; }
static void setup(float c, float s, int first, int last, int wrap, unsigned roll) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    cosValue = c; sinValue = s; rollValue = roll;
    unsigned recs = 0x1000; std::memcpy(obj + 1188, &recs, 4);
    put16(obj, 122, first); put16(obj, 120, last); put16(obj, 118, wrap);
    for (int i = 0; i < 16; ++i) { put16(arena + recs + 10 * i, 6, 100 + i); put16(arena + recs + 10 * i, 8, 200 + i); put16(arena + recs + 10 * i, 0, 7); }
}
int main() {
    int bad = 0;
    // c=1, s=0: half114 = 3, half116 = 0. Visits 1,2,3 (last=4): lane 2 picks index 2 only.
    setup(1.0f, 0.0f, 1, 4, 8, 2);
    newWind__10StaticFirei(obj, 77);
    if (calls != "cos77;sin77;rand;") { std::puts(calls.c_str()); bad = 1; }
    if (get16(obj, 114) != 3 || get16(obj, 116) != 0) { std::puts("fail halfwords"); bad = 1; }
    for (int i = 0; i < 16; ++i) {
        const short want6 = static_cast<short>(100 + i + (i == 2 ? 3 : 0));
        if (get16(arena + 0x1000 + 10 * i, 6) != want6 || get16(arena + 0x1000 + 10 * i, 8) != 200 + i || get16(arena + 0x1000 + 10 * i, 0) != 7) { std::printf("fail record %d\n", i); bad = 1; }
    }
    // c=0, s=-2 (normalized to -1): half114 = 0, half116 = -3. Wraps at 5: visits 3,4,0,1 (last=2), roll low bits 1 picks 1 only... and 5 would be lane 1 too but wraps first.
    setup(0.0f, -2.0f, 3, 2, 5, 0xfffffff5u);
    newWind__10StaticFirei(obj, 0);
    if (get16(obj, 114) != 0 || get16(obj, 116) != -3) { std::puts("fail halfwords 2"); bad = 1; }
    for (int i = 0; i < 16; ++i) {
        const bool picked = i == 1;      // indices 3,4,0,1 have lanes 3,0,0,1; roll & 3 is 1
        const short want8 = static_cast<short>(200 + i + (picked ? -3 : 0));
        if (get16(arena + 0x1000 + 10 * i, 8) != want8 || get16(arena + 0x1000 + 10 * i, 6) != 100 + i) { std::printf("fail wrap record %d\n", i); bad = 1; }
    }
    // equal bounds visit nothing, but the roll is still drawn
    setup(1.0f, 1.0f, 3, 3, 8, 3);
    newWind__10StaticFirei(obj, 5);
    if (calls != "cos5;sin5;rand;" || get16(obj, 114) != 2 || get16(obj, 116) != 2) { std::puts("fail equal bounds"); bad = 1; }
    for (int i = 0; i < 16; ++i) if (get16(arena + 0x1000 + 10 * i, 6) != 100 + i || get16(arena + 0x1000 + 10 * i, 8) != 200 + i) { std::puts("fail equal bounds records"); bad = 1; break; }
    // 16-bit wraparound of the sums
    setup(1.0f, 0.0f, 0, 1, 8, 0);
    put16(arena + 0x1000, 6, -2);
    newWind__10StaticFirei(obj, 1);
    if (get16(arena + 0x1000, 6) != 1) { std::puts("fail wraparound"); bad = 1; }
    return bad;
}
