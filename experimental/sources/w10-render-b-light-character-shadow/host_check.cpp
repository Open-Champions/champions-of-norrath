// Host contract for lightCharacterShadow__FG6Point3P11LightResult. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstdlib>
struct Point3 {
    float x, y, z;
};
struct Light {
    unsigned char pad0[28];
    float radius;
    unsigned char pad1[16];
    Point3 pos;
};
struct LightResult {
    signed char b[12];
};
extern "C" {
float scene[14];
Light* saveBestList[3];
int saveValid;
static Light lights[4];
static int count;
void findLightsAtPoint__FG6Point3PP5Lighti(Point3*, Light** out, int n) {
    for (int i = 0; i < n; ++i) out[i] = i < count ? &lights[i] : nullptr;
}
void lightCharacterShadow__FG6Point3P11LightResult(Point3*, LightResult*);
}
static void setLight(Light& l, float x, float y, float z, float r) {
    l.pos = {x, y, z};
    l.radius = r;
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    setLight(lights[0], 10, 0, 0, 20);
    setLight(lights[1], 0, 100, 0, 200);
    setLight(lights[2], 0, 0, 5, 10);
    setLight(lights[3], 200, 0, 0, 300);
    scene[0] = 0.5f;
    scene[1] = -0.5f;
    count = 4;
    Point3 p = {0, 0, 0};
    LightResult r;
    for (auto& c : r.b) c = 0x55;
    lightCharacterShadow__FG6Point3P11LightResult(&p, &r);
    // keys: -300, -30000, -75, -50000: the three smallest are lights 3, 1, 0.
    expect(saveBestList[0] == &lights[3] && saveBestList[1] == &lights[1] && saveBestList[2] == &lights[0],
           "three smallest keys, in order");
    expect(saveValid == 1, "valid flag");
    expect(std::abs(r.b[0] - 127) <= 1 && r.b[1] == 0 && r.b[2] == 42, "far light scaled to 127");
    expect(r.b[3] == 0 && std::abs(r.b[4] - 127) <= 1 && r.b[5] == 63, "second light");
    expect(r.b[6] == 20 && r.b[7] == 0 && r.b[8] == 63, "near light doubled");
    expect(r.b[9] == 63 && r.b[10] == -63 && r.b[11] == 24, "scene pair and constant");
    count = 1;
    for (auto& c : r.b) c = 0x55;
    lightCharacterShadow__FG6Point3P11LightResult(&p, &r);
    expect(saveBestList[0] == &lights[0] && saveBestList[1] == nullptr && saveBestList[2] == nullptr,
           "missing slots are null");
    expect(r.b[6] == 0x55 && r.b[7] == 0x55 && r.b[8] == 0 && r.b[5] == 0, "unused triples only get a zero third byte");
    count = 0;
    lightCharacterShadow__FG6Point3P11LightResult(&p, &r);
    expect(saveBestList[0] == nullptr && r.b[2] == 0 && r.b[11] == 24, "no lights");
    if (failures) return 1;
    std::printf("lightCharacterShadow contract passed\n");
    return 0;
}
