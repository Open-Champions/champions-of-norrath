#define NORRATH_HOST_CONTRACT 1
#include <cstring>
static float scratch[256];
extern "C" volatile float* norrath_host_scratchpad(unsigned address) { return &scratch[(address - 0x70000000u) / 4]; }
#include "set_fog.cpp"

static int called;
static const void* called_with;
extern "C" void SetFogRegisters__8VIRaster(void* r) { ++called; called_with = r; }

int main() {
    alignas(16) static unsigned char raster[19000];
    std::memset(raster, 0x44, sizeof raster);
    unsigned color[4] = {0x11111111u, 0x22222222u, 0x33333333u, 0x44444444u};
    scratch[0x1f0 / 4] = 123.0f;
    int r = SetFog__8VIRasteriffRC10VIColor32F(raster, 5, 10.0f, 110.0f, color);
    if (r != 0 || called != 1 || called_with != raster) return 1;
    int mode; float n, f; unsigned copy[4]; int flag;
    std::memcpy(&mode, raster + 17548, 4); std::memcpy(&n, raster + 17552, 4); std::memcpy(&f, raster + 17556, 4);
    std::memcpy(copy, raster + 17560, 16); std::memcpy(&flag, raster + 17888, 4);
    if (mode != 5 || n != 10.0f || f != 110.0f || flag != 1) return 2;
    if (std::memcmp(copy, color, 16) != 0) return 3;
    // bytes around the copy are untouched
    if (raster[17576] != 0x44 || raster[17892] != 0x44 || raster[17547] != 0x44) return 4;
    if (scratch[0x1f8 / 4] != 0.0f || scratch[0x1fc / 4] != 255.0f) return 5;
    if (scratch[0x1f0 / 4] != (0.0f - 255.0f) / (110.0f - 10.0f)) return 6;
    if (scratch[0x1f4 / 4] != (255.0f * 110.0f - 0.0f * 10.0f) / (110.0f - 10.0f)) return 7;
    // a different range
    SetFog__8VIRasteriffRC10VIColor32F(raster, -2, 0.0f, 64.0f, color);
    if (scratch[0x1f0 / 4] != -255.0f / 64.0f || scratch[0x1f4 / 4] != 255.0f) return 8;
    return 0;
}
