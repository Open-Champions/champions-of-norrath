// Host check only; not a build input. Build with -ffp-contract=off. Stubs supply a light list and a
// light-map factor; the expectations below are exact binary fractions worked out by hand, so no
// rounding choice affects them. The same function was also compared with the original image in a
// private interpreter run (random lights, scene bytes and factors).
#include "light_floor_sprite.cpp"
#include <cstdio>
#include <cstring>

unsigned char scene[56];
void* world;
static unsigned char lights[4][64];
static int light_count;
static float map_factor;
static int find_calls, query_calls, bad_args;

extern "C" {
void findLightsAtPoint__FG6Point3PP5Lighti(Point3 p, const unsigned char** out, int capacity) {
    ++find_calls;
    if (p.x != 1.0f || p.y != 2.0f || p.z != 3.0f || capacity != 32) bad_args = 1;
    for (int i = 0; i < light_count; ++i) out[i] = lights[i];
    out[light_count] = nullptr;
}
float worldQueryLightMap__FP12_worldHeaderG6Point3(void* w, Point3 p) {
    ++query_calls;
    if (w != world || p.x != 1.0f || p.y != 2.0f || p.z != 3.0f) bad_args = 1;
    return map_factor;
}
}

static void light(int i, signed char flag, short a, short b, short c, float radius, float k, float dx, float dy, float dz) {
    std::memset(lights[i], 0x77, sizeof lights[i]);
    lights[i][9] = static_cast<unsigned char>(flag);
    std::memcpy(lights[i] + 10, &a, 2);
    std::memcpy(lights[i] + 12, &b, 2);
    std::memcpy(lights[i] + 14, &c, 2);
    std::memcpy(lights[i] + 28, &radius, 4);
    std::memcpy(lights[i] + 32, &k, 4);
    const float pos[3] = {1.0f + dx, 2.0f + dy, 3.0f + dz};
    std::memcpy(lights[i] + 48, pos, 12);
}
static void set_scene(float s, int b44, int b45, int b46, int b48, int b49, int b50) {
    std::memset(scene, 0, sizeof scene);
    std::memcpy(scene + 8, &s, 4);
    scene[44] = b44; scene[45] = b45; scene[46] = b46; scene[48] = b48; scene[49] = b49; scene[50] = b50;
}
static int failures;
static void expect(int got, int want, const char* what) {
    if (got != want) { std::printf("FAIL %s: got 0x%06x want 0x%06x\n", what, got, want); ++failures; }
}
static int call() { return lightFloorSprite__FG6Point3(Point3{1.0f, 2.0f, 3.0f}); }

int main() {
    light_count = 1;
    light(0, 1, 100, 50, 25, 8.0f, 0.015625f, 0, 0, 4);   // dist2 16, atten (64-16)/64 * (4*0.25) = 0.75
    set_scene(0.5f, 10, 20, 30, 1, 2, 3);
    world = nullptr;
    expect(call(), 81 | (49 << 8) | (36 << 16), "one light, no world");
    if (query_calls != 0) { std::puts("FAIL world not consulted"); ++failures; }

    world = reinterpret_cast<void*>(0x1234);
    map_factor = 2.0f;
    expect(call(), 86 | (59 << 8) | (51 << 16), "world factor doubles the scene term");
    if (query_calls != 1) { std::puts("FAIL world query count"); ++failures; }

    map_factor = -1.0f;
    expect(call(), 76 | (39 << 8) | (21 << 16), "negative factor clamps the scene term to zero");

    world = nullptr;
    light(0, 0, 100, 50, 25, 8.0f, 0.015625f, 0, 0, 4);
    expect(call(), 6 | (12 << 8) | (18 << 16), "flag byte zero skips the light");
    light(0, -1, 100, 50, 25, 8.0f, 0.015625f, 0, 0, 4);
    expect(call(), 81 | (49 << 8) | (36 << 16), "negative flag byte counts as set");

    light(0, 1, 100, 50, 25, 100.0f, 1.0f, 0, 0, 4);       // attenuation far above 1 limits to 1
    expect(call(), 106 | (62 << 8) | (43 << 16), "attenuation limited to 1");
    light(0, 1, 100, 50, 25, 8.0f, 0.015625f, 0, 0, -4);   // behind: negative attenuation limits to 0
    expect(call(), 6 | (12 << 8) | (18 << 16), "negative attenuation limited to 0");

    light_count = 2;
    light(0, 1, 100, 50, 25, 8.0f, 0.015625f, 0, 0, 4);
    light(1, 1, 8, 16, 32, 8.0f, 0.015625f, 0, 4, 0);      // dz 0 gives attenuation 0
    expect(call(), 81 | (49 << 8) | (36 << 16), "second light with zero vertical offset adds nothing");
    light(1, 1, 8, 16, 32, 8.0f, 0.015625f, 0, 0, 4);
    expect(call(), 87 | (61 << 8) | (60 << 16), "two lights add");

    light_count = 0;
    set_scene(2.0f, 255, 255, 255, 255, 255, 255);
    expect(call(), 0xffffff, "sums limited to 255");
    set_scene(-1.0f, 255, 255, 255, 0, 0, 0);
    expect(call(), 0, "negative scene float limited to zero");

    if (bad_args) { std::puts("FAIL callee arguments"); ++failures; }
    if (find_calls != 11) { std::puts("FAIL find call count"); ++failures; }
    std::puts(failures ? "FAIL" : "ok");
    return failures != 0;
}
