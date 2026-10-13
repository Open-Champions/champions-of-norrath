// Host check only. Not an EE build input.
#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" const unsigned norrath_rtti_entities_a_view_480cb4 = 0x3db851eb;
extern "C" const unsigned norrath_rtti_entities_a_view_480cb8 = 0x3f199999;
extern "C" const unsigned norrath_rtti_entities_a_view_480cbc = 0x3f68ba2f;
extern "C" const unsigned norrath_rtti_entities_a_view_480cc0 = 0x45081000;
#include "add_turn_animation.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static float get_f(const unsigned char* b, unsigned off) { float v; std::memcpy(&v, b + off, 4); return v; }
static int get_i(const unsigned char* b, unsigned off) { int v; std::memcpy(&v, b + off, 4); return v; }
static void put_f(unsigned char* b, unsigned off, float v) { std::memcpy(b + off, &v, 4); }

// Independent restatement of the model using named constants.
static void model(short half, float pos, float rate, float* npos, float* nrate, int* count, float* pos_out, float* neg_out) {
    float target = float(half) / 512.0f;
    if (target > 1.0f) target = 1.0f;
    if (target < -1.0f) target = -1.0f;
    float r2 = rate + (target - pos) * 0.09f;
    float p2 = pos + r2;
    *nrate = r2 - r2 * 0.6f;
    *npos = p2;
    const float out = p2 * 0.9090909f;
    *count = int(out * 2177.0f);
    *pos_out = out > 0 ? out : 0.0f;
    *neg_out = out > 0 ? 0.0f : -out;
}

int main() {
    unsigned char o[1100];
    const short halves[] = {0, 1, -1, 256, 511, 512, 700, -700, 32767, -32768};
    const float positions[] = {0.0f, 0.5f, -0.5f, 0.99f, -0.01f, 2.0f};
    const float rates[] = {0.0f, 0.25f, -0.3f};
    for (short h : halves)
        for (float p : positions)
            for (float r : rates) {
                std::memset(o, 0x77, sizeof o);
                std::memcpy(o + 284, &h, 2);
                put_f(o, 980, p); put_f(o, 984, r);
                unsigned char before[1100];
                std::memcpy(before, o, sizeof o);
                addTurnAnimation__3Cat(o);
                float np, nr, po, ng; int count;
                model(h, p, r, &np, &nr, &count, &po, &ng);
                // the constants are single-precision, so allow a tiny tolerance against the double-ish model
                expect(std::fabs(get_f(o, 980) - np) <= 1e-5f * (1.0f + std::fabs(np)), "position");
                expect(std::fabs(get_f(o, 984) - nr) <= 1e-5f * (1.0f + std::fabs(nr)), "rate");
                expect(std::abs(get_i(o, 976) - count) <= 1, "count");
                expect(std::fabs(get_f(o, 368) - po) <= 1e-4f * (1.0f + po) && std::fabs(get_f(o, 400) - ng) <= 1e-4f * (1.0f + ng), "pair");
                expect(get_f(o, 368) == 0.0f || get_f(o, 400) == 0.0f, "one side is zero");
                for (unsigned i = 0; i < sizeof o; ++i) {
                    const bool written = (i >= 976 && i < 988) || (i >= 368 && i < 372) || (i >= 400 && i < 404);
                    if (!written && o[i] != before[i]) { expect(false, "stray write"); break; }
                }
            }
    // exact case: half 512 -> target 1.0; pos 0, rate 0: rate = 0.09, pos = 0.09
    std::memset(o, 0, sizeof o);
    short h = 512; std::memcpy(o + 284, &h, 2);
    addTurnAnimation__3Cat(o);
    expect(std::fabs(get_f(o, 980) - 0.09f) < 1e-7f, "first step position");
    expect(std::fabs(get_f(o, 984) - 0.09f * 0.4f) < 1e-7f, "first step rate");
    expect(get_f(o, 400) == 0.0f && get_f(o, 368) > 0.0f, "positive side");
    // clamp: a huge half behaves like 512
    unsigned char o2[1100]; std::memset(o2, 0, sizeof o2);
    h = 20000; std::memcpy(o2 + 284, &h, 2);
    addTurnAnimation__3Cat(o2);
    expect(get_f(o2, 980) == get_f(o, 980) && get_f(o2, 984) == get_f(o, 984), "target clamps at 1");
    if (failures == 0) std::printf("addTurnAnimation__3Cat cases passed\n");
    return failures == 0 ? 0 : 1;
}
