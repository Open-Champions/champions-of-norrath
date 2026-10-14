// Host contract for lightSetupForCharacter__FP6PacketG6Point3. Not an EE plan input.
// The callees are stubs. On the host rsqrt.s, min.s and max.s use portable equivalents,
// so the direction rows are compared with a tolerance. Build with -ffp-contract=off.
#include <cmath>
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
struct Packet { unsigned char* cursor; unsigned char* last; };
extern "C" {
int saveValid; const unsigned char* saveBestList[3];
short charAmbient[4], charDirect[4]; unsigned short charDirect2[4];
float charScaleR, charScaleG, charScaleB, charDirectDir[3], charDirectDir2[3], worldCameraMatrix[12];
void* world; int interfaceMode, disableVolumetricLighting;
}
static int findCalls, transformCalls, mapCalls; static const unsigned char* found[32]; static int foundCount;
static float mapScale = 0.5f; static Point3 seenMapPoint;
extern "C" {
void findLightsAtPoint__FG6Point3PP5Lighti(Point3, const unsigned char** out, int n) {
    ++findCalls;
    for (int i = 0; i < foundCount; ++i) out[i] = found[i];
    if (foundCount < n - 1) out[foundCount] = nullptr;
}
Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3* o, const float*, const Point3* in) { ++transformCalls; o->x = -in->x; o->y = in->y; o->z = in->z; return o; }
float worldQueryLightMap__FP12_worldHeaderG6Point3(void*, Point3 p) { ++mapCalls; seenMapPoint = p; return mapScale; }
void lightSetupForCharacter__FP6PacketG6Point3(Packet*, Point3);
}
static float getf(const unsigned char* b, int off) { float v; std::memcpy(&v, b + off, 4); return v; }
static unsigned getw(const unsigned char* b, int off) { unsigned v; std::memcpy(&v, b + off, 4); return v; }
static short geth(const unsigned char* b, int off) { short v; std::memcpy(&v, b + off, 2); return v; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    auto near = [](float a, float b) { return std::fabs(a - b) < 1e-4f; };
    for (int i = 0; i < 4; ++i) { charAmbient[i] = static_cast<short>(10 + i); charDirect[i] = static_cast<short>(100 * (i + 1)); charDirect2[i] = static_cast<unsigned short>(40000 + i); }
    charScaleR = 2.0f; charScaleG = 3.0f; charScaleB = 4.0f;
    for (int i = 0; i < 3; ++i) { charDirectDir[i] = 1.5f + i; charDirectDir2[i] = -2.5f - i; }
    alignas(16) unsigned char buffer[300];
    std::memset(buffer, 0xaa, sizeof buffer);
    Packet packet{buffer + 16, nullptr};
    const unsigned char* base = buffer + 20;

    // No lights found and no saved list: both point-light slots are zero.
    saveValid = 0; foundCount = 0;
    lightSetupForCharacter__FP6PacketG6Point3(&packet, Point3{1, 2, 3});
    expect(findCalls == 1 && transformCalls == 0 && mapCalls == 0, "search once, no transform, no light map");
    expect(packet.cursor == base + 100, "cursor advance");
    expect(getw(buffer, 16) == 0x6c03c000u && getw(base, 48) == 0x14000010u && getw(base, 52) == 0x6d058000u && getw(base, 96) == 0x14000024u, "tag words");
    for (int s = 0; s < 2; ++s) {
        for (int h = 0; h < 4; ++h) expect(geth(base, 56 + 8 * s + 2 * h) == 0, "zero quad");
        expect(getw(base, 4 * s) == 0 && getw(base, 16 + 4 * s) == 0 && getw(base, 32 + 4 * s) == 0, "zero rows");
    }
    for (int h = 0; h < 4; ++h) {
        expect(geth(base, 88 + 2 * h) == 10 + h, "ambient halfwords");
        expect(static_cast<unsigned short>(geth(base, 72 + 2 * h)) == 40000 + h, "slot 2 quad");
        expect(geth(base, 80 + 2 * h) == 100 * (h + 1), "slot 3 quad scaled by 1.0 without a world");
    }
    expect(getf(base, 8) == -2.5f && getf(base, 24) == -3.5f && getf(base, 40) == -4.5f, "slot 2 rows");
    expect(getf(base, 12) == 1.5f && getf(base, 28) == 2.5f && getf(base, 44) == 3.5f, "slot 3 rows");

    // A saved light is used once and clears saveValid; the world scales slot 3.
    alignas(16) unsigned char light[96] = {};
    auto setf = [&](int off, float v) { std::memcpy(light + off, &v, 4); };
    auto seth = [&](int off, short v) { std::memcpy(light + off, &v, 2); };
    setf(28, 10.0f); setf(32, 0.01f); setf(48, 4.0f); setf(52, 2.0f); setf(56, 3.0f);
    seth(10, 100); seth(12, 200); seth(14, 300);
    saveValid = 1; saveBestList[0] = light; saveBestList[1] = nullptr;
    int dummyWorld; world = &dummyWorld;
    findCalls = 0;
    std::memset(buffer, 0xaa, sizeof buffer);
    packet.cursor = buffer + 16;
    lightSetupForCharacter__FP6PacketG6Point3(&packet, Point3{1, 2, 3});
    expect(saveValid == 0 && findCalls == 0, "saved list used and cleared");
    expect(transformCalls == 1 && mapCalls == 1 && seenMapPoint.x == 1.0f && seenMapPoint.z == 3.0f, "transform and light map once");
    const float d2 = 3 * 3 + 0 + 0;
    const float atten = (100.0f - d2) * 0.01f;
    expect(geth(base, 56) == static_cast<short>(100 * atten * 2.0f) && geth(base, 58) == static_cast<short>(200 * atten * 3.0f) && geth(base, 60) == static_cast<short>(300 * atten * 4.0f) && geth(base, 62) == 0, "quad from halfwords, attenuation and scales");
    expect(near(getf(base, 0), -1.0f) && near(getf(base, 16), 0.0f) && near(getf(base, 32), 0.0f), "direction through the transform stub, z negated");
    expect(getw(base, 4) == 0 && geth(base, 64) == 0, "slot 1 zero");
    expect(geth(base, 80) == 50 && geth(base, 82) == 100, "slot 3 quad scaled by the light map (0.5)");
    if (failures) return 1;
    std::printf("lightSetupForCharacter contract passed\n");
    return 0;
}
