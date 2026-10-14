// Host contract for gameLineGetNextPoint__FP17gameLinePathStateiP6Point3. Not an EE plan input.
// Build: c++ -std=c++17 host_check.cpp. A host pointer is 8 bytes; the record's pointer word at
// +20 holds one (the unit reads only +4 and +20 of the record).
#include <cstdio>
#include <cstring>
unsigned char* gameLines[100];
#include "game_line_get_next_point.cpp"
static unsigned char points[64 * 4];
static unsigned char record[40];
int main() {
    for (int i = 0; i != 4; ++i) {
        float v[3] = {1.f + i, 10.f + i, 100.f + i};
        std::memcpy(points + 64 * i, v, 12);
        std::memset(points + 64 * i + 12, 0x55, 52);
    }
    int count = 4; unsigned char* p = points;
    std::memcpy(record + 4, &count, 4);
    std::memcpy(record + 20, &p, sizeof p);
    gameLines[3] = record;
    unsigned char state[56]; std::memset(state, 0xcc, sizeof state);
    int line = 3, cur = 1;
    std::memcpy(state + 28, &line, 4); std::memcpy(state + 32, &cur, 4);
    unsigned char before[56]; std::memcpy(before, state, 56);
    struct { float f[3]; float guard; } out = {{0, 0, 0}, 123.f};
    gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(state, 1, reinterpret_cast<Point3Words*>(&out));
    if (out.f[0] != 3.f || out.f[1] != 12.f || out.f[2] != 102.f) return 1;
    if (out.guard != 123.f) return 2;                       // only 12 bytes written
    if (std::memcmp(state, before, 56) != 0) return 3;      // state unchanged
    gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(state, 3, reinterpret_cast<Point3Words*>(&out));
    if (out.f[0] != 1.f) return 4;                          // wraps: (1+3) % 4 == 0
    gameLineGetNextPoint__FP17gameLinePathStateiP6Point3(state, 0, reinterpret_cast<Point3Words*>(&out));
    if (out.f[0] != 2.f) return 5;
    std::puts("gameLineGetNextPoint check passed");
    return 0;
}
