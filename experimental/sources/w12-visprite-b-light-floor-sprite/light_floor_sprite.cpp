// Observed lightFloorSprite__FG6Point3 at 0x0013d700, 624 bytes in the pinned SLUS-20565 image.
// Argument: a 12-byte point (passed by value, so the callee receives a pointer to a caller copy and
// copies it to the stack). Returns an int made of three 0..255 values.
//
// Light words used: signed byte +9, halfwords +10, +12, +14, floats +28 (radius), +32 and +48, +52,
// +56 (position). Other data: scene (0x004f4060: float at +8, bytes at +44, +45, +46, +48, +49,
// +50) and world.
//  1. Three float sums start at 0. findLightsAtPoint__FG6Point3PP5Lighti(point, list, 32) fills a
//     null-terminated list. For each light up to the first null whose byte +9 is nonzero:
//     d = position - point; dist2 = (dx*dx + dy*dy) + dz*dz; inv = rsqrt.s(1.0f, dist2);
//     atten = ((radius*radius - dist2) * float(+32)) * (dz * inv). The original reloads byte +9
//     after the float stores and, if it were zero there, multiplies by 0.25f first; that path is
//     only reachable if the byte changed in between and is kept here as observed. atten is then
//     limited with max.s against 0.0f and min.s against 1.0f. The sums for halfwords +10, +12 and
//     +14 each gain (float)halfword * atten.
//  2. s = scene float +8, multiplied by worldQueryLightMap(world, point) when world is nonzero,
//     then max.s against 0.0f. The sum for +10 gains (float)scene[44] * s then (float)scene[48];
//     the sum for +12 gains (float)scene[45] * s then (float)scene[49]; the sum for +14 gains
//     (float)scene[46] * s then (float)scene[50].
//  3. Each sum is limited to 0.0f..255.0f (max.s then min.s) and converted to an int. The result is
//     value(+10 sum) | value(+12 sum) << 8 | value(+14 sum) << 16.
// Inferred: this packs a floor sprite colour from nearby lights plus the scene's own light.
// Unknown: which colour channel each halfword is and the units of the scene bytes. Nothing here was
// executed on the EE.
struct Point3 {
    float x, y, z;
};

extern "C" {
void findLightsAtPoint__FG6Point3PP5Lighti(Point3, const unsigned char**, int);
float worldQueryLightMap__FP12_worldHeaderG6Point3(void*, Point3);
extern unsigned char scene[56];
extern void* world;
}

namespace {
inline float fl(const unsigned char* base, int off) { return *reinterpret_cast<const float*>(base + off); }
inline short shw(const unsigned char* base, int off) { return *reinterpret_cast<const short*>(base + off); }
inline float min_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b < a ? b : a;
#endif
}
inline float max_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b > a ? b : a;
#endif
}
inline float reciprocal_sqrt(float value) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(value));
    return result;
#else
    return 1.0f / __builtin_sqrtf(value);
#endif
}
inline int channel(float sum) { return static_cast<int>(min_s(max_s(sum, 0.0f), 255.0f)); }
}  // namespace

extern "C" int lightFloorSprite__FG6Point3(Point3 position) {
    const Point3 point = position;
    const unsigned char* list[32];
    float sum14 = 0.0f;
    float sum10 = 0.0f;
    float sum12 = 0.0f;
    findLightsAtPoint__FG6Point3PP5Lighti(point, list, 32);
    for (int i = 0; list[i] != nullptr; ++i) {
        const unsigned char* light = list[i];
        if (static_cast<signed char>(light[9]) == 0) continue;
        Point3 d;
        d.x = fl(light, 48) - point.x;
        d.y = fl(light, 52) - point.y;
        d.z = fl(light, 56) - point.z;
        const float dist2 = (d.x * d.x + d.y * d.y) + d.z * d.z;
        const float radius = fl(light, 28);
        float atten = (radius * radius - dist2) * fl(light, 32);
        const float inv = reciprocal_sqrt(dist2);
        if (static_cast<signed char>(light[9]) == 0) atten = atten * 0.25f;
        atten = atten * (d.z * inv);
        atten = min_s(max_s(atten, 0.0f), 1.0f);
        sum14 += static_cast<float>(shw(light, 14)) * atten;
        sum10 += static_cast<float>(shw(light, 10)) * atten;
        sum12 += static_cast<float>(shw(light, 12)) * atten;
    }
    float s = fl(scene, 8);
    if (world != nullptr) s = s * worldQueryLightMap__FP12_worldHeaderG6Point3(world, point);
    s = max_s(s, 0.0f);
    sum12 += static_cast<float>(scene[45]) * s;
    sum10 += static_cast<float>(scene[44]) * s;
    sum14 += static_cast<float>(scene[46]) * s;
    sum12 += static_cast<float>(scene[49]);
    sum10 += static_cast<float>(scene[48]);
    const int c12 = channel(sum12);
    sum14 += static_cast<float>(scene[50]);
    const int c10 = channel(sum10);
    const int c14 = channel(sum14);
    return c10 | (c12 << 8) | (c14 << 16);
}
