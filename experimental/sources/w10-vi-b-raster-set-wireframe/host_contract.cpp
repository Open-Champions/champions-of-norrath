#include "set_wireframe.cpp"

#include <cstring>

static VIRaster* seen;
static int stored_at_call;
static int calls;

extern "C" void SetOverrideMaterial__8VIRaster(VIRaster* raster) {
    seen = raster;
    std::memcpy(&stored_at_call, reinterpret_cast<char*>(raster) + 17808, 4);
    ++calls;
}

int main() {
    alignas(4) static unsigned char object[17900];
    std::memset(object, 0x5a, sizeof object);
    VIRaster* raster = reinterpret_cast<VIRaster*>(object);
    if (SetWireframe__8VIRasteri(raster, 1) != 0) return 1;
    if (calls != 1 || seen != raster || stored_at_call != 1) return 2;
    if (SetWireframe__8VIRasteri(raster, 0) != 0) return 3;
    if (calls != 2 || stored_at_call != 0) return 4;
    for (int i = 0; i < 17900; ++i) {
        if ((i < 17808 || i >= 17812) && object[i] != 0x5a) return 5;
    }
    return 0;
}
