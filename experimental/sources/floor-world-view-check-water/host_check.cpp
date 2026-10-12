#include <cstdio>
#include <cstring>

extern "C" int norrath_world_check_for_water(void* world, const void* point, float radius, float* output);

namespace {
int calls = 0;
float seen_min[3];
float seen_max[3];
int seen_mode = 0;
void* seen_world = nullptr;
void* patch_result = nullptr;

unsigned bits_of(float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    return bits;
}
}

extern "C" int worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
    void* world, const float* minimum, const float* maximum, void** patch, int mode) {
    ++calls;
    seen_world = world;
    seen_mode = mode;
    std::memcpy(seen_min, minimum, sizeof seen_min);
    std::memcpy(seen_max, maximum, sizeof seen_max);
    *patch = patch_result;
    return 0;
}

int main() {
    unsigned char point[12];
    unsigned point_bits[3] = {0x41200000u, 0x41a00000u, 0x41f00000u};
    std::memcpy(point, point_bits, 12);
    int world = 0;
    unsigned char patch[20];
    unsigned height_bits = 0x42200000u;
    std::memcpy(patch + 16, &height_bits, 4);
    patch_result = patch;
    float output = 0.0f;
    const int found = norrath_world_check_for_water(&world, point, 32.0f, &output);
    if (calls != 1 || seen_world != &world || seen_mode != 2 || found != 1) return 1;
    const float x = 10.0f;
    const float y = 20.0f;
    const float z = 30.0f;
    const float radius = 32.0f;
    if (bits_of(seen_min[0]) != bits_of((x - radius) - 1.0f)) return 2;
    if (bits_of(seen_min[1]) != bits_of((y - radius) - 1.0f)) return 3;
    if (bits_of(seen_min[2]) != bits_of(z - 1.0f)) return 4;
    if (bits_of(seen_max[0]) != bits_of((x + radius) + 1.0f)) return 5;
    if (bits_of(seen_max[1]) != bits_of((y + radius) + 1.0f)) return 6;
    if (bits_of(seen_max[2]) != bits_of(z + 60.0f)) return 7;
    if (bits_of(output) != height_bits) return 8;

    float untouched = 5.0f;
    patch_result = nullptr;
    if (norrath_world_check_for_water(&world, point, 32.0f, &untouched) != 0) return 9;
    if (untouched != 5.0f) return 10;

    patch_result = patch;
    if (norrath_world_check_for_water(&world, point, 32.0f, nullptr) != 1) return 11;
    return 0;
}
