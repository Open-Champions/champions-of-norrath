// Host contract for lightSetupAmbientAndDirOnly__FP6PacketG6Point3f. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* last; };
struct Point3 { float x, y, z; };
extern "C" {
short charAmbient[4], charDirect[4];
unsigned short charDirect2[4];
float charDirectDir2[3];
unsigned char scene[0x80];
void* world;
static int mapCalls; static void* seenWorld; static Point3 seenPoint; static float mapValue = 0.5f;
float worldQueryLightMap__FP12_worldHeaderG6Point3(void* w, Point3 p) { ++mapCalls; seenWorld = w; seenPoint = p; return mapValue; }
void lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(Packet*, Point3, float);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    for (int i = 0; i < 4; ++i) { charAmbient[i] = static_cast<short>(10 + i); charDirect[i] = static_cast<short>(100 * (i + 1) - 250); charDirect2[i] = static_cast<unsigned short>(40000 + i); }
    for (int i = 0; i < 3; ++i) charDirectDir2[i] = -2.5f - i;
    float sc[3] = {7.5f, 8.5f, 9.5f};
    std::memcpy(scene + 12, sc, 12);
    static unsigned char marker; world = &marker;
    alignas(16) unsigned char out[512];
    std::memset(out, 0xa5, sizeof out);
    Packet packet{out + 16, out + 8};
    lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(&packet, {1, 2, 3}, 2.0f);
    const unsigned char* b = out + 20;
    auto w32 = [&](int off) { unsigned v; std::memcpy(&v, b + off, 4); return v; };
    auto f32 = [&](int off) { float v; std::memcpy(&v, b + off, 4); return v; };
    auto h16 = [&](int off) { short v; std::memcpy(&v, b + off, 2); return v; };
    unsigned tag; std::memcpy(&tag, out + 16, 4);
    expect(tag == 0x6c03c000u, "tag");
    expect(packet.cursor == b + 100 && packet.last == out + 8, "cursor and last");
    expect(w32(48) == 0x14000010u && w32(52) == 0x6d058000u && w32(96) == 0x14000024u, "fixed words");
    for (int i = 0; i < 4; ++i) expect(h16(88 + 2 * i) == charAmbient[i], "ambient");
    for (int k = 0; k < 2; ++k) {
        expect(f32(4 * k) == 0 && f32(16 + 4 * k) == 0 && f32(32 + 4 * k) == 0, "zero rows");
        for (int i = 0; i < 4; ++i) expect(h16(56 + 8 * k + 2 * i) == 0, "zero quads");
    }
    for (int i = 0; i < 4; ++i) expect(static_cast<unsigned short>(h16(72 + 2 * i)) == charDirect2[i], "slot 2 quad");
    expect(f32(8) == -2.5f && f32(24) == -3.5f && f32(40) == -4.5f, "slot 2 rows");
    expect(f32(12) == 7.5f && f32(28) == 8.5f && f32(44) == 9.5f, "slot 3 rows");
    expect(mapCalls == 1 && seenWorld == world && seenPoint.x == 1 && seenPoint.y == 2 && seenPoint.z == 3, "map query");
    // scale = 0.5 * 2.0 = 1.0
    for (int i = 0; i < 4; ++i) expect(h16(80 + 2 * i) == charDirect[i], "slot 3 quad at scale 1");
    mapValue = 0.25f;
    lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(&packet, {1, 2, 3}, 3.0f);   // 0.75
    b = out + 16 + 104 + 4;
    for (int i = 0; i < 4; ++i) expect(h16(80 + 2 * i) == static_cast<short>(static_cast<int>(charDirect[i] * 0.75f)), "slot 3 quad at scale 0.75");
    world = nullptr;
    lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(&packet, {1, 2, 3}, 1.0f);
    expect(mapCalls == 3 && seenWorld == nullptr, "queried even when world is null");
    if (failures) return 1;
    std::printf("lightSetupAmbientAndDirOnly contract passed\n");
    return 0;
}
