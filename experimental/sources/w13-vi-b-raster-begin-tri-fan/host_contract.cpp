#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char dmaA[256], dmaB[256];
static unsigned char* const arenaBase = dmaA;
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arenaBase + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arenaBase); memcpy(field, &v, 4); }
#include "begin_tri_fan.cpp"

static unsigned char* gRaster;
static unsigned char* gNewDma;
static int log[8], logCount;
extern "C" int UploadRasterMicro__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer) {
    if (tagPointer != static_cast<unsigned char*>(raster) + 19492) return 99;
    log[logCount++] = 1;
    return 0;
}
extern "C" int CalcMatrices__8VIRaster(void*) { log[logCount++] = 2; return 0; }
extern "C" int UploadMatrices__8VIRasterPP10_sceDmaTag(void* raster, void* tagPointer) {
    if (tagPointer != static_cast<unsigned char*>(raster) + 19492) return 99;
    log[logCount++] = 3;
    // The pointer is read after this call, so a changed value must be honoured.
    norrath_host_store_pointer(static_cast<unsigned char*>(raster) + 19492, gNewDma);
    return 0;
}

int main() {
    alignas(16) static unsigned char object[20000], want[20000];
    struct Case { int micro, flag; } cases[] = {{1, 0}, {0, 0}, {2, 5}, {1, -1}};
    for (const Case& c : cases) {
        memset(object, 0x6b, sizeof object);
        gRaster = object; gNewDma = dmaB; logCount = 0;
        norrath_host_store_pointer(object + 19492, dmaA);
        memcpy(object + 19532, &c.micro, 4);
        memcpy(object + 17888, &c.flag, 4);
        memcpy(want, object, sizeof object);
        unsigned char* dma = c.flag ? dmaB : dmaA;
        int zero = 0, two = 2;
        unsigned char* p52 = dma + 48; unsigned char* p56 = dma + 16;
        memcpy(want + 48, &zero, 4); norrath_host_store_pointer(want + 52, p52); norrath_host_store_pointer(want + 56, p56);
        memcpy(want + 17884, &two, 4);
        norrath_host_store_pointer(want + 19492, dma);
        if (BeginTriFan__8VIRaster(object) != 0) return 1;
        int expectLog[4], n = 0;
        if (c.micro != 1) expectLog[n++] = 1;
        if (c.flag) { expectLog[n++] = 2; expectLog[n++] = 3; }
        if (n != logCount) return 2;
        for (int i = 0; i < n; ++i) if (log[i] != expectLog[i]) return 3;
        if (memcmp(object, want, sizeof object) != 0) return 4;
    }
    return 0;
}
