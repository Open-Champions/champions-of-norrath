#define NORRATH_HOST_CONTRACT
#include "raster_begin_scene.cpp"

#include <cstring>
#include <string>

extern "C" {
unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");
unsigned norrath_surf_cache[16] __asm__("SurfCache");
}

alignas(4) static unsigned char object[20000];
static std::string trace;

static int& word(int offset) { return *reinterpret_cast<int*>(object + offset); }

extern "C" void norrath_host_store64(unsigned address, unsigned long long value) {
    trace += "P" + std::to_string(address) + "=" + std::to_string(value) + ";";
}
extern "C" int sceGsResetGraph(int a, int b, int c, int d) {
    trace += "G" + std::to_string(a) + std::to_string(b) + std::to_string(c) + std::to_string(d) + ";";
    return 0;
}
extern "C" int sceGsSyncV(int mode) {
    trace += "V" + std::to_string(mode) + ";";
    return 0;
}
extern "C" void sceGsSwapDBuffDc(sceGsDBuffDc* db, int id) {
    trace += std::string("W") + (reinterpret_cast<unsigned char*>(db) == norrath_raster_dbuffer ? "d" : "?") + std::to_string(id) + ";";
}
extern "C" void SetGSScissoring__8VIRaster(VIRaster* r) {
    trace += std::string("C") + (r == reinterpret_cast<VIRaster*>(object) ? "r" : "?") + ";";
    // The cleared word is already zero when scissoring runs.
    if (norrath_surf_cache[7] != 0) trace += "BAD;";
}

int main() {
    std::memset(object, 0x44, sizeof object);
    for (int i = 0; i < 16; ++i) norrath_surf_cache[i] = 0x9999u;
    object[19536] = 0;
    word(17880) = 10;
    if (BeginScene__8VIRaster(reinterpret_cast<VIRaster*>(object)) != 0) return 1;
    if (trace != "Cr;") return 2;
    if (norrath_surf_cache[7] != 0 || norrath_surf_cache[6] != 0x9999u || norrath_surf_cache[8] != 0x9999u) return 3;
    if (word(17880) != 10 || object[19536] != 0) return 4;

    trace.clear();
    for (int i = 0; i < 16; ++i) norrath_surf_cache[i] = 0x9999u;
    object[19536] = 1;
    if (BeginScene__8VIRaster(reinterpret_cast<VIRaster*>(object)) != 0) return 5;
    std::string want = "G0120;";
    for (int i = 0; i < 3; ++i)
        want += "V0;Wd" + std::to_string(10 + i) + ";P301989888=32544;";
    want += "Cr;";
    if (trace != want) return 6;
    if (word(17880) != 13 || object[19536] != 0 || norrath_surf_cache[7] != 0) return 7;
    return 0;
}
