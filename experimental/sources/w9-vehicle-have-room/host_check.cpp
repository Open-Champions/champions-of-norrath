// Host contract for haveRoom__7Vehicle. Not an EE plan input.
#include "have_room.cpp"

#include <cstdio>
#include <cstring>

static void put_int(unsigned char* o, int off, int v) { std::memcpy(o + off, &v, 4); }
static void put_float(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }

int main() {
    int failures = 0;
    struct Case { int count, used; float f; int want; } cases[] = {
        {4, 0, 0.0f, 1}, {4, 3, 0.0f, 1}, {4, 4, 0.0f, 0}, {4, 5, 0.0f, 0},
        {4, 0, 1.5f, 0}, {4, 0, -0.0f, 1}, {0, -1, 0.0f, 1}, {-1, -2, 0.0f, 1},
        {-2, -1, 0.0f, 0}, {4, 0, __builtin_nanf(""), 0},
    };
    for (const Case& c : cases) {
        unsigned char o[2304];
        std::memset(o, 0x6b, sizeof o);
        put_int(o, 976, c.count); put_int(o, 980, c.used); put_float(o, 808, c.f);
        unsigned char before[sizeof o];
        std::memcpy(before, o, sizeof o);
        if (haveRoom__7Vehicle(o) != c.want) { std::fprintf(stderr, "fail: count %d used %d\n", c.count, c.used); ++failures; }
        if (std::memcmp(before, o, sizeof o) != 0) { std::fprintf(stderr, "fail: wrote\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
