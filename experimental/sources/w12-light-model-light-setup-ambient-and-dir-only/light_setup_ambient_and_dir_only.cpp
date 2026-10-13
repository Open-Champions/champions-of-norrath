// Observed lightSetupAmbientAndDirOnly__FP6PacketG6Point3f at 0x0013df98, extent 456. Arguments: a
// Packet (first word = output cursor; the second word is not touched), a Point3 (by address
// of a caller copy) and a float scale. Returns nothing.
// Same packet shape as the one lightSetupForCharacter builds, with no point lights: with
// base = cursor + 4, *cursor = 0x6c03c000, base+48 = 0x14000010, base+52 = 0x6d058000,
// base+96 = 0x14000024, halfwords base+88..94 = charAmbient (0x00486850), and the cursor becomes
// base + 100. Slots 0 and 1 have zero quads (four halfwords each at base+56 and base+64) and
// zero float words at base + 4k, base + 16 + 4k and base + 32 + 4k for k = 0 and 1.
// Slot 2: quad at base+72 = charDirect2 (four unsigned halfwords, 0x00486860); float rows
// base+8, +24, +40 = charDirectDir2[0..2] (0x004f40e0).
// Slot 3: float rows base+12, +28, +44 = the floats at scene+12, +16, +20 (scene, 0x004f4060);
// quad at base+80: s = worldQueryLightMap(world, point) * scale, then each of the four signed
// halfwords of charDirect (0x00486858) is converted to float, multiplied by s, truncated to int
// and stored as a halfword. worldQueryLightMap is called every time, with no check of world.
// Unknown: what the tag words and slot numbering mean beyond the shared packet shape, and who
// supplies scale.
struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};
struct Point3 {
    float x, y, z;
};
typedef unsigned __attribute__((may_alias)) u32;
typedef short __attribute__((may_alias)) i16;
typedef float __attribute__((may_alias)) f32;
extern "C" {
extern short charAmbient[4];
extern short charDirect[4];
extern unsigned short charDirect2[4];
extern float charDirectDir2[3];
extern unsigned char scene[];
extern void* world;
float worldQueryLightMap__FP12_worldHeaderG6Point3(void* world, Point3 point);
void lightSetupAmbientAndDirOnly__FP6PacketG6Point3f(Packet* packet, Point3 position, float scale) {
    const Point3 point = position;
    unsigned char* start = packet->cursor;
    *reinterpret_cast<u32*>(start) = 0x6c03c000u;
    unsigned char* base = start + 4;
    *reinterpret_cast<i16*>(base + 88) = charAmbient[0];
    packet->cursor = base + 100;
    *reinterpret_cast<i16*>(base + 90) = charAmbient[1];
    *reinterpret_cast<i16*>(base + 92) = charAmbient[2];
    *reinterpret_cast<i16*>(base + 94) = charAmbient[3];
    *reinterpret_cast<u32*>(base + 96) = 0x14000024u;
    *reinterpret_cast<u32*>(base + 48) = 0x14000010u;
    *reinterpret_cast<u32*>(base + 52) = 0x6d058000u;
    for (int k = 0; k < 2; ++k) {
        i16* quad = reinterpret_cast<i16*>(base + 56 + 8 * k);
        quad[0] = 0;
        quad[1] = 0;
        quad[2] = 0;
        quad[3] = 0;
        *reinterpret_cast<u32*>(base + 4 * k) = 0;
        *reinterpret_cast<u32*>(base + 16 + 4 * k) = 0;
        *reinterpret_cast<u32*>(base + 32 + 4 * k) = 0;
    }
    i16* quad2 = reinterpret_cast<i16*>(base + 72);
    quad2[0] = static_cast<short>(charDirect2[0]);
    *reinterpret_cast<f32*>(base + 8) = charDirectDir2[0];
    quad2[1] = static_cast<short>(charDirect2[1]);
    *reinterpret_cast<f32*>(base + 24) = charDirectDir2[1];
    quad2[2] = static_cast<short>(charDirect2[2]);
    *reinterpret_cast<f32*>(base + 40) = charDirectDir2[2];
    quad2[3] = static_cast<short>(charDirect2[3]);
    *reinterpret_cast<f32*>(base + 12) = *reinterpret_cast<const f32*>(scene + 12);
    *reinterpret_cast<f32*>(base + 28) = *reinterpret_cast<const f32*>(scene + 16);
    *reinterpret_cast<f32*>(base + 44) = *reinterpret_cast<const f32*>(scene + 20);
    const float s = worldQueryLightMap__FP12_worldHeaderG6Point3(world, point) * scale;
    i16* quad3 = reinterpret_cast<i16*>(base + 80);
    for (int i = 0; i < 4; ++i) quad3[i] = static_cast<short>(static_cast<int>(static_cast<float>(charDirect[i]) * s));
}
}
