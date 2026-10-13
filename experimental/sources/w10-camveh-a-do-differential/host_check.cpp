// Host contract for doDifferential__7Vehicle. Not an EE plan input.
// The object keeps 4-byte-stride pointers; the host's are 8 bytes wide, so the
// two wheel objects are mapped at addresses whose halves line up
// (0x400000003 and 0x300000004 read from words 3, 4, 3).
#include "do_differential.cpp"

#include <cstdio>
#include <cstring>
#include <sys/mman.h>

struct Call { char kind; unsigned char* wheel; float value; };
static Call calls[16];
static int ncalls;
extern "C" void SetNaturalRPM__5Wheel(unsigned char* w) { calls[ncalls++] = {'S', w, 0}; }
extern "C" void ModifyRPM__5Wheelf(unsigned char* w, float v) { calls[ncalls++] = {'M', w, v}; }

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static void putw(unsigned char* o, int off, int v) { std::memcpy(o + off, &v, 4); }

int main() {
    int failures = 0;
    unsigned char* w0 = (unsigned char*)mmap((void*)0x400000000ull, 0x4000, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0);
    unsigned char* w1 = (unsigned char*)mmap((void*)0x300000000ull, 0x4000, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0);
    if (w0 != (void*)0x400000000ull || w1 != (void*)0x300000000ull) { std::fprintf(stderr, "fail: cannot map the wheel pages\n"); return 2; }
    unsigned char* wheel0 = w0 + 3;  // 0x400000003
    unsigned char* wheel1 = w1 + 4;  // 0x300000004
    float spec[4] = {600.0f, 0, 0, 0};
    // vehicle words at 1052.. = 1, 2, 3 ; spec pointer at 2052 (8 bytes, set directly)
    struct Case { float rpm, neutral, pre; int n, drive0, drive1, reverse; } cases[] = {
        {100.0f, 0.25f, 10.0f, 2, 0, 0, 0},
        {100.0f, 0.25f, 10.0f, 2, 1, 1, 0},
        {100.0f, 0.5f, 10.0f, 2, 1, 0, 1},
        {-50.0f, 0.5f, 10.0f, 2, 1, 1, 0},
        {0.0f, 0.5f, 10.0f, 1, 1, 0, 0},
        {100.0f, 0.5f, 10.0f, 0, 1, 1, 0},
        {100.0f, 0.5f, 10.0f, -3, 1, 1, 0},
    };
    for (const Case& c : cases) {
        unsigned char v[2304];
        std::memset(v, 0, sizeof v);
        putw(v, 1052, 3); putw(v, 1056, 4); putw(v, 1060, 3);
        unsigned char* sp = (unsigned char*)spec;
        std::memcpy(v + 2052, &sp, sizeof sp);
        put(v, 748, c.rpm); put(v, 744, c.neutral); put(v, 912, 2.0f); putw(v, 1000, c.n); putw(v, 2152, c.reverse);
        put(wheel0, 364, c.pre); put(wheel1, 364, c.pre);
        putw(wheel0, 388, c.drive0); putw(wheel1, 388, c.drive1);
        ncalls = 0;
        doDifferential__7Vehicle(v);
        // expectation
        Call want[16]; int nw = 0;
        float old = c.rpm, now = old < 0 ? 0.0f : old;
        float ratio = old == 0.0f ? 1.0f : now / old;
        unsigned char* ws[2] = {wheel0, wheel1};
        for (int i = 0; i < c.n && i < 2; ++i) {
            want[nw++] = {'S', ws[i], 0};
            want[nw++] = {'M', ws[i], c.pre * ratio};
            if (i == 0 ? c.drive0 : c.drive1) {
                float s = now / (spec[0] / 2.0f);
                if (c.reverse) s = -s; else if (s < 0) s = 0;
                want[nw++] = {'M', ws[i], s * (1.0f - c.neutral)};
            }
        }
        bool ok = nw == ncalls;
        for (int i = 0; ok && i < nw; ++i) ok = calls[i].kind == want[i].kind && calls[i].wheel == want[i].wheel && calls[i].value == want[i].value;
        float stored; std::memcpy(&stored, v + 748, 4);
        if (stored != now) ok = false;
        if (!ok) { std::fprintf(stderr, "fail: rpm %g n %d drive %d %d reverse %d\n", c.rpm, c.n, c.drive0, c.drive1, c.reverse); ++failures; }
    }
    return failures ? 1 : 0;
}
