// Observed move__10ProjectileG6Point3 at 0x00144cb8, 1072 bytes. Arguments: the projectile O, a vector V (by
// value) and, as the result, a flag word (bit 0: the move hit the ground, bit 1: it touched at least one object).
// Floats at O+N are written fN; positions are (f16, f20, f24); f28 is a radius and f32 a height (these
// readings come only from how the values are combined below). Steps in order:
//  1. V is scaled componentwise by the float k0 (the image reads it from its global pool at 0x0047fcf0; the stored
//     bits are 0x3c888888, 1/60 rounded, and no symbol names it, so the bits are written here) and added to the
//     position; objectUpdateInGrid__FP10GameObject(O) follows.
//  2. h = worldFindHeight__FP12_worldHeaderG6Point3f(world, position, f28). When f24 < h the ground was hit:
//     the result gets bit 0 and a step-back search runs. A step t starts at k1 (pool 0x0047fcf4, bits 0x3dcccccc,
//     0.1 rounded) and grows by k1 after each test while it stays below 2.0; for each t the candidate C =
//     position - V * t (V already scaled) is looked up the same way, and the search stops at the first C whose
//     height h satisfies h <= C.z, or when t has reached 2.0. The position becomes the last C tried.
//  3. When the byte at +80 is zero and worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, position, 12.0, 0)
//     reports water: the word at +68 advances by the 32-bit step state * 0x19660d + 0x3c6ef35f (its result is
//     unused), the byte at +80 is set to one, a name is formed with sprintf(buffer, "foly_wtrsplash_sml%d.anm", 1),
//     lumpFindResource__FPcT0("fx.lmp", name) is called and soundPlaySequence__FP15AnimationHeaderP10GameObjectf
//     plays it on O with the float k2 (pool 0x0047fcf8, bits 0x3e4ccccc, 0.2 rounded); finally
//     worldPerturbWater__FP12_worldHeaderG6Point3ff(world, position, 700.0, 24.0) is called.
//  4. A box query: low = (f16 - f28, f20 - f28, f24) and high = (f16 + f28, f20 + f28, f24 + 12.0) go to
//     objectFindInBox__FG6Point3T0PP10GameObjecti with a 256-entry output array. Let owner be the word at +76 and
//     skip the word at owner+12 (zero when owner is null). Each listed object X is ignored when it is O, when it is
//     the skip object, when its flag word at +12 has bit 0x1 or 0x200, when X.f24 + X.f32 < f24, when
//     f24 + f32 < X.f24 (using O's own f32), or when the horizontal distance squared between the positions is not
//     below (f28 + X.f28) squared. Otherwise the result gets bit 1, then slot byte 48 of X's table (word at +72) is
//     called on X with the argument O, and slot byte 48 of O's table on O with the argument X (each slot entry is a
//     signed halfword adjusting the object and a function word at +4). The list ends at its first null entry.
// The step sum is kept a run-time float addition (the reviewed compiler would otherwise turn the loop into an
// integer trip count folded with its own float rounding).
// Inferred: the vector is a displacement and the checks decide ground and object collisions; slot byte 48 is the
// collision message (the VehicleBodySphere and OniBall collision functions in this repository are such slots, but
// that is not shown for the slot index itself). Unknown: what the three pool floats stand for (k0 equals the
// 1/60 used as a frame step elsewhere) and the meaning of the fields. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern void* world;
void objectUpdateInGrid__FP10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, Point3 position, float radius, float* result);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, Point3 position, float a, float b);
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
int sprintf(char* buffer, const char* format, ...);

int move__10ProjectileG6Point3(void* self, Point3 v) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    int result = 0;
    const float k0 = bits(0x3c888888u);
    v.x = v.x * k0;
    v.y = v.y * k0;
    v.z = v.z * k0;
    position->x = position->x + v.x;
    position->y = position->y + v.y;
    position->z = position->z + v.z;
    objectUpdateInGrid__FP10GameObject(o);
    {
        Point3 probe = *position;
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, real(o, 28));
        if (real(o, 24) < height) {
            result = 1;
            float step = bits(0x3dccccccu);
            const float increment = step;
            Point3 candidate;
            Point3 scaled;
            do {
                scaled.x = v.x * step;
                scaled.y = v.y * step;
                scaled.z = v.z * step;
                candidate.x = position->x - scaled.x;
                candidate.y = position->y - scaled.y;
                candidate.z = position->z - scaled.z;
                const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, candidate, real(o, 28));
                if (h <= candidate.z) break;
                step = step + increment;
#ifdef __mips__
                __asm__("" : "+f"(step));   // keep the sum a run-time float add: the compiler would otherwise fold the trip count
#endif
            } while (step < 2.0f);
            *position = candidate;
            if (static_cast<signed char>(o[80]) == 0) {
                Point3 probe2 = *position;
                if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, probe2, 12.0f, nullptr) != 0) {
                    word(o, 68) = word(o, 68) * 0x19660du + 0x3c6ef35fu;
                    o[80] = 1;
                    char name[80];
                    sprintf(name, "foly_wtrsplash_sml%d.anm", 1);
                    void* const sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name);
                    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sequence, o, bits(0x3e4cccccu));
                    Point3 probe3 = *position;
                    worldPerturbWater__FP12_worldHeaderG6Point3ff(world, probe3, 700.0f, 24.0f);
                }
            }
        }
    }
    const float z = real(o, 24);
    const float radius = real(o, 28);
    Point3 low;
    Point3 high;
    low.x = real(o, 16) - radius;
    low.y = real(o, 20) - radius;
    low.z = z;
    high.x = real(o, 16) + radius;
    high.y = real(o, 20) + radius;
    high.z = z + 12.0f;
    void* skip = nullptr;
    if (word(o, 76) != 0u) skip = from_word(word(from_word(word(o, 76)), 12));
    void* list[256];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 256);
    for (int i = 0; list[i] != nullptr; ++i) {
        unsigned char* const x = static_cast<unsigned char*>(list[i]);
        if ((word(x, 12) & 0x201u) != 0u) continue;
        if (x == o) continue;
        if (x == skip) continue;
        const float xz = real(x, 24);
        if (xz + real(x, 32) < real(o, 24)) continue;
        if (real(o, 24) + real(o, 32) < xz) continue;
        const float dx = real(o, 16) - real(x, 16);
        const float dy = real(o, 20) - real(x, 20);
        const float reach = real(o, 28) + real(x, 28);
        if (!(dx * dx + dy * dy < reach * reach)) continue;
        result |= 2;
        call_entry(x, 48, to_word(o));
        call_entry(o, 48, to_word(x));
    }
    return result;
}
}
