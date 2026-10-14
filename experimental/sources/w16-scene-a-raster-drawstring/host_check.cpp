// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int cacheResult, cacheGlyphs[8], addMaterialResult, vertexCalls, endCalls, microCalls, matrixCalls, uploadMatrixCalls;
static int vx[16][2]; static float vu[16][2];
extern "C" int Cache__7VIRFontR8VIRasterPCUsiPi(unsigned char*, unsigned char*, const unsigned short*, int limit, int* out) {
    if (limit != 256) return -5;
    for (int i = 0; i < 8; ++i) out[i] = cacheGlyphs[i];
    return cacheResult;
}
extern "C" int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char*, int, unsigned*) { return addMaterialResult; }
extern "C" void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned*) { ++microCalls; }
extern "C" void CalcMatrices__8VIRaster(unsigned char*) { ++matrixCalls; }
extern "C" void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned*) { ++uploadMatrixCalls; }
extern "C" void Vertex__8VIRasterRC8VIVect2I(unsigned char* r, const int* p) {
    vx[vertexCalls][0] = p[0]; vx[vertexCalls][1] = p[1];
    std::memcpy(vu[vertexCalls], r + 12, 8);
    ++vertexCalls;
}
extern "C" void EndTriFan__8VIRaster(unsigned char*) { ++endCalls; }
#include "draw_string.cpp"
#include <cstdio>
static int& W(unsigned off) { return *reinterpret_cast<int*>(arena + off); }
int main() {
    // Layout inside the arena: raster at 0, font table at 0x6000, font at 0x6100, glyph records at 0x6200, slots at 0x6400.
    W(19516) = arena_base + 0x6000; W(0x6000 + 12) = arena_base + 0x6100;
    W(0x6100 + 16) = 9; W(0x6100 + 20) = 12; W(0x6100 + 64) = arena_base + 0x6200; W(0x6100 + 68) = arena_base + 0x6400;
    *reinterpret_cast<unsigned short*>(arena + 0x6200 + 12 * 3 + 8) = 1; arena[0x6200 + 12 * 3 + 10] = 7;
    *reinterpret_cast<unsigned short*>(arena + 0x6200 + 12 * 5 + 8) = 0; arena[0x6200 + 12 * 5 + 10] = 4;
    float uv[2][8] = {{0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f}, {1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f}};
    std::memcpy(arena + 0x6400 + 24, uv[0], 32); std::memcpy(arena + 0x6400 + 56 + 24, uv[1], 32);
    W(19532) = 1; W(17888) = 0; W(19492) = 0x3000;
    int pos[2] = {100, 50};
    unsigned char color[16]; for (int i = 0; i < 16; ++i) color[i] = static_cast<unsigned char>(0xa0 + i);
    cacheResult = 2; cacheGlyphs[0] = 3; cacheGlyphs[1] = 5; addMaterialResult = 0;
    bool ok = DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(arena, 1, pos, color, nullptr) == 0;
    ok = ok && vertexCalls == 8 && endCalls == 2 && microCalls == 0 && matrixCalls == 0;
    // first glyph (record 3, slot 1): x 100..107, y 50..62, slot 1's floats
    const int ex[8][2] = {{100, 50}, {107, 50}, {107, 62}, {100, 62}, {108, 50}, {112, 50}, {112, 62}, {108, 62}};
    for (int i = 0; i < 8; ++i) ok = ok && vx[i][0] == ex[i][0] && vx[i][1] == ex[i][1];
    for (int k = 0; k < 4; ++k) ok = ok && vu[k][0] == uv[1][2 * k] && vu[k][1] == uv[1][2 * k + 1];
    for (int k = 0; k < 4; ++k) ok = ok && vu[4 + k][0] == uv[0][2 * k] && vu[4 + k][1] == uv[0][2 * k + 1];
    ok = ok && std::memcmp(arena + 32, color, 16) == 0 && W(17884) == 2 && W(48) == 0 && W(56) == 0x3010 && W(52) == 0x3030;
    // micro upload and matrices only when their flags say so; failures return -1
    W(19532) = 0; W(17888) = 1; vertexCalls = 0; endCalls = 0;
    ok = ok && DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(arena, 1, pos, color, nullptr) == 0 && microCalls == 2 && matrixCalls == 2 && uploadMatrixCalls == 2;
    cacheResult = -1; ok = ok && DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(arena, 1, pos, color, nullptr) == -1;
    cacheResult = 0; vertexCalls = 0; ok = ok && DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(arena, 1, pos, color, nullptr) == 0 && vertexCalls == 0;
    cacheResult = 2; addMaterialResult = -1; ok = ok && DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(arena, 1, pos, color, nullptr) == -1 && vertexCalls == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
