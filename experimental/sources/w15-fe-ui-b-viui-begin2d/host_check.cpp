#include <cstdint>
#include <cstring>
#include "begin2d.cpp"

// Host check: the forwarder must pass the word at offset 72 of the object, and nothing else.
static const void* seen;
static int calls;
extern "C" void Begin2D__8VIRaster(VIRaster* raster) { seen = raster; ++calls; }

int main() {
    alignas(16) unsigned char obj[128];
    std::memset(obj, 0x11, sizeof obj);
    void* marker = &calls;
    std::memcpy(obj + 72, &marker, sizeof marker);
    Begin2D__4VIUI(reinterpret_cast<VIUI*>(obj));
    if (calls != 1 || seen != marker) return 1;
    return 0;
}
