// Host contract for modelSetupCharacter. Not an EE plan input.
#include <cstdint>
#include <cstdio>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* start; unsigned char* gifTag; };
struct Point3 { float x, y, z; };
struct Matrix44 { float m[16]; };
struct _modelHeader;
volatile signed char modelClippingOn;
static Packet* seenPacket; static Point3 seenPos; static int lightCalls; static unsigned char* cursorAtCall;
extern "C" void lightSetupForCharacter__FP6PacketG6Point3(Packet* p, Point3 pos) {
    seenPacket = p; seenPos = pos; lightCalls++; cursorAtCall = p->cursor;
    p->cursor += 5;                                    // leaves the cursor unaligned
}
extern "C" void modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(Packet*, Matrix44*, Point3*, _modelHeader*, char);
alignas(16) static unsigned char buf[512];
alignas(16) static unsigned char model[64];
static uint64_t q(int off) { uint64_t v; memcpy(&v, buf + off, 8); return v; }
static uint32_t w(int off) { uint32_t v; memcpy(&v, buf + off, 4); return v; }
int main() {
    Matrix44 m; for (int i = 0; i < 16; i++) m.m[i] = 1.0f + i;
    Point3 pos{7, 8, 9};
    memset(model, 0, sizeof model);
    uint16_t flags = 0x10; memcpy(model + 16, &flags, 2);
    uint64_t alt = 0x1122334455667788ull; memcpy(model + 24, &alt, 8);
    model[32] = 0x21;
    memset(buf, 0xcc, sizeof buf);
    modelClippingOn = 3;
    Packet p{buf, 0, 0};
    modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(&p, &m, &pos, (_modelHeader*)model, 40);
    if (w(8) != 0x11000000u) return 1;
    if (w(12) != 0x50000006u) return 2;
    if (q(16) != (0x1000000000008000ull | 5) || q(24) != 14) return 3;
    if (q(32) != 0 || q(40) != 63 || q(48) != 0 || q(56) != 6) return 4;
    if (q(64) != ((88ull << 32) | 100) || q(72) != 66) return 5;
    if (q(80) != ((0x21ull << 32) | 100) || q(88) != 67) return 6;
    if (q(96) != 0x507f5 || q(104) != 71) return 7;
    if (w(112) != 0x6c04c000u) return 8;
    for (int i = 0; i < 16; i++) { float f; memcpy(&f, buf + 116 + 4 * i, 4); if (f != 1.0f + i) return 9; }
    if (w(180) != (0x04000000u | 3) || w(184) != 0x14000002u) return 10;
    if (lightCalls != 1 || seenPacket != &p || seenPos.x != 7 || seenPos.z != 9 || cursorAtCall != buf + 188) return 11;
    // cursor ended at 193, padded to 208: (208 / 8 - 2) / 2 = 12
    if (p.cursor != buf + 208 || p.start != buf || p.gifTag != buf + 16) return 12;
    if (q(0) != 0x1000000cull) return 13;
    if (buf[193] != 0 || buf[207] != 0 || buf[208] != 0xcc) return 14;
    // zero fade uses the model word; no bit 0x10 omits register 67
    memset(buf, 0xcc, sizeof buf); flags = 0; memcpy(model + 16, &flags, 2);
    p = Packet{buf, 0, 0};
    modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(&p, &m, &pos, (_modelHeader*)model, 0);
    if (w(12) != 0x50000005u || q(16) != (0x1000000000008000ull | 4)) return 15;
    if (q(64) != alt || q(72) != 66 || q(80) != 0x507f5 || q(88) != 71) return 16;
    std::printf("modelSetupCharacter contract passed\n");
    return 0;
}
