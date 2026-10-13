// Host contract for worldPrecacheWorld__FP12_worldHeaderP8Matrix34. Not an EE plan input.
// Build with -DNORRATH_HOST_WORLD; stored pointers are arena offsets.
#include <cstdio>
#include <cstring>
#include <vector>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_world_arena = arena;
struct Matrix34;
extern "C" {
unsigned char worldLastDrawSucceded = 1;
signed char texturePageTable[16];
int texturePageLoc[4];
float worldCameraMatrix[12];
}
static std::vector<int> frustum_calls;
static std::vector<std::pair<unsigned, int>> cached;
static int clip_result = 0x803f, clip_calls;
extern "C" void worldTransformFrustum__FP8Matrix34(Matrix34* m) {
    frustum_calls.push_back(m == reinterpret_cast<Matrix34*>(worldCameraMatrix) ? 2 : 1);
}
extern "C" int worldClipCubeToFrustum__FPA1_fi(float*, int) { ++clip_calls; return clip_result; }
extern "C" void texCacheData__FP8_texturei(void* t, int page) {
    cached.push_back({unsigned(static_cast<unsigned char*>(t) - arena), page});
}
extern "C" void worldPrecacheWorld__FP12_worldHeaderP8Matrix34(void*, Matrix34*);
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, int v) { short s = short(v); std::memcpy(arena + off, &s, 2); }
int main() {
    std::memset(texturePageTable, -1, sizeof texturePageTable);
    // header at 0: tree at 0x100 (a leaf of three records), records at 0x200.
    put32(4, 0x100); put32(36, 0x200);
    put16(0x100, 3); put16(0x102, 0); put16(0x104, 1); put16(0x106, 2);
    // record 0: loaded page 5 (slot 1), texture at 0x1000; record 1: unloaded page 6; record 2: flagged out.
    put16(0x200 + 52, 5); put32(0x200 + 32, 0x20);
    put16(0x200 + 60 + 52, 6);
    put16(0x200 + 120 + 52, 5); put32(0x200 + 120 + 36, 0x80);
    texturePageTable[5] = 1; texturePageLoc[1] = 0x1000 - 0x20;
    put16(0x1000 + 8, 0x100); put32(0x1000 + 20, 0);
    auto w = reinterpret_cast<void*>(arena);
    worldPrecacheWorld__FP12_worldHeaderP8Matrix34(w, reinterpret_cast<Matrix34*>(arena + 0x3000));
    if (frustum_calls.size() != 2 || frustum_calls[0] != 1 || frustum_calls[1] != 2) return 1;
    if (cached.size() != 1 || cached[0].first != 0x1000u || cached[0].second != 5) return 2;
    if (worldLastDrawSucceded != 0) return 3;                    // record 1 hit an unloaded page
    if (clip_calls != 3) return 4;                               // root mask 0x8000 is not 0x803f: each record is clipped
    std::printf("world precache world contract passed\n");
    return 0;
}
