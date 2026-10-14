// Host check only; not a build input.
#include "fillrectangle.cpp"
#include <cstdio>
#include <string>

static std::string trace;
static unsigned raster_seen[8];
static int nraster;
extern "C" {
void SetSolidFillUIMaterial__8VIRaster(void* r) { trace += "mat;"; raster_seen[nraster++] = static_cast<unsigned>(reinterpret_cast<unsigned long>(r)); }
void BeginTriFan__8VIRaster(void*) { trace += "begin;"; }
void Color__8VIRasterRC10VIColor32F(void*, const void* c) { trace += c == reinterpret_cast<const void*>(0x1) ? "col;" : "col?;"; }
void Vertex__8VIRasterRC8VIVect2I(void*, const int* p) { trace += "v" + std::to_string(p[0]) + "," + std::to_string(p[1]) + ";"; }
void EndTriFan__8VIRaster(void*) { trace += "end;"; }
}

int main() {
    // The raster is held in a 32-bit word; the mocks only compare the number, so a small value is used.
    unsigned self[32] = {};
    self[18] = 0x1234;
    const int rect[4] = {10, 20, 30, 50};
    FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(self, rect, reinterpret_cast<const void*>(0x1));
    const bool ok = trace == "mat;begin;col;v10,20;v30,20;v30,50;v10,50;end;" && nraster == 1 && raster_seen[0] == 0x1234;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
