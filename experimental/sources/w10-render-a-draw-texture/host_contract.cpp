// Host contract for drawTexture__FR6PacketP8_textureiiiiii. Not an EE plan input.
#define NORRATH_HOST_ARENA 1
#include <cstdio>
#include <cstring>
extern "C" {
int engineFullFrameMode;
signed char supersampleMode;
static int fieldCalls, fieldValue[4];
int engineGetField__Fv() { return fieldValue[fieldCalls++ & 3]; }
}
#include "draw_texture.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
static unsigned long long qlo(const unsigned char* base, int i) { return hw(base + 16 * i) | (static_cast<unsigned long long>(hw(base + 16 * i + 4)) << 32); }
static unsigned long long qhi(const unsigned char* base, int i) { return hw(base + 16 * i + 8) | (static_cast<unsigned long long>(hw(base + 16 * i + 12)) << 32); }
int main() {
    unsigned char* packet = arena_at(0x10);
    unsigned char* texture = arena_at(0x40);
    unsigned char* buffer = arena_at(0x1000);
    const unsigned long long E = 0xA00000000ull;
    sh_(texture, 64); sh_(texture + 2, 32);                          // W = 64, H = 32
    auto reset = [&] { sw_(packet, norrath_host_address(buffer)); std::memset(buffer, 0xdd, 256); };
    // Full-frame mode with an x whose bit 0x10 is clear: p3 = 0 gives x = 29184.
    engineFullFrameMode = 1; reset();
    drawTexture__FR6PacketP8_textureiiiiii(packet, texture, 1, 0, 2, 3, 4, 5);
    const unsigned long long x = 29184, y = (1 << 5) + 22528;
    if (hw(packet) != norrath_host_address(buffer) + 176) return 1;
    if (qlo(buffer, 0) != 3 || qhi(buffer, 0) != 34 || qlo(buffer, 5) != 2 || qhi(buffer, 5) != 34) return 2;
    if (qlo(buffer, 1) != (((2u << 4) + 4) | (((3u << 3) + 4) << 16)) || qhi(buffer, 1) != 3) return 3;
    if (qlo(buffer, 2) != ((y | (x << 16)) | E) || qhi(buffer, 2) != 5) return 4;
    if (qlo(buffer, 3) != (((4u << 4) + 4) | (((5u << 3) + 4) << 16)) || qhi(buffer, 3) != 3) return 5;
    if (qlo(buffer, 4) != (((y + (64u << 5)) | ((x + (32u << 4)) << 16)) | E) || qhi(buffer, 4) != 5) return 6;
    if (qlo(buffer, 6) != (((2u << 4) + 4) | (((16u << 4) + (3u << 3)) << 16)) || qhi(buffer, 6) != 3) return 7;
    if (qlo(buffer, 8) != (((4u << 4) + 4) | (((16u << 4) + (5u << 3)) << 16)) || qhi(buffer, 8) != 3) return 8;
    if (qlo(buffer, 7) != qlo(buffer, 2) || qlo(buffer, 9) != qlo(buffer, 4) || qlo(buffer, 10) != 0 || qhi(buffer, 10) != 34) return 9;
    if (buffer[176] != 0xdd) return 10;
    // Plain mode.
    engineFullFrameMode = 0; supersampleMode = 0; reset();
    drawTexture__FR6PacketP8_textureiiiiii(packet, texture, 1, 0, 2, 3, 4, 5);
    const unsigned long long y3 = (1 << 4) + 27648;
    if (hw(packet) != norrath_host_address(buffer) + 64 || buffer[64] != 0xdd) return 11;
    if (qlo(buffer, 0) != (((2u << 4) + 8) | (0x80000u + (3u << 20))) || qhi(buffer, 0) != 3) return 12;
    if (qlo(buffer, 1) != ((y3 | (x << 16)) | E) || qhi(buffer, 1) != 5) return 13;
    if (qlo(buffer, 2) != (((4u << 4) + 8) | (0x80000u + (5u << 20))) || qhi(buffer, 2) != 3) return 14;
    if (qlo(buffer, 3) != (((y3 + (64u << 4)) | ((x + (32u << 4)) << 16)) | E) || qhi(buffer, 3) != 5) return 15;
    // Supersample mode, x&0x10 clear (p3 = 0), field low bit clear: f = ((H>>1)<<4) - 4 = 252.
    supersampleMode = 1; fieldCalls = 0; fieldValue[0] = 0; reset();
    drawTexture__FR6PacketP8_textureiiiiii(packet, texture, 1, 0, 2, 3, 4, 5);
    if (fieldCalls != 1 || hw(packet) != norrath_host_address(buffer) + 64) return 16;
    if (qlo(buffer, 0) != (((2u << 4) + 4) | (((3u << 3) + 252 + 4) << 16)) || qhi(buffer, 0) != 3) return 17;
    if (qlo(buffer, 1) != ((y | (x << 16)) | E)) return 18;
    // Field low bit set: f = 0.
    fieldCalls = 0; fieldValue[0] = 1; reset();
    drawTexture__FR6PacketP8_textureiiiiii(packet, texture, 1, 0, 2, 3, 4, 5);
    if (qlo(buffer, 2) != (((4u << 4) + 4) | (((5u << 3) + 4) << 16))) return 19;
    // x&0x10 set (p3 = 1 gives 29200): two field reads; first even moves x down by 16, second even gives f = -4.
    fieldCalls = 0; fieldValue[0] = 0; fieldValue[1] = 0; reset();
    drawTexture__FR6PacketP8_textureiiiiii(packet, texture, 1, 1, 2, 3, 4, 5);
    const unsigned long long xd = 29200 - 16;
    if (fieldCalls != 2 || qlo(buffer, 1) != ((y | (xd << 16)) | E)) return 20;
    if (qlo(buffer, 0) != (((2u << 4) + 4) | ((unsigned long long)(unsigned)((3 << 3) - 4 + 4) << 16))) return 21;
    std::puts("drawTexture contract passed");
    return 0;
}
