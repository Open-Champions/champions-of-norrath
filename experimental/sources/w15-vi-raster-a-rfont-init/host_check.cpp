// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int seq[8], nseq; static int rets[6];
static unsigned char* sd; static unsigned char* sr; static unsigned char* sdict;
static int s_size, s_depth, s_last, s_extra_seen;
extern "C" {
int CalcMaxWidth__7VIRFontiPUc(unsigned char* s, int size, unsigned char* data) { seq[nseq++] = 1; sd = data; s_size = size; return s == arena ? rets[0] : 99; }
int CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi(unsigned char*, unsigned char* r, unsigned char* dict, int size, int extra, int depth, int last) {
    seq[nseq++] = 2; sr = r; sdict = dict; s_extra_seen = extra; s_depth = depth; s_last = last; return size == s_size ? rets[1] : 99; }
int CalcDataSize__7VIRFontPUc(unsigned char*, unsigned char* data) { seq[nseq++] = 3; return data == sd ? rets[2] : 99; }
int Allocate__7VIRFont(unsigned char*) { seq[nseq++] = 4; return rets[3]; }
int InitSlots__7VIRFontR8VIRaster(unsigned char*, unsigned char* r) { seq[nseq++] = 5; return r == sr ? rets[4] : 99; }
int InitChars__7VIRFontPUc(unsigned char*, unsigned char* data) { seq[nseq++] = 6; return data == sd ? rets[5] : 99; }
}
#include "init.cpp"
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    unsigned char raster[4], dict[4], data[4];
    for (int fail = -1; fail < 6; ++fail) for (int neg : {0, 1}) {
        for (int k = 0; k < 6; ++k) rets[k] = 0;
        if (fail >= 0) rets[fail] = neg ? -5 : 7;
        nseq = 0;
        std::memset(arena, 0, 128);
        W(64) = 0x1000; W(68) = 0x2000; W(72) = 0x3000; W(76) = 0x4000; W(84) = 0x500;
        int r = Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi(arena, raster, dict, 12, data, 34, 5, 6);
        int expect_calls = (fail < 0 || !neg) ? 6 : fail + 1;
        int expect = (fail >= 0 && neg) ? -1 : 0;
        ok = ok && r == expect && nseq == expect_calls && W(20) == 34 && W(40) == 12 && s_extra_seen == 34 && s_depth == 5 && s_last == 6 && sdict == dict;
        for (int k = 0; k < nseq; ++k) ok = ok && seq[k] == k + 1;
        if (nseq >= 5) ok = ok && W(64) == 0x1500 && W(68) == 0x2500 && W(72) == 0x3500 && W(76) == 0x4500;
        else ok = ok && W(64) == 0x1000 && W(68) == 0x2000 && W(72) == 0x3000 && W(76) == 0x4000;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
