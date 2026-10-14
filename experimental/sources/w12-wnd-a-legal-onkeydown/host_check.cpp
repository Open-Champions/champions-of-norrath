// Host check only; not a build input. Scripts the timer: for key 144 the first read is compared
// with the 64-bit stamp at +40 through the low 32 bits as a signed value (below 5001 returns); on
// a pass the second read minus 7000 is stored. Other keys never read the timer.
#include "onkeydown.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

static long long now_values[2];
static int reads;
extern "C" long long GetTime__7VITimer() { return now_values[reads++ < 1 ? 0 : 1]; }

int main() {
    bool ok = true;
    for (int key : {0, 13, 143, 145, -144, 400}) {
        alignas(8) unsigned char object[64];
        std::memset(object, 0x5a, sizeof object);
        reads = 0;
        now_values[0] = now_values[1] = 99999;
        OnKeyDown__10VIWndLegali(object, key);
        for (unsigned char b : object) ok = ok && b == 0x5a;
        ok = ok && reads == 0;
    }
    struct Case { long long stamp, now1, now2; bool stores; };
    const Case cases[] = {
        {1000, 6000, 7777, false},              // 5000 elapsed: still waiting
        {1000, 6001, 7777, true},               // 5001 elapsed: reset
        {1000, 500, 7777, false},               // negative elapsed
        {0, 0x100000000LL + 6000, 123456, true}, // only the low 32 bits count
        {0, 0x100000000LL + 100, 123456, false},
        {-5000, 1, 50000, true},                // 5001 across zero
        {0, 0x80000000LL, 9, false},             // low word is negative as a signed value
        {0, 5000, 9, false},
    };
    for (const Case& c : cases) {
        alignas(8) unsigned char object[64];
        std::memset(object, 0x5a, sizeof object);
        std::memcpy(object + 40, &c.stamp, 8);
        reads = 0;
        now_values[0] = c.now1;
        now_values[1] = c.now2;
        OnKeyDown__10VIWndLegali(object, 144);
        long long stamp;
        std::memcpy(&stamp, object + 40, 8);
        ok = ok && stamp == (c.stores ? c.now2 - 7000 : c.stamp) && reads == (c.stores ? 2 : 1);
        for (int i = 0; i < 64; ++i) if (i < 40 || i >= 48) ok = ok && object[i] == 0x5a;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
