// Host check only; not a build input.
#include "set.cpp"
#include <cstdio>
#include <cstring>
static int log2_calls, log_args[4];
extern "C" int Log2__13VIEESurfCachei(unsigned char*, int v) { log_args[log2_calls++] = v; int n = 0; while ((1 << n) < v) ++n; return n; }
static unsigned long long field(unsigned long long v, int lo, int bits) { return (v >> lo) & ((1ull << bits) - 1); }
int main() {
    bool ok = true;
    int s[20], d[32];
    unsigned char pass[160];
    std::memset(s, 0, sizeof s); std::memset(d, 0, sizeof d); std::memset(pass, 0, sizeof pass);
    s[2] = 256; s[3] = 128; s[10] = 0;
    d[5] = 0; d[7] = 3;                       // format 0, three levels
    d[8] = 0x1000; d[9] = 0x2000; d[10] = 0x3000; d[11] = 0x4000; d[12] = 0x5000; d[13] = 0x6000; d[14] = 0x7000;
    *reinterpret_cast<int*>(pass + 4) = 0;      // flags clear: bit 0 clear -> TFX bit, bit 3 clear -> 352
    *reinterpret_cast<float*>(pass + 52) = -0.5f;   // * -127 = 63.5 -> 63
    log2_calls = 0;
    ok = ok && Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(nullptr, reinterpret_cast<unsigned char*>(s), reinterpret_cast<unsigned char*>(d), pass) == 0;
    ok = ok && log2_calls == 2 && log_args[0] == 256 && log_args[1] == 128;
    long long* q = reinterpret_cast<long long*>(pass + 80);
    unsigned long long tex0 = q[0], tex1 = q[2], m1 = q[4], m2 = q[6];
    ok = ok && q[1] == 6 && q[3] == 20 && q[5] == 52 && q[7] == 54;
    ok = ok && field(tex0, 0, 14) == 0x10 && field(tex0, 14, 6) == 4 && field(tex0, 20, 6) == 0 && field(tex0, 26, 4) == 7 && field(tex0, 30, 4) == 6
        && field(tex0, 34, 1) == 1 && field(tex0, 35, 2) == 1 && field(tex0, 37, 14) == 0 && field(tex0, 61, 3) == 0;
    ok = ok && field(tex1, 2, 3) == 2 && field(tex1, 5, 4) == 0b1011 && field(tex1, 32, 12) == 63;
    ok = ok && field(m1, 0, 14) == 0x20 && field(m1, 14, 6) == 2 && field(m1, 20, 14) == 0x30 && field(m1, 34, 6) == 1 && field(m1, 40, 14) == 0x40 && field(m1, 54, 6) == 1;
    ok = ok && field(m2, 0, 14) == 0x20 && field(m2, 14, 6) == 1 && field(m2, 20, 14) == 0x30 && field(m2, 34, 6) == 1 && field(m2, 40, 14) == 0x40 && field(m2, 54, 6) == 1;
    // format 20 with a palette and flags 9
    d[5] = 20; d[1] = 2; d[3] = 0x4400; s[10] = 5; s[2] = 64;
    *reinterpret_cast<int*>(pass + 4) = 9;
    *reinterpret_cast<float*>(pass + 52) = 0.0f;
    ok = ok && Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(nullptr, reinterpret_cast<unsigned char*>(s), reinterpret_cast<unsigned char*>(d), pass) == 0;
    tex0 = q[0]; tex1 = q[2]; m1 = q[4]; m2 = q[6];
    ok = ok && field(tex0, 14, 6) == 2 && field(tex0, 20, 6) == 20 && field(tex0, 35, 1) == 0 && field(tex0, 37, 14) == 0x44 && field(tex0, 51, 4) == 2 && field(tex0, 61, 1) == 1;
    ok = ok && field(tex1, 5, 4) == 0b1010 && field(tex1, 32, 12) == 0;
    ok = ok && field(m1, 14, 6) == 2 && field(m1, 34, 6) == 2 && field(m1, 54, 6) == 2;
    ok = ok && field(m2, 0, 14) == 0x50 && field(m2, 20, 14) == 0x60 && field(m2, 40, 14) == 0x70 && field(m2, 14, 6) == 2;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
