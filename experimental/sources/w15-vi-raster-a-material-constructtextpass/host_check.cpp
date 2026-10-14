// Host check only; not a build input.
#include <cstdio>
#include <cstring>
static unsigned long long zreg; static unsigned char* z_arg; static int z_calls;
extern "C" unsigned long long GetZBufferReg__C8VIRaster(unsigned char* r) { ++z_calls; z_arg = r; return zreg; }
#include "construct_text_pass.cpp"
static unsigned long long Q(const unsigned char* out, int qw, int half) { unsigned long long v; std::memcpy(&v, out + 16 * qw + 8 * half, 8); return v; }
int main() {
    bool ok = true;
    unsigned char material[512];
    unsigned char out[320];
    static unsigned char raster[17600];
    for (int kind = 0; kind < 6; ++kind) for (int flags = 0; flags < 4; ++flags) for (int f4 = 0; f4 < 8; ++f4) for (int rflag = 0; rflag < 2; ++rflag) {
        std::memset(material, 0, sizeof material);
        std::memset(out, 0xaa, sizeof out);
        *reinterpret_cast<int*>(raster + 17548) = rflag;
        *reinterpret_cast<unsigned*>(material + 12) = static_cast<unsigned>(flags);
        unsigned char* rec = material + 144;
        *reinterpret_cast<unsigned*>(rec + 60) = static_cast<unsigned>(kind);
        *reinterpret_cast<unsigned*>(rec + 52) = static_cast<unsigned>(f4);
        for (int i = 0; i < 64; ++i) rec[128 + i] = static_cast<unsigned char>(i * 3 + 1);
        unsigned char* c = rec + 56;
        c[0] = 255; c[1] = 0; c[2] = 51; c[3] = 255;
        float* tm = reinterpret_cast<float*>(rec + 64);
        for (int i = 0; i < 13; ++i) tm[i] = 0.25f * static_cast<float>(i + 1);
        zreg = 0x123456789abcdef0ull; z_calls = 0;
        ok = ok && ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord(material, 1, raster, out) == 0 && z_calls == 1 && z_arg == raster;
        unsigned w0; std::memcpy(&w0, out, 4);
        ok = ok && w0 == ((f4 & 6) ? 3u : 1u) && Q(out, 0, 1) == 0;
        ok = ok && Q(out, 1, 0) == 0x1000000000008008ull && Q(out, 1, 1) == 14;
        const unsigned long long test = ((flags & 1) ? 0x50000ull : 0x30000ull) | (kind != 0 ? 0x3001ull : 0x80bull);
        ok = ok && Q(out, 2, 0) == test && Q(out, 2, 1) == 71;
        const unsigned long long lows[6] = {0x64, 0x48, 0x42, 0x44, 0x89, 0x64};
        ok = ok && Q(out, 3, 0) == (0x8000000000ull | lows[kind]) && Q(out, 3, 1) == 66;
        ok = ok && Q(out, 4, 0) == (((f4 >> 1) & 1) | ((f4 & 4) ? 4 : 0)) && Q(out, 4, 1) == 8;
        ok = ok && std::memcmp(out + 80, rec + 128, 64) == 0;
        const unsigned long long zb = (0x9abcdef0ull & 0x1ff) | (0x9abcdef0ull & 0x0f000000ull) | ((flags & 2) ? 0 : 1ull << 32);
        ok = ok && Q(out, 9, 0) == zb && Q(out, 9, 1) == 78;
        const unsigned long long g = (rflag && kind != 4) ? 1 : 0;
        const unsigned long long b = (g << 5) | (kind != 0 ? 1ull << 6 : 0);
        ok = ok && Q(out, 10, 0) == (((b | 28) << 47) | 0x3000400000008003ull) && Q(out, 10, 1) == 1042;
        ok = ok && Q(out, 11, 0) == (((b | 29) << 47) | 0x3000400000008003ull) && Q(out, 11, 1) == 1042;
        ok = ok && Q(out, 12, 0) == (((b | 10) << 47) | 0x2000400000008003ull) && Q(out, 12, 1) == 65;
        float f[4]; std::memcpy(f, out + 208, 16);
        const float k = __builtin_bit_cast(float, 0x3b808080u);
        ok = ok && f[0] == 255.0f * k * 128.0f && f[1] == 0.0f && f[2] == 51.0f * k * 128.0f && f[3] == (255.0f * k * 128.0f) * (kind != 0 ? 0.5f : 1.0f);
        float m[12]; std::memcpy(m, out + 224, 48);
        ok = ok && m[0] == tm[0] && m[1] == tm[1] && m[2] == tm[2] && m[4] == tm[3] && m[5] == tm[4] && m[6] == tm[5];
        ok = ok && m[8] == tm[6] + tm[12] && m[9] == tm[7] + tm[13] && m[10] == tm[8];
        unsigned char* guard = out + 224;
        ok = ok && guard[12] == 0xaa && guard[28] == 0xaa && guard[44] == 0xaa;      // fourth words of 14-16 untouched
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
