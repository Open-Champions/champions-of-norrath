// Host contract for modelDrawHalo. Not an EE plan input.
#include <cstdint>
#include <cstdio>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* start; unsigned char* gifTag; unsigned char* vif; };
struct _modelHeader; struct HaloDef;
extern "C" void modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(Packet*, _modelHeader*, char, HaloDef*, int);
alignas(16) static unsigned char buf[2048];
alignas(16) static unsigned char model[128];
static uint64_t q(int off) { uint64_t v; memcpy(&v, buf + off, 8); return v; }
static uint32_t w(int off) { uint32_t v; memcpy(&v, buf + off, 4); return v; }
int main() {
    uint16_t flags = 0;
    memset(model, 0, sizeof model);
    memcpy(model + 16, &flags, 2);
    memset(buf, 0xcc, sizeof buf);
    Packet p{buf, 0, 0, 0};
    modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(&p, (_modelHeader*)model, 1, 0, 0);
    if (p.cursor != buf || buf[0] != 0xcc) return 1;                         // bit 0x20 clear: nothing
    flags = 0x20; memcpy(model + 16, &flags, 2);
    model[18] = 2; model[38] = 1;                                           // two segments, one level
    uint32_t table[3] = {0x1000, 0x1040, 0x1100};                           // 16-byte aligned addresses
    memcpy(model + 40, table, 12);
    uint64_t w24 = 0x0102030405060708ull; memcpy(model + 24, &w24, 8);
    unsigned char halos[8] = {1, 2, 3, 4, 0, 0, 0x80, 0x3f};                // colour bytes then 1.0f
    modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(&p, (_modelHeader*)model, 1, (HaloDef*)halos, 1);
    // state packet: 0..96. VIF word at 12, GIF tag at 16, entries at 32, 48, 64 and 80.
    if (q(0) != 0x10000005ull) return 2;                                    // (96 / 8 - 2) / 2
    if (w(8) != 0 && w(8) != 0xccccccccu) return 20;
    if (w(12) != (0x50000000u | 5)) return 3;                               // (96 - 12 - 4) / 16
    if (q(16) != (0x1000000000008000ull | 4) || q(24) != 14) return 4;
    if (q(32) != 0 || q(40) != 26 || q(48) != 72 || q(56) != 27 || q(64) != 72 || q(72) != 66) return 5;
    if (q(80) != 0x71001 || q(88) != 71) return 6;
    // halo chunk at 96
    if (q(96) != 0x10000002ull) return 7;                                   // 48 bytes
    if (w(104) != 0x13000000u || w(108) != 0x6002c000u || w(112) != 1u) return 8;
    float f; memcpy(&f, buf + 116, 4); if (f != 1.0f) return 9;
    if (w(120) != 0x6e01c002u || buf[124] != 1 || buf[127] != 4 || w(128) != 0x1400002au) return 10;
    if (buf[132] != 0 || buf[143] != 0) return 11;
    // one run (table[0]..table[2]) emitted at the clear bit as a REF tag at 144
    if (q(144) != (0x0000100030000000ull | 16) || q(152) != 0) return 12;
    // closing packet at 160
    if (w(168) != 0x13000000u || w(172) != 0x6001c000u || w(176) != 0 || w(180) != 0x1400002au) return 13;
    if (buf[184] != 0 || buf[187] != 0) return 14;
    if (w(188) != (0x50000000u | 4)) return 15;
    if (q(192) != (0x1000000000008000ull | 3) || q(200) != 14) return 16;
    if (q(208) != 1 || q(216) != 26 || q(224) != w24 || q(232) != 66 || q(240) != 0x507f5 || q(248) != 71) return 17;
    if (q(160) != 0x10000005ull) return 18;
    if (p.cursor != buf + 256 || p.start != buf + 160 || p.vif != buf + 188 || p.gifTag != buf + 192) return 19;
    // zero flag selects 68 for the third state entry
    memset(buf, 0xcc, sizeof buf); p = Packet{buf, 0, 0, 0};
    modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(&p, (_modelHeader*)model, 0, (HaloDef*)halos, 0);
    if (q(64) != 68 || q(72) != 66) return 21;
    // no halos: the closing packet follows the state packet directly at 96
    if (q(96) != 0x10000005ull || p.cursor != buf + 192) return 22;
    std::printf("modelDrawHalo contract passed\n");
    return 0;
}
