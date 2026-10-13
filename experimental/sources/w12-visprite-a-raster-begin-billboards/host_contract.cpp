#define NORRATH_HOST_CONTRACT 1
#include <cstring>
alignas(16) static unsigned char spad[0x400];
extern "C" unsigned char* norrath_host_scratchpad(unsigned address) { return spad + (address - 0x70000000u); }
#include "begin_billboards.cpp"
#include <string>
#include <vector>

alignas(16) static unsigned char raster[20000];
alignas(16) static unsigned char packet[4096];
alignas(16) static unsigned char material[128];
static std::vector<std::string> ev;
static void tagev(const char* n, long v = 0) { ev.push_back(std::string(n) + ":" + std::to_string(v)); }
extern "C" void* Material__8VIRasteri(void*, int h) { tagev("material", h); return material; }
extern "C" void UploadBillboardMicro__8VIRasterPP10_sceDmaTag(void*, void* slot) { tagev("micro", static_cast<unsigned char*>(slot) == raster + 19492); }
extern "C" void CalcMatrices__8VIRaster(void*) { tagev("calc"); }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* res, const void* a, const void* b) {
    tagev("mul", (static_cast<const unsigned char*>(a) == spad + 0x40) * 2 + (static_cast<const unsigned char*>(b) == spad + 0xc0));
    float out[16];
    for (int i = 0; i < 16; ++i) out[i] = 100.0f + i;
    std::memcpy(res, out, 64);
}
extern "C" void FlushDMABufferIfFull__8VIRaster(void*) {
    unsigned char* p; std::memcpy(&p, raster + 19492, sizeof p);
    tagev("flush", p - packet);
}
extern "C" void _$_10VIMatrix44(void*, int f) { tagev("dtor", f); }

static unsigned w(const unsigned char* b, int off) { unsigned v; std::memcpy(&v, b + off, 4); return v; }

int main() {
    std::memset(raster, 0, sizeof raster);
    std::memset(packet, 0xee, sizeof packet);
    for (int i = 0; i < 0x400; ++i) spad[i] = static_cast<unsigned char>(i * 7 + 3);
    unsigned char* p = packet + 32;
    std::memcpy(raster + 19492, &p, sizeof p);
    int h = 5; std::memcpy(raster + 17824, &h, 4);
    int two = 2; std::memcpy(raster + 19532, &two, 4);
    float f1 = 3.5f, f2 = 4.5f; std::memcpy(raster + 17780, &f1, 4); std::memcpy(raster + 17784, &f2, 4);
    void* out = nullptr;
    // count <= 0
    if (BeginBillboards__8VIRasterPP11VIBillboardi(raster, &out, 0) != 0 || out != nullptr) return 1;
    if (ev.size() != 1 || ev[0] != "material:5") return 2;
    // clean matrices: only the header and count quadwords, 7 billboards
    ev.clear();
    if (BeginBillboards__8VIRasterPP11VIBillboardi(raster, &out, 7) != 7) return 3;
    if (w(raster, 17860) != 7) return 4;
    if (w(p, 16) != 0 || w(p, 24) != 0x13000000u || w(p, 28) != ((22u << 16) | 0x6c008000u)) return 5;
    if (w(p, 32) != 7 || w(p, 36) != 0 || w(p, 40) != 0 || w(p, 44) != 0) return 6;
    if (out != p + 48) return 7;
    unsigned char* cur; std::memcpy(&cur, raster + 19492, sizeof cur);
    if (cur != p) return 8;
    // the count is limited to 80, the micro upload happens when the word is not 2
    ev.clear(); int three = 3; std::memcpy(raster + 19532, &three, 4);
    if (BeginBillboards__8VIRasterPP11VIBillboardi(raster, &out, 500) != 80) return 9;
    if (ev.size() != 2 || ev[1] != "micro:1") return 10;
    if (w(p, 28) != ((241u << 16) | 0x6c008000u) || w(p, 32) != 80) return 11;
    // dirty matrices: full block
    ev.clear(); std::memcpy(raster + 19532, &two, 4);
    int dirty = 1; std::memcpy(raster + 17892, &dirty, 4);
    std::memset(material, 0, sizeof material);
    if (BeginBillboards__8VIRasterPP11VIBillboardi(raster, &out, 3) != 3) return 12;
    std::vector<std::string> want = {"material:5", "calc:0", "mul:3", "flush:", "dtor:2"};
    if (ev.size() != 5) return 13;
    for (int i : {0, 1, 2, 4}) if (ev[i] != want[i]) return 14;
    // layout: P + 16 .. : 1 + 12 + 1 + 5 + 28 + 1 + 1 + 1 + 1 qwords
    const int quadwords = 1 + 12 + 1 + 5 + 28 + 1 + 1 + 1 + 1;
    if ((w(p, 0) & 0xffff) != (unsigned)quadwords) return 15;
    if (p[3] != 16 || w(p, 4) != 0) return 16;
    if (w(p, 16 + 8) != 0x13000000u || w(p, 16 + 12) != 0x6c0c0000u) return 17;
    if (std::memcmp(p + 32, spad, 64) != 0) return 18;
    float m0; std::memcpy(&m0, p + 32 + 64, 4);
    if (m0 != 100.0f) return 19;
    if (std::memcmp(p + 32 + 128, spad + 0x80, 64) != 0) return 20;
    unsigned char* t = p + 32 + 192;
    if (w(t, 12) != 0x6c040010u) return 21;
    unsigned long long d; std::memcpy(&d, t + 16, 8);
    if (d != 0xb000000000008000ull) return 22;
    std::memcpy(&d, t + 24, 8); if (d != 0x0000044242424421ull) return 23;
    std::memcpy(&d, t + 32, 8); if (d != 0x202a400000008001ull) return 24;
    std::memcpy(&d, t + 40, 8); if (d != 66ull) return 25;
    if (w(t, 56) != 0x3f800000u || w(t, 64) != 0x2000 || w(t, 68) != 0x2000 || w(t, 72) != 0x2000) return 26;
    if (w(t, 92) != 0x6c1c0020u) return 27;
    std::memcpy(&d, t + 96, 8); if (d != (0xfe00ull << 46)) return 28;
    std::memcpy(&d, t + 96 + 8, 8); if (d != 0x3f800000ull) return 29;
    std::memcpy(&d, t + 96 + 16 * 27, 8); if (d != 0x3f800000ull) return 30;
    unsigned char* u = t + 96 + 16 * 28;
    if (w(u, 12) != 0x6c02000cu) return 31;
    for (int i = 0; i < 4; ++i) if (w(u, 16 + 4 * i) != 0x3f800000u) return 32;
    if (w(u, 32) != w(raster, 17780) || w(u, 36) != w(raster, 17784) || w(u, 40) != 0x43000000u || w(u, 44) != 0x43000000u) return 33;
    if (w(u, 60) != 0x14000000u) return 34;
    if (ev[3] != "flush:" + std::to_string(32 + 16 * quadwords + 16 - 0)) {
        // the flush sees the pointer already advanced past the last quadword
        return 35;
    }
    // the material flag selects the last float
    ev.clear(); std::memcpy(raster + 17892, &dirty, 4);
    std::memcpy(raster + 19492, &p, sizeof p);
    int flag = 1; std::memcpy(material + 60, &flag, 4);
    BeginBillboards__8VIRasterPP11VIBillboardi(raster, &out, 3);
    if (w(u, 44) != 0x42800000u) return 36;
    return 0;
}
