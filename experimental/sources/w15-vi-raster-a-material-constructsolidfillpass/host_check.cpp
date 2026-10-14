// Host check only; not a build input.
#include <cstdio>
#include <cstring>
static unsigned long long zreg; static unsigned char* z_arg; static int z_calls;
extern "C" unsigned long long GetZBufferReg__C8VIRaster(unsigned char* r) { ++z_calls; z_arg = r; return zreg; }
#include "construct_solid_fill_pass.cpp"
static unsigned long long Q(const unsigned char* out, int qw, int half) { unsigned long long v; std::memcpy(&v, out + 16 * qw + 8 * half, 8); return v; }
int main() {
    bool ok = true;
    unsigned char material[512];
    unsigned char out[320];
    unsigned char raster[4];
    for (int kind = 0; kind < 7; ++kind) for (int flags = 0; flags < 4; ++flags) {
        std::memset(material, 0, sizeof material);
        std::memset(out, 0xaa, sizeof out);
        *reinterpret_cast<unsigned*>(material + 12) = static_cast<unsigned>(flags);
        *reinterpret_cast<unsigned*>(material + 144 + 60) = static_cast<unsigned>(kind);
        unsigned char* c = material + 144 + 56;
        c[0] = 255; c[1] = 0; c[2] = 51; c[3] = 255;
        zreg = 0x123456789abcdef0ull; z_calls = 0;
        ok = ok && ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord(material, 1, raster, out) == 0 && z_calls == 1 && z_arg == raster;
        ok = ok && Q(out, 0, 0) == 0 && Q(out, 0, 1) == 0;
        ok = ok && Q(out, 1, 0) == 0x1000000000008003ull && Q(out, 1, 1) == 14;
        ok = ok && Q(out, 2, 0) == ((flags & 1) ? 0x50000ull : 0x30000ull) && Q(out, 2, 1) == 71;
        const unsigned long long lows[7] = {0x64, 0x48, 0x42, 0x44, 0x89, 0x64, 0x64};
        ok = ok && Q(out, 3, 0) == (0x8000000000ull | lows[kind]) && Q(out, 3, 1) == 66;
        const unsigned long long zb = (0x9abcdef0ull & 0x1ff) | (0x9abcdef0ull & 0x0f000000ull) | ((flags & 2) ? 0 : 1ull << 32);
        ok = ok && Q(out, 4, 0) == zb && Q(out, 4, 1) == 78;
        const bool tex = kind != 0;
        ok = ok && Q(out, 10, 0) == (tex ? 0x2026400000008003ull : 0x2006400000008003ull) && Q(out, 11, 0) == (tex ? 0x2026c00000008003ull : 0x2006c00000008003ull)
            && Q(out, 12, 0) == (tex ? 0x2025400000008003ull : 0x2005400000008003ull) && Q(out, 10, 1) == 65 && Q(out, 11, 1) == 65 && Q(out, 12, 1) == 65;
        float f[4]; std::memcpy(f, out + 208, 16);
        const float k = __builtin_bit_cast(float, 0x3b808080u);
        ok = ok && f[0] == 255.0f * k * 255.0f && f[1] == 0.0f && f[2] == 51.0f * k * 255.0f && f[3] == (255.0f * k * 255.0f) * (tex ? 0.5f : 1.0f);
        for (int i = 80; i < 160; ++i) ok = ok && out[i] == 0xaa;
        for (int i = 224; i < 320; ++i) ok = ok && out[i] == 0xaa;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
