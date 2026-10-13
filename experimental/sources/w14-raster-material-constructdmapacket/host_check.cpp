// Host check only; not a build input. The time query, Process and the three pass builders are stubbed and log their calls.
#include "construct.cpp"
#include <cstdio>
#include <cstring>
static int log_[16][2];  // builder kind (0 wire, 1 text, 2 solid), pass index
static unsigned* outSeen[16];
static int nlog, processCalls, processTime, timeCalls;
extern "C" int GetTime__8VIRaster(void*) { ++timeCalls; return 99; }
extern "C" void Process__10VIMateriall(void*, int t) { ++processCalls; processTime = t; }
extern "C" void ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord(void*, int pass, void*, void* out) { log_[nlog][0] = 0; log_[nlog][1] = pass; outSeen[nlog++] = static_cast<unsigned*>(out); }
extern "C" void ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord(void*, int pass, void*, void* out) { log_[nlog][0] = 1; log_[nlog][1] = pass; outSeen[nlog++] = static_cast<unsigned*>(out); }
extern "C" void ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord(void*, int pass, void*, void* out) { log_[nlog][0] = 2; log_[nlog][1] = pass; outSeen[nlog++] = static_cast<unsigned*>(out); }
int main() {
    alignas(16) unsigned char material[512] = {};
    alignas(16) unsigned char raster[18000] = {};
    alignas(16) static unsigned out[400];
    unsigned* m = reinterpret_cast<unsigned*>(material);
    m[2] = 3;                      // three passes (used when the raster flag is set)
    m[3] = 4;                      // animated
    m[8] = 0x55;
    // record 0 wire (flag 0x20), record 1 text (word 0 != -1), record 2 solid fill (word 0 == -1)
    m[12 + 1] = 0x20;
    m[12 + 36] = 7;
    m[12 + 72] = 0xffffffffu;
    *reinterpret_cast<int*>(raster + 17816) = 1;
    int r = ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(material, raster, out);
    bool ok = r == 2 + 17 * 3 && timeCalls == 1 && processCalls == 1 && processTime == 99 && m[8] == 0 &&
              out[0] == 0 && out[1] == 0 && out[2] == 0x10000000u && out[3] == ((52u << 16) | 0x6c0001b3u) && out[4] == 3 && out[5] == 0 && out[6] == 0 && out[7] == 0;
    ok = ok && nlog == 3 && log_[0][0] == 0 && log_[1][0] == 1 && log_[2][0] == 2 && log_[0][1] == 0 && log_[1][1] == 1 && log_[2][1] == 2 &&
         outSeen[0] == out + 8 && outSeen[1] == out + 8 + 68 && outSeen[2] == out + 8 + 136;
    // the raster flag clear means one pass, and a non-animated material makes no time query
    nlog = 0; m[3] = 0; *reinterpret_cast<int*>(raster + 17816) = 0;
    r = ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(material, raster, out);
    ok = ok && r == 19 && timeCalls == 1 && nlog == 1 && log_[0][0] == 0 && out[4] == 1 && out[3] == ((18u << 16) | 0x6c0001b3u);
    // a zero pass count runs no builder
    nlog = 0; *reinterpret_cast<int*>(raster + 17816) = 1; m[2] = 0;
    r = ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(material, raster, out);
    ok = ok && r == 2 && nlog == 0 && out[4] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
