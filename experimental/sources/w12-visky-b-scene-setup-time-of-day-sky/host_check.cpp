#include "setup_sky.cpp"

#include <cstring>

static float overcast_result;
static int overcast_calls;
static void* overcast_arg;
static void* sky_atmosphere;
static void* sky_scene;
static unsigned char sky_color[4];
static int sky_calls;
static void* lava_atmosphere;
static float lava_value;
static int lava_calls;
static int order[4];
static int order_n;

extern "C" float GetOvercastInterp__12VIAtmosphere(void* a) { overcast_arg = a; ++overcast_calls; order[order_n++] = 1; return overcast_result; }
extern "C" void SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(void* a, void* s, const void* c) {
    sky_atmosphere = a; sky_scene = s; std::memcpy(sky_color, c, 4); ++sky_calls; order[order_n++] = 2;
}
extern "C" void SetLavastormIntensity__12VIAtmospheref(void* a, float v) { lava_atmosphere = a; lava_value = v; ++lava_calls; order[order_n++] = 3; }

alignas(8) static unsigned char scene[12000];
static VITerrainProfileBlend blend;

static void put(int offset, float v) { std::memcpy(scene + offset, &v, 4); }

static void reset() {
    std::memset(scene, 0, sizeof scene);
    std::memset(&blend, 0, sizeof blend);
    overcast_calls = sky_calls = lava_calls = order_n = 0;
}

static int run(float time) {
    put(8160, time);
    return SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend(reinterpret_cast<VIScene*>(scene), &blend);
}

int main() {
    // Profile 2 gets full weight; its tint is (1, 0.5, 0.25, 1) and its lava value 0.75.
    reset();
    blend.profile[0] = blend.profile[1] = blend.profile[2] = blend.profile[3] = 2;
    blend.weight[0] = 1.0f;
    put(100 + 80, 1.0f); put(100 + 84, 0.5f); put(100 + 88, 0.25f); put(100 + 92, 1.0f);
    put(128 + 80, 0.75f);
    // Hour 5: first slot (0.5, 1, 0.5, 1) and second slot (0, 0, 1, 1).
    put(8256 + 108 * 5, 0.5f); put(8260 + 108 * 5, 1.0f); put(8264 + 108 * 5, 0.5f); put(8268 + 108 * 5, 1.0f);
    put(8272 + 108 * 5, 0.0f); put(8276 + 108 * 5, 0.0f); put(8280 + 108 * 5, 1.0f); put(8284 + 108 * 5, 1.0f);
    overcast_result = 0.0f;
    if (run(5.0f) != 0) return 1;
    if (order_n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 2;
    if (overcast_arg != scene + 1600 || sky_atmosphere != scene + 1600 || lava_atmosphere != scene + 1600) return 3;
    if (sky_scene != scene) return 4;
    // overcast 0: the first slot times the tint: (0.5, 0.5, 0.125, 1) -> bytes 128, 128, 32, 255.
    if (sky_color[0] != 128 || sky_color[1] != 128 || sky_color[2] != 32 || sky_color[3] != 255) return 5;
    if (lava_value != 0.75f) return 6;
    // overcast 1: the second slot times the tint: (0, 0, 0.25, 1) -> bytes 0, 0, 64, 255.
    overcast_result = 1.0f;
    run(5.0f);
    if (sky_color[0] != 0 || sky_color[1] != 0 || sky_color[2] != 64 || sky_color[3] != 255) return 7;
    // overcast 0.5: the halfway colour (0.25, 0.5, 0.75, 1) times the tint: (0.25, 0.25, 0.1875, 1).
    overcast_result = 0.5f;
    run(5.0f);
    if (sky_color[0] != 64 || sky_color[1] != 64 || sky_color[2] != 48 || sky_color[3] != 255) return 8;
    // Halfway between hour 5 and 6 (hour 6 first slot is (1, 1, 1, 1)); overcast 0: first slot mix
    // (0.75, 1, 0.75, 1) times the tint: (0.75, 0.5, 0.1875, 1).
    for (int i = 0; i < 4; ++i) put(8256 + 108 * 6 + 4 * i, 1.0f);
    overcast_result = 0.0f;
    run(5.5f);
    if (sky_color[0] != 191 || sky_color[1] != 128 || sky_color[2] != 48 || sky_color[3] != 255) return 9;
    // At 23.5 the next record is hour 0, not hour 24 (which would be beyond the table).
    for (int i = 0; i < 4; ++i) { put(8256 + 108 * 23 + 4 * i, 0.0f); put(8256 + 4 * i, 1.0f); }
    run(23.5f);
    if (sky_color[0] != 128 || sky_color[1] != 64 || sky_color[2] != 32 || sky_color[3] != 128) return 10;
    // Weighted sums: profiles 0 and 1 at weight 0.5 each; profile 0 has tint 1 and lava value 0.5,
    // profile 1 has tint 0 and lava value 1.
    blend.profile[0] = 0; blend.profile[1] = 1; blend.weight[0] = 0.5f; blend.weight[1] = 0.5f; blend.weight[2] = 0.0f;
    put(100, 1.0f); put(104, 1.0f); put(108, 1.0f); put(112, 1.0f);
    put(128, 0.5f); put(168, 1.0f);
    put(100 + 40, 0.0f); put(104 + 40, 0.0f); put(108 + 40, 0.0f); put(112 + 40, 0.0f);
    run(23.5f);
    if (lava_value != 0.5f * 0.5f + 1.0f * 0.5f) return 11;
    // The sky is half the halfway colour (0.5 each), so the alpha 0.5 * 0.5 -> byte 64.
    if (sky_color[3] != 64) return 12;
    return 0;
}
