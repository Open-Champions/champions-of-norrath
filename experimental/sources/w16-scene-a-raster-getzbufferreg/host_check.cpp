// Host check only; not a build input.
#include "get_zbuffer_reg.cpp"
#include <cstdio>
unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");
int main() {
    unsigned long long v = 0x1122334455667788ull, o = 0xdeadbeefcafef00dull;
    __builtin_memcpy(norrath_raster_dbuffer + 480, &v, 8);
    __builtin_memcpy(norrath_raster_dbuffer + 472, &o, 8);
    __builtin_memcpy(norrath_raster_dbuffer + 488, &o, 8);
    bool ok = GetZBufferReg__C8VIRaster(nullptr) == v;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
