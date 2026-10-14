// Host contract for modelSetupAttachment__FP6PacketP12_modelHeaderT1i. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA (arena base address 0x100000, a multiple of 16).
#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
struct Point3 { float x, y, z; };
extern "C" {
int modelOption;
Point3 attachmentLightPos;
static int lightCalls; static unsigned lightPacket; static const Point3* lightPoint;
void lightSetupForCharacter__FP6PacketG6Point3(void* packet, const Point3* point) {
    ++lightCalls; lightPacket = norrath_host_address(static_cast<unsigned char*>(packet)); lightPoint = point;
    // append one word and leave the cursor unaligned so the final padding is exercised
    unsigned char* p = norrath_host_pointer(hw(static_cast<unsigned char*>(packet)));
    sw_(p, 0x77665544u);
    sw_(static_cast<unsigned char*>(packet), norrath_host_address(p + 4));
}
void modelSetupAttachment__FP6PacketP12_modelHeaderT1i(unsigned char*, const unsigned char*, const unsigned char*, int);
}
static unsigned long long q(const unsigned char* p) { return (unsigned long long)hw(p) | ((unsigned long long)hw(p + 4) << 32); }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char* pk = arena_at(0x10);
    unsigned char* m = arena_at(0x100);
    unsigned char* n = arena_at(0x200);
    unsigned char* out = arena_at(0x1000);        // 16-aligned address 0x101000
    std::memset(m, 0, 64); std::memset(n, 0, 512);
    // Case A: aligned cursor, no flag, no attachment matrix (byte +19 is -1), no options.
    std::memset(out, 0xaa, 512);
    sw_(pk, norrath_host_address(out)); sw_(pk + 4, 0); sw_(pk + 8, 0);
    m[19] = 0xff; m[16] = 0; modelOption = 0;
    sw_(m + 24, 0x11223344u); sw_(m + 28, 0x55667788u);
    modelSetupAttachment__FP6PacketP12_modelHeaderT1i(pk, m, n, 0);
    // head 0x101000: head tag at +0, word 0x11000000 at +8, pad +12.. none (t = head + 12 has low nibble 12)
    expect(hw(out + 8) == 0x11000000u, "A: second head word");
    expect(hw(pk + 4) == norrath_host_address(out), "A: head recorded");
    expect(hw(out + 12) == 0x50000004u, "A: tag word without flag");
    expect(q(out + 16) == (0x1000000000008000ull | 3), "A: gif tag with count");
    expect(q(out + 24) == 14, "A: register count");
    expect(q(out + 32) == 0 && q(out + 40) == 63 && q(out + 48) == 0 && q(out + 56) == 6, "A: constant rows");
    expect(q(out + 64) == 0x5566778811223344ull && q(out + 72) == 66, "A: model doubleword");
    expect(hw(pk) == norrath_host_address(out + 80), "A: cursor after block");
    expect(hw(pk + 8) == norrath_host_address(out + 16), "A: gif tag address recorded");
    expect(q(out) == (0x10000000ull | 4), "A: head tag count");
    expect(out[80] == 0xaa, "A: nothing past the end");
    // Case B: unaligned cursor, flag, matrix, flip, lighting.
    std::memset(out, 0xaa, 512);
    sw_(pk, norrath_host_address(out + 5)); sw_(pk + 4, 0); sw_(pk + 8, 0);
    sh_(m + 16, 0x10); m[19] = 2; m[32] = 0x9; modelOption = 0x80 | 0x40;
    n[34] = 1;
    unsigned char* rec = n + 16 + 52;                  // k = 1 from index
    sw_(rec, 0x0f0f0f0fu);
    for (int i = 0; i < 12; ++i) sf_(rec + 4 + 4 * i, 1.0f + i);
    attachmentLightPos = Point3{1, 2, 3};
    lightCalls = 0;
    modelSetupAttachment__FP6PacketP12_modelHeaderT1i(pk, m, n, 1);
    unsigned char* head = out + 5;
    unsigned char* t = out + 16 + 12;                   // first address >= head + 12 with low nibble 12
    expect(hw(head + 8) == 0x11000000u, "B: second head word");
    for (unsigned char* p = head + 12; p < t; ++p) expect(*p == 0, "B: pad is zero");
    expect(hw(t) == 0x50000005u, "B: tag word with flag");
    expect(q(t + 68) == ((9ull << 32) | 100) && q(t + 76) == 67, "B: flag block");
    expect(hw(pk + 8) == norrath_host_address(t + 4), "B: gif tag address recorded");
    expect(q(t + 4) == (0x1000000000008000ull | ((84 - 4) / 16 - 1)), "B: gif tag count");
    unsigned char* c = t + 84;
    expect(hw(c) == 0x6001c000u && hw(c + 4) == 0x0f0f0f0fu << 2 && hw(c + 8) == 0x6c03c001u, "B: matrix header");
    // rows: floats are 1..12; offsets from r+4 are 0,12,24,36 / 4,16,28,40 / 8,20,32,44 in words 0,3,6,9 / 1,4,7,10 / 2,5,8,11
    const int src[12] = {0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11};
    for (int i = 0; i < 12; ++i) {
        float want = 1.0f + src[i];
        if (i == 0 || i == 4 || i == 8) want = -want;
        expect(lf_(c + 12 + 4 * i) == want, "B: matrix float");
    }
    expect(hw(c + 60) == 0x1400001au, "B: matrix trailer");
    expect(lightCalls == 1 && lightPoint == &attachmentLightPos && lightPacket == norrath_host_address(pk), "B: lighting call");
    expect(hw(c + 64) == 0x77665544u, "B: lighting output");
    // cursor was left at c + 68 by the stub; the final padding rounds it up to a multiple of 16
    unsigned char* end = c + 80;
    expect(hw(pk) == norrath_host_address(end), "B: cursor aligned");
    for (unsigned char* p = c + 68; p < end; ++p) expect(*p == 0, "B: final pad is zero");
    expect(q(head) == (0x10000000ull | (((end - head) / 8 - 2) >> 1)), "B: head tag count");
    // Case C: no flip, attachment index from the header byte, no lighting.
    std::memset(out, 0xaa, 512);
    sw_(pk, norrath_host_address(out)); modelOption = 0; sh_(m + 16, 0); m[19] = 0; lightCalls = 0;
    modelSetupAttachment__FP6PacketP12_modelHeaderT1i(pk, m, n, -1);
    c = out + 80;
    expect(hw(c + 4) == hw(n + 16) << 2, "C: record 0 word");
    expect(lf_(c + 12) == lf_(n + 16 + 4), "C: no flip");
    expect(lightCalls == 0, "C: no lighting");
    if (failures) return 1;
    std::printf("modelSetupAttachment contract passed\n");
    return 0;
}
