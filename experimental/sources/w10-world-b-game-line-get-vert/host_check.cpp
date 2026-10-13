// Host contract for gameLineGetVert__Fii. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
// On the host a pointer is 8 bytes, so the record's pointer word at +20 holds a host pointer
// (the unit reads only +4 and +20).
#include <cstdio>
#include <cstring>
unsigned char* gameLines[100];
#include "game_line_get_vert.cpp"
static unsigned char points[64 * 8];
static unsigned char record[40];
int main() {
    int count = 5;
    unsigned char* p = points;
    std::memcpy(record + 4, &count, 4);
    std::memcpy(record + 20, &p, sizeof p);
    gameLines[7] = record;
    if (gameLineGetVert__Fii(7, 0) != points) return 1;
    if (gameLineGetVert__Fii(7, 3) != points + 192) return 2;
    if (gameLineGetVert__Fii(7, 5) != points) return 3;       // wraps at the count
    if (gameLineGetVert__Fii(7, 12) != points + 128) return 4;
    if (gameLineGetVert__Fii(7, -1) != points - 64) return 5; // signed remainder stays negative
    static unsigned char other[40];
    int c2 = 2; unsigned char* q = points + 64;
    std::memcpy(other + 4, &c2, 4); std::memcpy(other + 20, &q, sizeof q);
    gameLines[0] = other;
    if (gameLineGetVert__Fii(0, 3) != points + 128) return 6;  // line number picks the record
    std::puts("gameLineGetVert check passed");
    return 0;
}
