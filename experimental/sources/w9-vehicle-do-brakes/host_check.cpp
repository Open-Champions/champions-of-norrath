// Host contract for doBrakes__7Vehicle. Not an EE plan input.
#include "do_brakes.cpp"

#include <cstdio>
#include <cstring>

unsigned char pad[4 * 88];
unsigned char player[4 * 23984];
int gameControlPaused;

static int failures = 0;
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static unsigned char vehicle[2304];
static unsigned char record[64];

static void setup(int driver, int padbits, int paused, bool has_record, int record_word, int w1036, float x, float speed) {
    std::memset(vehicle, 0, sizeof vehicle);
    wd(vehicle, 956) = driver; wd(vehicle, 1036) = w1036; at(vehicle, 744) = x; at(vehicle, 840) = speed;
    std::memset(pad, 0, sizeof pad); std::memset(player, 0, sizeof player);
    if (driver >= 0) wd(pad, 8 + 88 * driver) = padbits;
    gameControlPaused = paused;
    wd(record, 12) = record_word;
    if (driver >= 0 && has_record) { unsigned char* r = record; std::memcpy(player + 23984 * driver + 21872, &r, sizeof r); }
}
static float run() { doBrakes__7Vehicle(vehicle); return at(vehicle, 744); }
static bool near(float a, float b) { return a > b - 1e-6f && a < b + 1e-6f; }

int main() {
    const float A = 0x1.111110p-5f, B = 0x1.111110p-3f;
    // Mode with +2148 set.
    setup(-1, 0, 0, false, 0, 0, 0.3f, 5.0f); wd(vehicle, 2148) = 1; at(vehicle, 848) = 4.0f;
    expect(run() == 1.0f, "+2148 set: 1.0 when +848 < +840");
    at(vehicle, 848) = 6.0f; expect(run() == 0.0f, "+2148 set: 0.0 otherwise");
    // +2152 set: nothing.
    setup(-1, 0, 0, false, 0, 0, 0.3f, 5.0f); wd(vehicle, 2152) = 1;
    expect(run() == 0.3f, "+2152 set: untouched");
    // No driver: up.
    setup(-1, 0, 0, false, 0, 0, 0.3f, 5.0f);
    expect(near(run(), 0.3f + A) && wd(vehicle, 2152) == 0, "no driver goes up, no stop flag");
    // Driver, pad bit clear: word +1036 decides.
    setup(2, 0x00, 0, false, 0, 0, 0.5f, 5.0f); expect(near(run(), 0.5f - B), "bit clear, +1036 zero: down");
    setup(2, 0x00, 0, false, 0, 1, 0.5f, 5.0f); expect(near(run(), 0.5f + A), "bit clear, +1036 set: up");
    // Bit set but paused: same as bit clear.
    setup(1, 0x80, 1, true, 0x400000, 0, 0.5f, 5.0f); expect(near(run(), 0.5f - B), "paused: +1036 zero: down");
    // Bit set, not paused: record rules.
    setup(1, 0x80, 0, false, 0, 0, 0.5f, 5.0f); expect(near(run(), 0.5f + A), "no record: up");
    setup(1, 0x80, 0, true, 0x000000, 0, 0.5f, 5.0f); expect(near(run(), 0.5f + A), "record bit clear: up");
    setup(1, 0x80, 0, true, 0x400000, 0, 0.5f, 5.0f); expect(near(run(), 0.5f - B), "record bit set, +1036 zero: down");
    setup(1, 0x80, 0, true, 0x400000, 1, 0.5f, 5.0f); expect(near(run(), 0.5f + A), "record bit set, +1036 set: up");
    // Going up with a driver and low speed stops the counter.
    setup(1, 0x80, 0, false, 0, 0, 0.5f, 1.5f);
    expect(run() == 0.0f && wd(vehicle, 2152) == 1, "up at low speed: cleared and flagged");
    // Clamps.
    setup(-1, 0, 0, false, 0, 0, 0.99f, 5.0f); expect(run() == 1.0f, "clamped to 1");
    setup(1, 0, 0, false, 0, 0, 0.01f, 5.0f); expect(run() == 0.0f, "clamped to 0");
    return failures ? 1 : 0;
}
