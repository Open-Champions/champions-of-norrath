#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
int scanMask;
void drawTexture__FR6Packetiiiiiiiii(unsigned char* packet, int i1, int i2, int i3, int i4, int i5,
                                    int i6, int i7, int i8, int i9);
}
static long long dword(const unsigned char* p) { long long v; std::memcpy(&v, p, 8); return v; }
int main() {
    unsigned char* packet = arena_at(0x100);
    unsigned char* buf = arena_at(0x1000);
    auto reset = [&] { std::memset(buf, 0xee, 128); sw_(packet, norrath_host_address(buf)); };
    // Even x (i3 & 0x10 == 0) and scanMask 3: block A, cursor advances by 32.
    reset(); scanMask = 3;
    drawTexture__FR6Packetiiiiiiiii(packet, 10, 100, 0x20, 300, 5, 7, 8, 9, 11);
    if (hw(packet) != norrath_host_address(buf) + 32) return 1;
    if (dword(buf) != (long long)(7 + 4) + ((long long)(8 + 4) << 16)) return 2;
    if (dword(buf + 8) != 100 + (0x20ll << 16)) return 3;
    if (dword(buf + 16) != (9 + 4) + ((11ll + 4) << 16)) return 4;
    if (dword(buf + 24) != 300 + (5ll << 16)) return 5;
    if (buf[32] != 0xee) return 6;
    // Even x and scanMask 2: block B with h = (i1 >> 1) << 4 added to i7 and i9.
    reset(); scanMask = 2;
    drawTexture__FR6Packetiiiiiiiii(packet, 10, 100, 0x20, 300, 5, 7, 8, 9, 11);
    if (hw(packet) != norrath_host_address(buf) + 32) return 7;
    if (dword(buf) != (7 + 4) + ((long long)(80 + 8) << 16)) return 8;
    if (dword(buf + 16) != (9 + 4) + ((long long)(80 + 11) << 16)) return 9;
    if (dword(buf + 8) != 100 + (0x20ll << 16) || dword(buf + 24) != 300 + (5ll << 16)) return 10;
    // Odd x swaps the roles: scanMask 2 writes block A, scanMask 3 block B.
    reset(); scanMask = 2;
    drawTexture__FR6Packetiiiiiiiii(packet, 10, 100, 0x10, 300, 5, 7, 8, 9, 11);
    if (dword(buf) != (7 + 4) + ((long long)(8 + 4) << 16)) return 11;
    reset(); scanMask = 3;
    drawTexture__FR6Packetiiiiiiiii(packet, 10, 100, 0x10, 300, 5, 7, 8, 9, 11);
    if (dword(buf) != (7 + 4) + ((long long)(80 + 8) << 16)) return 12;
    // Other scanMask values write nothing.
    reset(); scanMask = 1;
    drawTexture__FR6Packetiiiiiiiii(packet, 10, 100, 0x20, 300, 5, 7, 8, 9, 11);
    if (hw(packet) != norrath_host_address(buf) || buf[0] != 0xee) return 13;
    // A set bit 31 sign-extends into the upper half of the 64-bit value.
    reset(); scanMask = 3;
    drawTexture__FR6Packetiiiiiiiii(packet, 0, 0, 0, 0, 0, 0, 0x8000, 0, 0);
    if ((unsigned long long)dword(buf) != 0xffffffff80040004ull) { std::printf("%llx\n", dword(buf)); return 14; }
    std::puts("drawTexture(9 ints) contract passed");
    return 0;
}
