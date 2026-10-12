#include <cstdio>
extern "C" int amxLightEffect__FP5__amxPi(void*, int*);
static int point_cells[3] = {2, -4, 100};
static int block_marker = 1;
static int allocs = 0;
static int calls = 0;
static int fail_alloc = 0;
static float seen_point[3];
static float seen_scaled = 0.f;
static float seen_one = 0.f;
static float seen_p3 = 0.f;
static float seen_p12 = 0.f;
static void* seen_object = &block_marker;
extern "C" int amx_GetAddr(void*, int address, void** physical) {
    if (address != 8) return 1;
    *physical = point_cells;
    return 0;
}
extern "C" void* blockAlloc__Fi(int bytes) {
    allocs += 1;
    if (bytes != 188 || fail_alloc) return nullptr;
    return &block_marker;
}
extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point,
    float p3, float, float, float, float, float, float, float,
    float, float p12, float one, float scaled, void* object, int flag) {
    calls += 1;
    if (block != &block_marker || flag != 0) calls = -100;
    seen_point[0] = point[0];
    seen_point[1] = point[1];
    seen_point[2] = point[2];
    seen_p3 = p3;
    seen_p12 = p12;
    seen_one = one;
    seen_scaled = scaled;
    seen_object = object;
}

int main() {
    int cells[] = {52, 8, 100, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    fail_alloc = 1;
    if (amxLightEffect__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (allocs != 1 || calls != 0) return 2;
    fail_alloc = 0;
    if (amxLightEffect__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (calls != 1 || seen_object != nullptr) return 4;
    if (seen_point[0] != 2.f || seen_point[1] != -4.f || seen_point[2] != 100.f) return 5;
    if (seen_p3 != 3.f || seen_p12 != 12.f || seen_one != 1.f) return 6;
    if (seen_scaled != 1.f) return 7;
    std::puts("amxLightEffect passed");
    return 0;
}
