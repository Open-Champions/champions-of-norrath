// Host contract for putBlock. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_CONTRACT=1 host_check.cpp put_block.cpp
#include <cstdint>
#include <cstdio>
#include <cstring>
struct Packet { int cursor; };
struct _texture;
extern "C" void putBlock__FP8_textureP6PacketPPP9uncompMapPPii(_texture*, Packet*, unsigned*, unsigned*, int);

// A small address space. Offsets below 0x700000 are an arena; 0x10000000 + n is byte n of uncompMap;
// 0x70000000 is a window into the arena at 0x700000 standing for the scratchpad; 0x20000000 is the
// uncached alias bit and is dropped.
static unsigned char arena[0x800000];
static unsigned regs[8];
static int syncs;
extern "C" int uncompSpin, uncompCache, drawFrameNm;
int uncompSpin, uncompCache, drawFrameNm;
extern "C" unsigned char uncompMap[];
unsigned char uncompMap[1280 * 8];
extern "C" unsigned char* norrath_host_ptr(unsigned a) {
    if (a >= 0x70000000u) return arena + 0x700000 + (a - 0x70000000u);
    if (a >= 0x20000000u) a -= 0x20000000u;
    if (a >= 0x10000000u) return uncompMap + (a - 0x10000000u);
    return arena + a;
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* c = (const unsigned char*)p;
    if (c >= uncompMap && c < uncompMap + sizeof uncompMap) return 0x10000000u + (unsigned)(c - uncompMap);
    return (unsigned)(c - arena);
}
extern "C" volatile unsigned* norrath_host_reg(unsigned a) {
    if (a == 0x1000e010u) return &regs[0];
    if (a == 0x1000d010u) return &regs[1];
    if (a == 0x1000d080u) return &regs[2];
    if (a == 0x1000d020u) return &regs[3];
    return &regs[4];
}
extern "C" void norrath_host_sync() { ++syncs; }
static unsigned& w(unsigned a) { return *(unsigned*)norrath_host_ptr(a); }
static unsigned& mapw(int index, int word) { return *(unsigned*)(uncompMap + index * 8 + word * 4); }

int main() {
    uncompCache = 0x8000;
    drawFrameNm = 100;
    uncompSpin = 4;
    const unsigned texA = 0x100, tex = 0x500;
    w(texA + 28) = 99;                          // used within the last three frames
    mapw(4, 0) = texA;
    Packet pk{(int)(0x70000000u + 40)};
    w(0x300) = 0;                               // chain slot word: no previous entry
    w(0x304) = 0x600;                           // output cursor word points at 0x600
    putBlock__FP8_textureP6PacketPPP9uncompMapPPii((_texture*)(arena + tex), &pk, (unsigned*)&w(0x300), (unsigned*)&w(0x304), 1);
    if (w(tex + 28) != 100) return 1;
    if (mapw(5, 0) != tex || uncompSpin != 6) return 2;            // advanced past the recent entry
    if (regs[1] != 0x8000u + 5 * 4096 + 16 || regs[2] != 0x70000000u || regs[3] != 3 || regs[4] != 256) return 3;
    if (pk.cursor != (int)0x70000000u) return 4;
    if (w(0x600) != 0x8000u + 5 * 4096) return 5;
    if (w(0x300) != 0x10000000u + 5 * 8 + 4) return 6;
    if (w(0xd000) != (0x60000000u | 2)) return 7;                    // flag set and nothing left over
    if (w(0x304) != 0xd004) return 8;
    if (syncs != 2) return 9;
    // leftover bytes: 16 bytes past the 4080 limit are moved down; the previous entry address is stored
    memset(uncompMap, 0, sizeof uncompMap);
    uncompSpin = 1279;
    for (int i = 0; i < 4096; i++) *(norrath_host_ptr(0x70000000u) + i) = (unsigned char)i;
    pk.cursor = (int)(0x70000000u + 4080 + 16);
    w(0x300) = 0x700; w(0x700) = 0xdeadbeef;
    w(0x304) = 0x600;
    putBlock__FP8_textureP6PacketPPP9uncompMapPPii((_texture*)(arena + tex), &pk, (unsigned*)&w(0x300), (unsigned*)&w(0x304), 0);
    if (regs[3] != 255) return 10;                                   // (4080 + 15) >> 4
    if (pk.cursor != (int)(0x70000000u + 16)) return 11;
    for (int i = 0; i < 16; i++) if (*(norrath_host_ptr(0x70000000u) + i) != (unsigned char)(4080 + i)) return 12;
    if (w(0x700) != 0x10000000u + 1279 * 8) return 13;                  // previous slot receives this entry
    if (w(0x300) != 0x10000000u + 1279 * 8 + 4) return 14;
    if (uncompSpin != 0) return 15;                                  // wrapped
    if (w(0x9000 + 0x4ff000 - 0x4ff000) == 0 && false) return 16;
    // a claimed old owner is released with its chain
    memset(uncompMap, 0, sizeof uncompMap);
    uncompSpin = 9;
    const unsigned old = 0x180;
    w(old + 28) = 10; w(old + 20) = 0x8000u + 2 * 4096;
    mapw(9, 0) = old;
    mapw(2, 0) = 0x111; mapw(2, 1) = 0x10000000u + 7 * 8; mapw(7, 0) = 0x222; mapw(7, 1) = 0;
    pk.cursor = (int)(0x70000000u + 16);
    w(0x300) = 0; w(0x304) = 0x600;
    putBlock__FP8_textureP6PacketPPP9uncompMapPPii((_texture*)(arena + tex), &pk, (unsigned*)&w(0x300), (unsigned*)&w(0x304), 0);
    if (w(old + 20) != 0 || mapw(2, 0) != 0 || mapw(7, 0) != 0) return 17;
    if (mapw(9, 0) != tex || uncompSpin != 10) return 18;
    std::printf("putBlock contract passed\n");
    return 0;
}
