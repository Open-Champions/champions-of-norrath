// Host contract for modelDrawAttachment. Not an EE plan input. Callees are recording stubs.
#include <cstdio>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* last; };
extern "C" {
unsigned char* glistp; int nmHalos; const void* halos;
}
static int log_[8], n; static const unsigned char *seenHdr, *seenHdr2; static int seenArg, seenValue; static void* seenTex; static void* seenRec; static unsigned char* seenList;
static unsigned long long cntOrRef = 0x10000000ull;
extern "C" {
void modelSetupAttachment__FP6PacketP12_modelHeaderT1i(Packet* p, const unsigned char* h, const unsigned char* h2, int a) {
    log_[n++] = 1; seenHdr = h; seenHdr2 = h2; seenArg = a;
    p->last = p->cursor; std::memcpy(p->cursor, &cntOrRef, 8); std::memset(p->cursor + 8, 0, 8); p->cursor += 16;
}
void modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(Packet*, const unsigned char*, signed char, const void*, int) { log_[n++] = 2; }
void drawAddToList__FPviP8_textureP11_drawRecordi(void* l, int v, void* t, void* r, int z) { log_[n++] = 3; seenList = (unsigned char*)l; seenValue = v; seenTex = t; seenRec = r; if (z) log_[n++] = 99; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(const unsigned char*, void*, int, const unsigned char*, void*, unsigned long long, int, int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char model[40 + 3 * 16] = {};
    model[18] = 3;   // segments: 4 addresses per level, stride 16
    model[38] = 2;   // two levels
    unsigned level0[4] = {0x1000, 0x1100, 0x1200, 0x1300}, level1[4] = {0x2000, 0x2100, 0x2200, 0x2300};
    std::memcpy(model + 40, level0, 16); std::memcpy(model + 56, level1, 16);
    alignas(16) unsigned char list[512] = {};
    glistp = list; nmHalos = 0;
    int tex = 0, rec = 0; unsigned char other[4];
    // segments 0 and 1 set, 2 clear: one REF from 0x2000 to level1[2]=0x2200 at the clear bit 2.
    n = 0;
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(model, &tex, 7, other, &rec, 3, 5, 99);
    expect(n == 2 && log_[0] == 1 && log_[1] == 3, "setup then draw list");
    expect(seenHdr == model && seenHdr2 == other && seenArg == 5, "setup arguments");
    expect(seenList == list && seenValue == 7 && seenTex == &tex && seenRec == &rec, "draw list arguments");
    unsigned long long t0, t1;
    std::memcpy(&t0, list + 16, 8);
    expect(t0 == ((0x2000ull << 32) | (0x200u >> 4) | 0x30000000ull), "REF tag, level clamped to last, count in quads");
    expect(glistp == list + 48, "cursor advanced by the setup tag, REF tag and close tag");
    std::memcpy(&t1, list + 32, 8);
    expect(t1 == 0x60000000ull, "close tag appended after a REF last tag");
    // no REF at all: the CNT tag from the setup stub is converted in place.
    glistp = list; std::memset(list, 0, sizeof list);
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(model, &tex, 7, other, &rec, 0, 5, 0);
    std::memcpy(&t0, list, 8);
    expect(glistp == list + 16, "cursor advanced only by the setup tag");
    expect(t0 == 0x60000000ull, "CNT tag converted to id 6 in place when no REF was made");
    // null header: nothing happens.
    n = 0; unsigned char* before = glistp;
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(nullptr, &tex, 7, other, &rec, 3, 5, 0);
    expect(n == 0 && glistp == before, "null header does nothing");
    // halo call
    nmHalos = 2; model[16] = 0x20; glistp = list; n = 0;
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(model, &tex, 7, other, &rec, 0, 5, 0);
    expect(n == 3 && log_[1] == 2, "halo drawn when bit 0x20 and nmHalos > 0");
    if (failures) return 1;
    std::printf("modelDrawAttachment contract passed\n");
    return 0;
}
