// Host contract for backupJustForces__7Vehicle. Not an EE plan input.
#include "backup_just_forces.cpp"

#include <cstdio>
#include <initializer_list>
#include <cstring>

static float get(const unsigned char* o, int off) { float v; std::memcpy(&v, o + off, 4); return v; }
static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static int getw(const unsigned char* o, int off) { int v; std::memcpy(&v, o + off, 4); return v; }

int main() {
    int failures = 0;
    const float k = 0x1.111110p-6f;
    unsigned char o[2304];
    std::memset(o, 0x2a, sizeof o);
    put(o, 672, 0.5f);
    put(o, 1224, 2.0f); put(o, 1228, 4.0f); put(o, 1232, 8.0f);
    put(o, 2004, 10.0f); put(o, 2008, 20.0f); put(o, 2012, 30.0f);
    put(o, 1992, 1.0f); put(o, 1996, 2.0f); put(o, 2000, 3.0f);
    put(o, 1188, 100.0f); put(o, 1192, 200.0f); put(o, 1196, 300.0f);
    put(o, 1200, 5.0f); put(o, 1204, 6.0f); put(o, 1208, 7.0f);
    int zero = 0; std::memcpy(o + 2164, &zero, 4);
    unsigned char before[sizeof o];
    std::memcpy(before, o, sizeof o);
    backupJustForces__7Vehicle(o);
    unsigned char expect[sizeof o];
    std::memcpy(expect, before, sizeof o);
    put(expect, 1188, 100.0f - (10.0f * 0.5f) * k);
    put(expect, 1192, 200.0f - (20.0f * 0.5f) * k);
    put(expect, 1196, 300.0f - (30.0f * 0.5f) * k);
    put(expect, 1200, 5.0f - (1.0f * 2.0f) * k);
    put(expect, 1204, 6.0f - (2.0f * 4.0f) * k);
    put(expect, 1208, 7.0f - (3.0f * 8.0f) * k);
    for (int off : {1992, 1996, 2000, 2004, 2008, 2012}) put(expect, off, 0.0f);
    int one = 1; std::memcpy(expect + 2164, &one, 4);
    if (std::memcmp(o, expect, sizeof o) != 0) { std::fprintf(stderr, "fail: first call\n"); ++failures; }
    // a second call is a no-op because the guard word is now set
    std::memcpy(before, o, sizeof o);
    put(o, 2004, 99.0f);
    std::memcpy(before, o, sizeof o);
    backupJustForces__7Vehicle(o);
    if (std::memcmp(o, before, sizeof o) != 0 || getw(o, 2164) != 1) { std::fprintf(stderr, "fail: guarded call wrote\n"); ++failures; }
    return failures ? 1 : 0;
}
