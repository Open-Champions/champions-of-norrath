// Host contract for doThrottle__7Vehicle. Not an EE plan input. The table
// pointer at +72 is host-sized and no neighbouring pointer is read.
#include "do_throttle.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

unsigned char pad[4 * 88];
unsigned char player[4 * 23984];
int gameControlPaused;
signed char cutMode;

static int failures = 0;
static int on_calls, off_calls;
static unsigned char* on_arg;
static unsigned char* off_arg;
static void gas_on(unsigned char* s) { ++on_calls; on_arg = s; }
static void gas_off(unsigned char* s) { ++off_calls; off_arg = s; }

static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

static unsigned char v[2304];
static unsigned char vtable[256];
static unsigned char record[64];
static short off_delta() { short d; std::memcpy(&d, vtable + 184, 2); return d; }
static const float A = 0x1.aaaaa8p+2f;

static void setup(float x, int counter, int driver, int padbits) {
    std::memset(v, 0, sizeof v); std::memset(vtable, 0, sizeof vtable); std::memset(pad, 0, sizeof pad); std::memset(player, 0, sizeof player);
    std::memset(record, 0, sizeof record);
    short d1 = 0x10;
    void (*f1)(unsigned char*) = gas_on; void (*f2)(unsigned char*) = gas_off;
    // An 8-byte host function pointer at +180 covers the next entry's delta at +184,
    // so that delta is whatever the pointer's upper half holds (read back below).
    std::memcpy(vtable + 180, &f1, sizeof f1); std::memcpy(vtable + 188, &f2, sizeof f2);
    std::memcpy(vtable + 176, &d1, 2);
    unsigned char* vt = vtable; std::memcpy(v + 72, &vt, sizeof vt);
    at(v, 740) = x; v[109] = (unsigned char)counter; wd(v, 956) = driver;
    if (driver >= 0) { wd(pad, 8 + 88 * driver) = padbits; unsigned char* r = record; std::memcpy(player + 23984 * driver + 21872, &r, sizeof r); }
    gameControlPaused = 0; cutMode = 0; on_calls = off_calls = 0;
}

int main() {
    // Auto mode.
    setup(0.5f, 0, -1, 0); wd(v, 2148) = 1; at(v, 832) = 2.0f; at(v, 848) = 6.0f; at(v, 840) = 2.0f;
    doThrottle__7Vehicle(v);
    expect(near(at(v, 740), 0.5f + ((3.0f - 1.0f) * 0.5f) * 0x1.111110p-6f), "auto: x moves by the speed ratio difference");
    setup(0.99f, 0, -1, 0); wd(v, 2148) = 1; at(v, 832) = 1.0f; at(v, 848) = 100.0f; doThrottle__7Vehicle(v);
    expect(at(v, 740) == 1.0f, "auto: clamped to 1");
    // Button pressed (pad bit 0x40, no word +2152): counter 0 and x == 0 -> gas-on virtual with delta.
    setup(0.0f, 0, 1, 0x40);
    doThrottle__7Vehicle(v);
    expect(on_calls == 1 && on_arg == v + 0x10 && v[109] == 30 && at(v, 740) == 1.0f, "press from idle: gas on, counter 30, x clamps to 1");
    // Held: counter counts down and no virtual call.
    setup(0.5f, 5, 1, 0x40);
    doThrottle__7Vehicle(v);
    expect(on_calls == 0 && v[109] == 4 && at(v, 740) == 1.0f, "held: counter decrements, x up");
    // Released from full: gas-off virtual.
    setup(1.0f, 0, 1, 0x00);
    doThrottle__7Vehicle(v);
    expect(off_calls == 1 && off_arg == v + off_delta() && v[109] == 30 && near(at(v, 740), 1.0f - A < 0 ? 0.0f : 1.0f - A), "release from full: gas off, x down by A (floored at 0)");
    // Not at either end: no virtual, counter 0 stays.
    setup(0.5f, 0, 1, 0x00);
    doThrottle__7Vehicle(v);
    expect(off_calls == 0 && on_calls == 0 && v[109] == 0 && at(v, 740) == 0.0f, "mid throttle released: no virtual call");
    // Word +2152 selects bit 0x80 instead of 0x40.
    setup(0.0f, 0, 1, 0x80); wd(v, 2152) = 1; at(v, 840) = 10.0f;
    doThrottle__7Vehicle(v);
    expect(on_calls == 1, "bit 0x80 counts as pressed when +2152 is set");
    setup(0.0f, 0, 1, 0x40); wd(v, 2152) = 1; at(v, 840) = 10.0f;
    doThrottle__7Vehicle(v);
    expect(on_calls == 0, "bit 0x40 does not when +2152 is set");
    // cutMode takes the word at +1040.
    setup(0.0f, 0, 1, 0x00); cutMode = 1; wd(v, 1040) = 9;
    doThrottle__7Vehicle(v);
    expect(on_calls == 1, "cutMode: +1040 is the button");
    // Pause or +1036 forces released.
    setup(0.0f, 0, 1, 0x40); gameControlPaused = 1;
    doThrottle__7Vehicle(v);
    expect(on_calls == 0, "paused: released");
    setup(0.0f, 0, 1, 0x40); wd(v, 1036) = 1;
    doThrottle__7Vehicle(v);
    expect(on_calls == 0, "+1036: released");
    setup(0.0f, 0, 1, 0x40); wd(record, 12) = 0x400000;
    doThrottle__7Vehicle(v);
    expect(on_calls == 0, "record bit 0x400000: released");
    // +2152 follow-up: no driver clears it.
    setup(0.5f, 0, -1, 0); wd(v, 2152) = 1; at(v, 840) = 10.0f;
    doThrottle__7Vehicle(v);
    expect(wd(v, 2152) == 0, "+2152 cleared with no driver");
    // Released and slow: cleared.
    setup(0.5f, 0, 1, 0x00); wd(v, 2152) = 1; at(v, 840) = 0.5f;
    doThrottle__7Vehicle(v);
    expect(wd(v, 2152) == 0, "+2152 cleared when released and below 1.0");
    // Pressed (0x80 form), bit 0x40 also set, speed below 4: cleared; at/above 4: x744 = 1.
    setup(0.5f, 0, 1, 0xc0); wd(v, 2152) = 1; at(v, 840) = 3.0f;
    doThrottle__7Vehicle(v);
    expect(wd(v, 2152) == 0, "speed below 4 clears +2152");
    setup(0.5f, 0, 1, 0xc0); wd(v, 2152) = 1; at(v, 840) = 5.0f;
    doThrottle__7Vehicle(v);
    expect(wd(v, 2152) == 1 && at(v, 744) == 1.0f, "speed 4 or more sets +744 to 1.0");
    return failures ? 1 : 0;
}
