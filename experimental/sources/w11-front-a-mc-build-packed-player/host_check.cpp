// Host check for MC_BuildPackedPlayer__FP11playerEntryi (source: ../w10-front-a-mc-build-packed-player).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// The stubs record the call order and every argument; the check fails on a wrong entry stride,
// byte offset, argument order, signedness or call order.
#include "../w10-front-a-mc-build-packed-player/mc_build_packed_player.cpp"

#include <cstdio>
#include <cstring>

namespace {
struct Call {
    int kind;  // 1 = playerInit, 2 = convertPointersToIndicies
    int a, b;
    char* name;
    int c, d, e, f;
    char* tint;
} calls[8];
int ncalls;
}

extern "C" void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char c1, signed char c2,
                                            signed char c3, signed char c4, char* tint) {
    calls[ncalls++] = Call{1, index, second, name, c1, c2, c3, c4, tint};
}
extern "C" void convertPointersToIndicies__Fi(int index) { calls[ncalls++] = Call{2, index, 0, nullptr, 0, 0, 0, 0, nullptr}; }

int main() {
    unsigned char table[41 * 4];
    for (unsigned i = 0; i < sizeof table; ++i) table[i] = static_cast<unsigned char>(0x80 + i * 3);
    for (int n = 0; n < 4; ++n) {
        ncalls = 0;
        MC_BuildPackedPlayer__FP11playerEntryi(table, n);
        if (ncalls != 2) return 1;
        const unsigned char* e = table + 41 * n;
        const Call& p = calls[0];
        if (p.kind != 1 || p.a != n) return 2;
        if (p.b != static_cast<signed char>(e[32])) return 3;
        if (p.name != reinterpret_cast<const char*>(e)) return 4;
        if (p.c != static_cast<signed char>(e[33]) || p.d != static_cast<signed char>(e[34])) return 5;
        if (p.e != static_cast<signed char>(e[36]) || p.f != static_cast<signed char>(e[35])) return 6;
        if (p.tint != reinterpret_cast<const char*>(e + 37)) return 7;
        if (calls[1].kind != 2 || calls[1].a != n) return 8;
    }
    // negative bytes must stay negative (signed char arguments)
    table[32] = 0xff;
    ncalls = 0;
    MC_BuildPackedPlayer__FP11playerEntryi(table, 0);
    if (calls[0].b != -1) return 9;
    std::puts("build packed player passed");
    return 0;
}
