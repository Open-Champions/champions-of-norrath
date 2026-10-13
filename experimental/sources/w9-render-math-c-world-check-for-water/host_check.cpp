// Host contract for worldCheckForWater__FP12_worldHeaderG6Point3fPf. Not an EE plan input.
// worldFindWaterPatches is a recorded stub. Built on the host, patch pointers are real
// pointers, so the function is exercised with a patch object on the host heap.
#include <cstdio>
#include <cstring>
static float seen_low[3], seen_high[3];
static int seen_limit = 0;
static void* seen_world = nullptr;
static unsigned char patch_storage[32];
static unsigned char* answer = nullptr;
extern "C" void worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(void* w, const float* lo, const float* hi, unsigned char** out, int limit) {
    seen_world = w; std::memcpy(seen_low, lo, 12); std::memcpy(seen_high, hi, 12); seen_limit = limit; out[0] = answer; out[1] = nullptr;
}
extern "C" int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, const float*, float, float*);
int main() {
    int world = 0;
    const float p[3] = {10.0f, 20.0f, 30.0f};
    float h = -1.0f;
    answer = nullptr;
    if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(&world, p, 4.0f, &h) != 0 || h != -1.0f) return 1;
    if (seen_world != &world || seen_limit != 2) return 2;
    if (seen_low[0] != 5.0f || seen_low[1] != 15.0f || seen_low[2] != 29.0f) return 3;
    if (seen_high[0] != 15.0f || seen_high[1] != 25.0f || seen_high[2] != 90.0f) return 4;
    float f = 12.5f; std::memcpy(patch_storage + 16, &f, 4);
    answer = patch_storage;
    if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(&world, p, 4.0f, &h) != 1 || h != 12.5f) return 5;
    if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(&world, p, 4.0f, nullptr) != 1) return 6;
    std::printf("world check for water contract passed\n");
    return 0;
}
