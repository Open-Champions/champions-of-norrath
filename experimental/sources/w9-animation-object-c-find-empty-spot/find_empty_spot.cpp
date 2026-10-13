#include "../w9-animation-object-c-common/mem.hpp"
// Observed objectFindEmptySpot__FG6Point3iiPc at 0x0013f2d0, 1076 bytes. Arguments: a hidden
// result pointer (a 12-byte point is returned through it and the pointer is also returned),
// the centre point (a 12-byte copy), an integer spread, an integer radius and an optional
// byte pointer. Tries up to 50 random candidates around the centre:
//  1. Draws a point in the unit sphere by rejection: three steps of the linear generator
//     state = state * 0x19660d + 0x3c6ef35f on syncRandState (0x483e5c, read at the start of
//     every attempt and written back once a point is accepted), each giving
//     2 * (float)(state >> 16) * 2^-16 - 1. A draw is rejected (all three redrawn from the
//     carried state) when 1.0 < (x*x + y*y) + z*z. The third value only takes part in that test.
//  2. Offset = (x * spread, y * spread, 0). worldTestForbidden__FP12_worldHeaderG6Point3f
//     (the word `world` at 0x4844a0, centre + offset, radius as float) must return exactly
//     zero or the attempt is spent.
//  3. h = worldFindHeight__FP12_worldHeaderG6Point3f(world, centre + offset, radius); the
//     attempt is spent unless h - centre.z < 6.0. Otherwise the candidate result becomes
//     (centre.x + offset.x, centre.y + offset.y, h) - this store happens even if step 4 then fails.
//  4. objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128) with low/high = candidate
//     -/+ (radius, radius, 100000.0) on (x, y, z). Objects whose word at +12 has bit 0 set are
//     ignored; any other object with squared planar distance (x at +16, y at +20) not above
//     (float)(radius * radius) + (float at +28) squared spends the attempt. If none does, the
//     candidate is accepted.
// The result starts as the centre. At the end, when the byte pointer is nonzero, it receives 1
// if fewer than 50 attempts were spent and 0 otherwise, and the result point is written to the
// result pointer, which is returned. Callers in the tree pass spread 36 and radius 24.
// Inferred: "empty spot" - a nearby ground position free of other objects; the fixed 100000
// vertical extent means any height counts.
// Unknown: the units of the radius and what bit 0 of the flag word means.
extern "C" {
extern unsigned syncRandState;
extern unsigned char* world;
float worldTestForbidden__FP12_worldHeaderG6Point3f(unsigned char* world_header, const float* point, float radius);
float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char* world_header, const float* point, float radius);
int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned* out, int limit);
}

namespace {
inline unsigned step(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }
inline float unit(unsigned state) {
    float value = static_cast<float>(state >> 16) * 0x1p-16f;
    value = value + value;
    return value - 1.0f;
}
}

extern "C" unsigned char* objectFindEmptySpot__FG6Point3iiPc(unsigned char* result, const unsigned char* centre_in,
                                                            int spread, int radius_int, char* found) {
    using namespace norrath_aoc;
    float centre[3];
    centre[0] = load_float(centre_in);
    centre[1] = load_float(centre_in + 4);
    centre[2] = load_float(centre_in + 8);
    float best[3] = {centre[0], centre[1], centre[2]};
    const float spread_f = static_cast<float>(spread);
    const float radius = static_cast<float>(radius_int);
    int attempts = 0;
    do {
        unsigned state = syncRandState;
        float x, y, z;
        do {
            const unsigned first = step(state);
            const unsigned second = step(first);
            state = step(second);
            x = unit(first);
            y = unit(second);
            z = unit(state);
        } while (1.0f < (x * x + y * y) + z * z);
        syncRandState = state;
        const float offset_x = x * spread_f;
        const float offset_y = y * spread_f;
        const float offset_z = 0.0f;
        float probe[3];
        probe[0] = offset_x + centre[0];
        probe[1] = offset_y + centre[1];
        probe[2] = centre[2];
        if (worldTestForbidden__FP12_worldHeaderG6Point3f(world, probe, radius) != 0.0f) { ++attempts; continue; }
        float ground[3];
        ground[0] = offset_x + centre[0];
        ground[1] = offset_y + centre[1];
        ground[2] = offset_z + centre[2];
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, ground, radius);
        if (!(height - centre[2] < 6.0f)) { ++attempts; continue; }
        best[0] = centre[0] + offset_x;
        best[1] = centre[1] + offset_y;
        best[2] = height;
        float low[3], high[3];
        low[0] = best[0] - radius; low[1] = best[1] - radius; low[2] = height - 100000.0f;
        high[0] = best[0] + radius; high[1] = best[1] + radius; high[2] = height + 100000.0f;
        unsigned list[128];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
        bool blocked = false;
        for (unsigned i = 0; list[i] != 0; ++i) {
            unsigned char* object = load_pointer(reinterpret_cast<unsigned char*>(&list[i]));
            if ((load_word(object + 12) & 1u) != 0) continue;
            const float dy = load_float(object + 20) - best[1];
            const float dx = load_float(object + 16) - best[0];
            const float edge = load_float(object + 28);
            const float limit = static_cast<float>(radius_int * radius_int) + edge * edge;
            if (dx * dx + dy * dy <= limit) { blocked = true; break; }
        }
        if (!blocked) break;
        ++attempts;
    } while (attempts < 50);
    if (found != nullptr) *found = attempts != 50;
    store_float(result, best[0]);
    store_float(result + 4, best[1]);
    store_float(result + 8, best[2]);
    return result;
}
