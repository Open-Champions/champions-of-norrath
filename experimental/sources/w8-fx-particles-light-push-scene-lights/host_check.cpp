// Host contract for lightPushSceneLights__FG6Point3iiiiiiiii. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" {
int interfaceMode = 0;
unsigned char scene[56];
unsigned char savedScene[56];
void lightPushSceneLights__FG6Point3iiiiiiiii(const float*, int, int, int, int, int, int, int, int, int);
}
static int transform_calls = 0, reset_calls = 0;
static unsigned char byte42_at_transform = 0;
static int order = 0, transform_at = 0, reset_at = 0;
extern "C" void transformSceneLightTask__Fv() { ++transform_calls; transform_at = ++order; byte42_at_transform = scene[42]; }
extern "C" void lightResetOptionalParams__Fv() { ++reset_calls; reset_at = ++order; }

static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}

int main() {
    for (int n = 0; n < 56; ++n) scene[n] = static_cast<unsigned char>(n + 1);
    std::memset(savedScene, 0, sizeof savedScene);
    unsigned char before[56];
    std::memcpy(before, scene, 56);
    const float dir[3] = {3.0f, 0.0f, 4.0f};
    lightPushSceneLights__FG6Point3iiiiiiiii(dir, 0x101, 2, 3, 4, 5, 6, 7, 8, 0x12345678);
    expect(interfaceMode == 1, "interfaceMode incremented");
    expect(std::memcmp(savedScene, before, 56) == 0, "old scene saved before it is overwritten");
    float out[3];
    std::memcpy(out, scene, 12);
    expect(std::fabs(out[0] - 0.6f) < 1e-5f && out[1] == 0.0f && std::fabs(out[2] - 0.8f) < 1e-5f, "direction normalized");
    const int expect_bytes[][2] = {{36, 1}, {37, 2}, {38, 3}, {39, 4}, {40, 5}, {41, 6}, {42, 7}, {43, 8},
                                   {44, 1}, {45, 2}, {46, 3}, {47, 4}, {48, 5}, {49, 6}, {50, 7}, {51, 8}};
    for (const auto& e : expect_bytes) expect(scene[e[0]] == e[1], "scene byte");
    unsigned w; std::memcpy(&w, scene + 52, 4);
    expect(w == 0x12345678u, "word 52 takes the ninth integer");
    expect(scene[12] == 13 && scene[35] == 36, "other bytes untouched");
    expect(transform_calls == 1 && reset_calls == 1 && transform_at == 1 && reset_at == 2, "task then reset");
    expect(byte42_at_transform == 7, "byte 42 is stored before the task runs");

    // Only the low byte of each of the first eight integers is stored.
    interfaceMode = -3;
    lightPushSceneLights__FG6Point3iiiiiiiii(dir, 0x1ff, 0, 0, 0, 0, 0, 0, 0x2aa, 0);
    expect(scene[36] == 0xff && scene[44] == 0xff && scene[51] == 0xaa, "low bytes only");
    expect(interfaceMode == -2, "negative mode increments");

    // A positive mode returns without touching anything.
    interfaceMode = 1;
    std::memset(savedScene, 0x77, sizeof savedScene);
    std::memcpy(before, scene, 56);
    lightPushSceneLights__FG6Point3iiiiiiiii(dir, 9, 9, 9, 9, 9, 9, 9, 9, 9);
    expect(interfaceMode == 1 && std::memcmp(scene, before, 56) == 0 && savedScene[0] == 0x77, "positive mode skips");
    expect(transform_calls == 2 && reset_calls == 2, "no calls when skipped");
    if (failures) return 1;
    std::printf("light-push-scene-lights contract passed\n");
    return 0;
}
