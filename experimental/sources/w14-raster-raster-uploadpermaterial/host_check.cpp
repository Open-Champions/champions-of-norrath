// Host check only; not a build input. DMA addresses map into a small arena and the scratchpad addresses into a second array.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static unsigned char scratch[0x400];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x70000000u) return scratch + (address - 0x70000000u);
    return arena + (address - arena_base);
}
unsigned colorMatrixValue __asm__("_8VIRaster$ColorMatrix") = 0x15;
unsigned materialOffsetValue __asm__("_8VIRaster$RMaterialOffset") = 0x3c;
#include "upload_per_material.cpp"
#include <cstdio>
#include <cstring>
int main() {
    scratch[0x224] = 255; scratch[0x225] = 0; scratch[0x226] = 51; scratch[0x227] = 128;
    float offsets[4] = {0.5f, -1.0f, 2.0f, 0.0f};
    memcpy(scratch + 0x1e0, offsets, sizeof offsets);
    memset(arena, 0xaa, 128);
    unsigned pointer = arena_base + 16;
    int r = UploadPerMaterial__8VIRasterPP10_sceDmaTag(nullptr, &pointer);
    const unsigned char* p = arena + 16;
    const unsigned* wp = reinterpret_cast<const unsigned*>(p);
    const float scale = __builtin_bit_cast(float, 0x3b808080u);
    float f[4];
    memcpy(f, p + 32, sizeof f);
    bool ok = r == 0 && pointer == arena_base + 80 && p[0] == 3 && p[1] == 0 && p[2] == 0xaa && p[3] == 0x10 && wp[1] == 0 && wp[2] == 0xaaaaaaaau && wp[3] == 0xaaaaaaaau;
    ok = ok && wp[4] == 0 && wp[5] == 0 && wp[6] == 0 && wp[7] == (0x15u + 2) + 0x6c010000u;
    ok = ok && f[0] == 255.0f * scale + 0.5f && f[1] == 0.0f * scale - 1.0f && f[2] == 51.0f * scale + 2.0f && f[3] == 128.0f * scale + 0.0f;
    ok = ok && wp[12] == 0 && wp[13] == 0 && wp[14] == 0 && wp[15] == (0x3cu | 0x14000000u) && p[64] == 0xaa && arena[15] == 0xaa;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
