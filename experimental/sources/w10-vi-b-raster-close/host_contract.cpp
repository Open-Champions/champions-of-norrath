#include "raster_close.cpp"

#include <string>

static std::string trace;
static int clear_result;
static VIRaster* clear_arg;

extern "C" void VIEEDMASync__Fi(int mode) { trace += "S" + std::to_string(mode) + ";"; }
extern "C" int Clear__8VIRaster(VIRaster* raster) {
    trace += "C;";
    clear_arg = raster;
    return clear_result;
}

int main() {
    VIRaster* raster = reinterpret_cast<VIRaster*>(0x5550);
    for (int r : {-5, -1}) {
        trace.clear();
        clear_result = r;
        if (Close__8VIRaster(raster) != -1 || trace != "S1;C;" || clear_arg != raster) return 1;
    }
    for (int r : {0, 1, 77}) {
        trace.clear();
        clear_result = r;
        if (Close__8VIRaster(raster) != 0 || trace != "S1;C;") return 2;
    }
    return 0;
}
