// Observed lightSetupForCharacter__FP6PacketG6Point3 at 0x0013d970, extent 1240.
// Arguments: a Packet (its first word is the output cursor) and a Point3 (arrives as a
// pointer to a caller copy, copied to the stack first). Returns nothing.
//
// Light words used: halfwords at +10, +12, +14; floats at +28 (radius), +32, +48, +52, +56
// (position). Other data used: saveValid (0x00483ab8), saveBestList (0x004fbf80, first two
// words), charAmbient, charDirect and charDirect2 (four halfwords each, at 0x00486850,
// 0x00486858, 0x00486860), charScaleR, charScaleG and charScaleB (floats at 0x00486868,
// 0x0048686c, 0x00486870), charDirectDir and charDirectDir2 (three floats each, at
// 0x004f40d0 and 0x004f40e0), worldCameraMatrix (0x0037ef00), world, interfaceMode and
// disableVolumetricLighting.
//
// Step 1, choosing at most two lights: if saveValid is non-zero it is cleared and the two
// lights are saveBestList[0] and [1]. Otherwise findLightsAtPoint(point, list, 32) fills a
// list. For each light up to the first null, key = ((int)(|point - pos|^2 - radius^2) << 7)
// | index, with squares summed (dx*dx + dy*dy) + dz*dz. The two smallest signed keys are kept
// by insertion, both starting at 0x1000001f, list[31] is set to null (after the search,
// whether or not the list was empty) and the two lights are list[key & 31].
// Step 2, the packet: *cursor = 0x6c03c000; from base = cursor + 4: halfwords at base+88..94
// are charAmbient, base+48 = 0x14000010, base+52 = 0x6d058000, base+96 = 0x14000024, and the
// cursor becomes base + 100. quads = base + 56 (four halfwords per slot, slot 0..3 at 8-byte
// steps); float rows at base + 4 * slot, + 16 and + 32.
// Step 3, for each chosen light up to the first null and at most two (slot = its count):
// d = pos - point; dist2 = (dx*dx + dy*dy) + dz*dz; inv = 1.0f / sqrt(dist2) by the hardware
// rsqrt.s; atten = min(max((radius^2 - dist2) * (float at +32), 0.0f), 1.0f) with the hardware
// max.s and min.s. Quad: [0] = (int)(((float)halfword10 * atten) * charScaleR), [1] likewise
// with halfword12 and charScaleG, [2] with halfword14 and charScaleB, [3] = 0 (all stored
// as halfwords). d * inv is passed through matTransform34Vec(d, worldCameraMatrix, d) and
// float rows are x, y and -z of the result. Unused slots below two get zero quads and zero
// rows (the float rows are zero words, not 1.0f).
// Step 4: slot 2 quad = charDirect2 halfwords and float rows = charDirectDir2; slot 3 float
// rows = charDirectDir. scale = worldQueryLightMap(world, point) when world is non-null and
// interfaceMode and disableVolumetricLighting are both zero, otherwise 1.0f; slot 3 quad =
// (int)((float)charDirect[i] * scale) for i = 0..3 (base+80 to base+86).
// Inferred: this builds the character lighting packet: up to two point lights then two
// fixed directional slots. Unknown: what the tag words mean, the unit of the quads, and
// who writes saveValid and the char* data besides lightCharacterShadow.
struct Point3 {
    float x, y, z;
};

struct CharLightPacket {
    unsigned char* cursor;
    unsigned char* last;
};

extern "C" {
void findLightsAtPoint__FG6Point3PP5Lighti(Point3, const unsigned char**, int);
Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3*, const float*, const Point3*);
float worldQueryLightMap__FP12_worldHeaderG6Point3(void*, Point3);
extern int saveValid;
extern const unsigned char* saveBestList[];
extern short charAmbient[4];
extern short charDirect[4];
extern unsigned short charDirect2[4];
extern float charScaleR;
extern float charScaleG;
extern float charScaleB;
extern float charDirectDir[3];
extern float charDirectDir2[3];
extern float worldCameraMatrix[12];
extern void* world;
extern int interfaceMode;
extern int disableVolumetricLighting;
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
inline short to_short(float value) { return static_cast<short>(static_cast<int>(value)); }
inline void keep_smaller(int& slot, int& carry) {
    if (!(slot < carry)) {
        const int t = slot;
        slot = carry;
        carry = t;
    }
}
}

extern "C" void lightSetupForCharacter__FP6PacketG6Point3(CharLightPacket* packet, Point3 position) {
    const Point3 point = position;
    const unsigned char* chosen[2];
    if (saveValid != 0) {
        saveValid = 0;
        chosen[0] = saveBestList[0];
        chosen[1] = saveBestList[1];
    } else {
        const unsigned char* list[32];
        findLightsAtPoint__FG6Point3PP5Lighti(point, list, 32);
        int best = 0x1000001f;
        int second = best;
        if (list[0] != nullptr) {
            int index = 0;
            while (true) {
                const unsigned char* light = list[index];
                const unsigned char* next = list[index + 1];
                const float radiusSquared = fl(light, 28) * fl(light, 28);
                const float dx = point.x - fl(light, 48);
                const float dy = point.y - fl(light, 52);
                const float dz = point.z - fl(light, 56);
                const float value = ((dx * dx + dy * dy) + dz * dz) - radiusSquared;
                const int key = static_cast<int>(static_cast<unsigned>(static_cast<int>(value)) << 7) | index;
                ++index;
                int carry = key;
                keep_smaller(best, carry);
                keep_smaller(second, carry);
                if (next == nullptr) break;
            }
        }
        list[31] = nullptr;
        chosen[0] = list[best & 0x1f];
        chosen[1] = list[second & 0x1f];
    }

    unsigned char* cursor = packet->cursor;
    *reinterpret_cast<unsigned*>(cursor) = 0x6c03c000u;
    unsigned char* base = cursor + 4;
    *reinterpret_cast<short*>(base + 88) = charAmbient[0];
    *reinterpret_cast<unsigned*>(base + 48) = 0x14000010u;
    *reinterpret_cast<short*>(base + 90) = charAmbient[1];
    *reinterpret_cast<short*>(base + 92) = charAmbient[2];
    packet->cursor = base + 100;
    *reinterpret_cast<unsigned*>(base + 52) = 0x6d058000u;
    *reinterpret_cast<unsigned*>(base + 96) = 0x14000024u;
    *reinterpret_cast<short*>(base + 94) = charAmbient[3];
    unsigned char* quads = base + 56;

    int count = 0;
    while (count < 2 && chosen[count] != nullptr) {
        const unsigned char* light = chosen[count];
        short* quad = reinterpret_cast<short*>(quads + count * 8);
        Point3 direction;
        direction.x = fl(light, 48) - point.x;
        direction.y = fl(light, 52) - point.y;
        direction.z = fl(light, 56) - point.z;
        const float distance = (direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z;
        const float radiusSquared = fl(light, 28) * fl(light, 28);
        const float inverse = reciprocal_sqrt(distance);
        float atten = (radiusSquared - distance) * fl(light, 32);
        atten = max_s(atten, 0.0f);
        atten = min_s(atten, 1.0f);
        quad[0] = to_short((static_cast<float>(shw(light, 10)) * atten) * charScaleR);
        direction.x = direction.x * inverse;
        quad[1] = to_short((static_cast<float>(shw(light, 12)) * atten) * charScaleG);
        direction.y = direction.y * inverse;
        direction.z = direction.z * inverse;
        quad[3] = 0;
        quad[2] = to_short((static_cast<float>(shw(light, 14)) * atten) * charScaleB);
        matTransform34Vec__FP6Point3P8Matrix34T0(&direction, worldCameraMatrix, &direction);
        float* row = reinterpret_cast<float*>(base + count * 4);
        row[0] = direction.x;
        row[4] = direction.y;
        row[8] = -direction.z;
        ++count;
    }
    for (int slot = count; slot < 2; ++slot) {
        short* quad = reinterpret_cast<short*>(quads + slot * 8);
        unsigned* row = reinterpret_cast<unsigned*>(base + slot * 4);
        quad[0] = 0;
        quad[1] = 0;
        quad[2] = 0;
        quad[3] = 0;
        row[0] = 0;
        row[4] = 0;
        row[8] = 0;
    }

    unsigned short* fixed = reinterpret_cast<unsigned short*>(quads + 16);
    fixed[0] = charDirect2[0];
    reinterpret_cast<float*>(base)[2] = charDirectDir2[0];
    fixed[1] = charDirect2[1];
    reinterpret_cast<float*>(base)[6] = charDirectDir2[1];
    fixed[2] = charDirect2[2];
    reinterpret_cast<float*>(base)[10] = charDirectDir2[2];
    fixed[3] = charDirect2[3];
    reinterpret_cast<float*>(base)[3] = charDirectDir[0];
    reinterpret_cast<float*>(base)[7] = charDirectDir[1];
    reinterpret_cast<float*>(base)[11] = charDirectDir[2];

    float scale;
    if (world != nullptr && interfaceMode == 0 && disableVolumetricLighting == 0) {
        scale = worldQueryLightMap__FP12_worldHeaderG6Point3(world, point);
    } else {
        scale = 1.0f;
    }
    short* scaled = reinterpret_cast<short*>(quads + 24);
    scaled[0] = to_short(static_cast<float>(charDirect[0]) * scale);
    scaled[1] = to_short(static_cast<float>(charDirect[1]) * scale);
    scaled[2] = to_short(static_cast<float>(charDirect[2]) * scale);
    scaled[3] = to_short(static_cast<float>(charDirect[3]) * scale);
}
