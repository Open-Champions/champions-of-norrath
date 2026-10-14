#include "raster_init.cpp"

#include <cstring>
#include <string>

unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");

alignas(8) static unsigned char object[20000];
static std::string trace;
static int init_result;
static unsigned char db_seen_flag;

static unsigned long long rd64(const unsigned char* base, int off) {
    unsigned long long v;
    std::memcpy(&v, base + off, 8);
    return v;
}
static void wr32f(int off, float v) { std::memcpy(object + off, &v, 4); }

extern "C" int sceGsResetGraph(int a, int b, int c, int d) {
    trace += "R" + std::to_string(a) + std::to_string(b) + std::to_string(c) + std::to_string(d) + ";";
    return 0;
}
extern "C" int sceGsSetDefDBuffDc(sceGsDBuffDc* db, int psm, int w, int h, int zt, int zp, int clr) {
    trace += "D" + std::string(reinterpret_cast<unsigned char*>(db) == norrath_raster_dbuffer ? "" : "?") +
             std::to_string(psm) + "," + std::to_string(w) + "," + std::to_string(h) + "," +
             std::to_string(zt) + "," + std::to_string(zp) + "," + std::to_string(clr) + ";";
    std::memset(norrath_raster_dbuffer, 0xff, sizeof norrath_raster_dbuffer);
    return 0;
}
extern "C" void FlushCache(int mode) { trace += "F" + std::to_string(mode) + ";"; }
extern "C" void InitSPRDoubleBuffer__8VIRaster(VIRaster*) { trace += "S;"; }
extern "C" int Init__8VIRaster(VIRaster* r) {
    trace += reinterpret_cast<unsigned char*>(r) == object ? "I;" : "I?;";
    return init_result;
}

static void prepare(int flag_unused) {
    (void)flag_unused;
    std::memset(object, 0x11, sizeof object);
    std::memset(norrath_raster_dbuffer, 0x22, sizeof norrath_raster_dbuffer);
    wr32f(17788, 1.0f);
    wr32f(17792, 0.5f);
    wr32f(17796, 0.0f);
    wr32f(17800, 0.2f);
    trace.clear();
}

int main() {
    // Wrong size: -1, nothing else touched (flag byte is still stored).
    prepare(0);
    if (Init__8VIRasterPviii(reinterpret_cast<VIRaster*>(object), 0, 640, 480, 1) != -1) return 1;
    if (!trace.empty() || object[19536] != 1) return 2;
    if (object[17880] != 0x11 || object[17748] != 0x11) return 3;
    prepare(0);
    if (Init__8VIRasterPviii(reinterpret_cast<VIRaster*>(object), 0, 320, 448, 0) != -1) return 4;
    if (!trace.empty() || object[19536] != 0) return 5;

    // Flag set: no ResetGraph.
    prepare(0);
    init_result = 5;
    if (Init__8VIRasterPviii(reinterpret_cast<VIRaster*>(object), 0, 640, 448, 3) != 0) return 6;
    if (trace != "D10,640,448,3,49,1;F0;S;I;") return 7;

    // Flag clear: ResetGraph(0,1,2,0) first; Init failure returns -1.
    prepare(0);
    init_result = -4;
    if (Init__8VIRasterPviii(reinterpret_cast<VIRaster*>(object), 0, 640, 448, 0) != -1) return 8;
    if (trace != "R0120;D10,640,448,3,49,1;F0;S;I;") return 9;

    // Stored values after the flag-clear run above.
    int word;
    std::memcpy(&word, object + 17880, 4);
    if (word != 0) return 10;
    std::memcpy(&word, object + 17748, 4);
    if (word != 640) return 11;
    std::memcpy(&word, object + 17752, 4);
    if (word != 448) return 12;
    float f;
    std::memcpy(&f, object + 17772, 4);
    if (f != 16.0f) return 13;
    unsigned u;
    std::memcpy(&u, object + 17776, 4);
    if (u != 0x4b000000u) return 14;

    // The buffer started as all ones (as left by the stub), so masked words
    // keep everything but the low nine bits.
    const unsigned char* db = norrath_raster_dbuffer;
    unsigned w16;
    std::memcpy(&w16, db + 16, 4);
    if (w16 != ((0xffffffffu & 0xfffffe00u) | 0x46u)) return 15;
    const unsigned long long mask = ~0x1ffull;
    if (rd64(db, 464) != ((0xffffffffffffffffull & mask) | 70)) return 16;
    if (rd64(db, 592) != ((~0ull & mask) | 70)) return 17;
    for (int off : {112, 240, 480, 608})
        if (rd64(db, off) != ((~0ull & mask) | 140)) return 18;
    for (int off : {96, 224})
        if (rd64(db, off) != (~0ull & mask)) return 19;
    // 255, 128, 0 and 51 packed with 0x3f800000 above.
    unsigned long long packed = 0x3f80000000000000ull | (51ull << 24) | (0ull << 16) | (128ull << 8) | 255ull;
    if (rd64(db, 752) != packed || rd64(db, 384) != packed) return 20;
    for (int off : {688, 192, 320, 560})
        if (rd64(db, off) != 1) return 21;
    // Untouched buffer words remain as the stub left them.
    if (rd64(db, 0) != ~0ull || rd64(db, 8) != ~0ull) return 22;

    // Color conversion rounds: 0.999 -> 255, 0.001 -> 0, 0.333 -> 85.
    prepare(0);
    wr32f(17788, 0.999f);
    wr32f(17792, 0.001f);
    wr32f(17796, 0.333f);
    wr32f(17800, 1.0f);
    init_result = 0;
    if (Init__8VIRasterPviii(reinterpret_cast<VIRaster*>(object), 0, 640, 448, 1) != 0) return 23;
    unsigned long long packed2 = 0x3f80000000000000ull | (255ull << 24) | (85ull << 16) | (0ull << 8) | 255ull;
    if (rd64(norrath_raster_dbuffer, 752) != packed2) return 24;
    return 0;
}
