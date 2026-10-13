// Host contract for initPosition__FP17gameLinePathState. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// A host pointer is 8 bytes: the state's position pointer at +0 and the record's points
// pointer at +20 are host pointers, so the state's word at +28 and +32 sit at offsets that
// the unit reads/writes as 32-bit ints (offset 8..16 holds the pointer's tail only for +0).
#include <cstdio>
#include <cstring>
#include <cmath>
unsigned char* gameLines[100];
#include "game_line_init_position.cpp"
int main() {
    static unsigned char points[64 * 6];
    static unsigned char record[40];
    static float pos[3];
    unsigned char state[64];
    auto put = [&](int i, float x, float y, float z) {
        float v[3] = {x, y, z}; std::memcpy(points + 64 * i, v, 12);
        std::memset(points + 64 * i + 12, 0x77, 52);
    };
    auto run = [&](int count, int line) {
        std::memset(state, 0xcc, sizeof state);
        unsigned char* pp = reinterpret_cast<unsigned char*>(pos);
        std::memcpy(state, &pp, sizeof pp);
        std::memcpy(state + 28, &line, 4);
        std::memcpy(record + 4, &count, 4);
        unsigned char* q = points; std::memcpy(record + 20, &q, sizeof q);
        gameLines[line] = record;
        initPosition__FP17gameLinePathState(state);
    };
    auto idx = [&]() { int v; std::memcpy(&v, state + 32, 4); return v; };
    pos[0] = 0; pos[1] = 0; pos[2] = 0;
    put(0, 10, 0, 0); put(1, 0, 3, 4); put(2, 0, 0, 2); put(3, 1, 1, 1); put(4, 9, 9, 9);
    run(5, 4);
    if (idx() != 3) return 1;           // sqrt(3) is the nearest
    if (state[17] != 1) return 2;
    if (state[16] != 0xcc || state[18] != 0xcc || state[28] != 4) return 3;   // other bytes untouched
    pos[0] = 0; pos[1] = 0; pos[2] = 2.f; run(5, 4);
    if (idx() != 2) return 4;           // exact hit
    // A tie keeps the earlier point (strictly nearer replaces).
    put(0, 5, 0, 0); put(1, 0, 5, 0); put(2, 0, 0, 5); pos[0] = pos[1] = pos[2] = 0;
    run(3, 1);
    if (idx() != 0) return 5;
    put(0, 6, 0, 0); put(2, 0, 0, 3); run(3, 1);
    if (idx() != 2) return 6;
    // Only the first `count` points are considered.
    put(0, 9, 9, 9); put(1, 8, 8, 8); put(2, 0, 0, 0.5f); run(2, 1);
    if (idx() != 1) return 7;
    run(1, 1);
    if (idx() != 0) return 8;
    // A count of zero still stores index 0 and sets the byte.
    run(0, 1);
    if (idx() != 0 || state[17] != 1) return 9;
    // Each axis matters, including negative coordinates and a moved origin.
    pos[0] = -3; pos[1] = 4; pos[2] = -5;
    put(0, -3, 4, 0); put(1, 0, 4, -5); put(2, -3, 0, -5); put(3, -3, 4, -4);
    run(4, 2);
    if (idx() != 3) return 10;
    std::puts("initPosition (path state) check passed");
    return 0;
}
