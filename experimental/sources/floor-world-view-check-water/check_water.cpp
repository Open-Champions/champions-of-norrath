// Observed worldCheckForWater__FP12_worldHeaderG6Point3fPf at 0x0017db78,
// 216 bytes. The point is the 12 bytes addressed by the second argument.
// The minimum corner is (x - radius - 1, y - radius - 1, z - 1). The
// maximum corner is (x + radius + 1, y + radius + 1, z + 60). Both 1 and
// 60 are immediate bits, 0x3f800000 and 0x42700000. The fifth argument to
// worldFindWaterPatches is 2. A null output pointer skips the height store.
// A null patch pointer also skips it. Otherwise the float at patch+16 is
// stored through the output pointer. The result is 1 when that patch
// pointer is non-null.
extern "C" int worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
    void* world, const float* minimum, const float* maximum, void** patch, int mode);

namespace {
float load_float(const unsigned char* address) {
    union {
        unsigned bits;
        float value;
    } converted;
    converted.bits = address[0] | (unsigned(address[1]) << 8) | (unsigned(address[2]) << 16) |
                     (unsigned(address[3]) << 24);
    return converted.value;
}
float load_float_ptr(const void* address) {
    return load_float(static_cast<const unsigned char*>(address));
}
}

extern "C" int norrath_world_check_for_water(void* world, const void* point, float radius, float* output) {
    const auto* bytes = static_cast<const unsigned char*>(point);
    const float x = load_float(bytes);
    const float y = load_float(bytes + 4);
    const float z = load_float(bytes + 8);
    const float one = 1.0f;
    const float rise = 60.0f;
    float minimum[3];
    float maximum[3];
    minimum[0] = (x - radius) - one;
    minimum[1] = (y - radius) - one;
    minimum[2] = z - one;
    maximum[0] = (x + radius) + one;
    maximum[1] = (y + radius) + one;
    maximum[2] = z + rise;
    void* patch = nullptr;
    worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
        world, minimum, maximum, &patch, 2);
    if (output != nullptr && patch != nullptr) {
        *output = load_float_ptr(static_cast<unsigned char*>(patch) + 16);
    }
    return patch != nullptr ? 1 : 0;
}
