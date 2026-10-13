#include "set_clear_color.cpp"

#include <cstring>
#include <string>

unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");

static std::string trace;
static unsigned long long db384_at_flush;
static unsigned long long db752_at_flush;

extern "C" void FlushCache(int mode) {
    trace += "F" + std::to_string(mode) + ";";
    std::memcpy(&db384_at_flush, norrath_raster_dbuffer + 384, 8);
    std::memcpy(&db752_at_flush, norrath_raster_dbuffer + 752, 8);
}

static unsigned long long pack(unsigned r, unsigned g, unsigned b, unsigned a) {
    return 0x3f80000000000000ull | r | (g << 8) | (b << 16) | (static_cast<unsigned long long>(a) << 24);
}

static int check(VIColor32F color, unsigned r, unsigned g, unsigned b, unsigned a) {
    alignas(4) static unsigned char object[20000];
    std::memset(object, 0x11, sizeof object);
    std::memset(norrath_raster_dbuffer, 0x22, sizeof norrath_raster_dbuffer);
    trace.clear();
    if (SetClearColor__8VIRasterRC10VIColor32F(reinterpret_cast<VIRaster*>(object), &color) != 0) return 1;
    if (trace != "F0;") return 2;
    if (std::memcmp(object + 17788, &color, 16) != 0) return 3;
    unsigned long long want = pack(r, g, b, a);
    unsigned long long got;
    std::memcpy(&got, norrath_raster_dbuffer + 752, 8);
    if (got != want || db752_at_flush != want || db384_at_flush != want) return 4;
    std::memcpy(&got, norrath_raster_dbuffer + 384, 8);
    if (got != want) return 5;
    for (int i = 0; i < 0x330; ++i) {
        bool written = (i >= 752 && i < 760) || (i >= 384 && i < 392);
        if (!written && norrath_raster_dbuffer[i] != 0x22) return 6;
    }
    for (int i = 0; i < 20000; ++i) {
        bool written = i >= 17788 && i < 17804;
        if (!written && object[i] != 0x11) return 7;
    }
    return 0;
}

int main() {
    if (int e = check({1.0f, 0.5f, 0.0f, 0.2f}, 255, 128, 0, 51)) return e;
    if (int e = check({0.0f, 0.0f, 0.0f, 0.0f}, 0, 0, 0, 0)) return 10 + e;
    if (int e = check({0.333f, 0.999f, 0.001f, 1.0f}, 85, 255, 0, 255)) return 20 + e;
    return 0;
}
