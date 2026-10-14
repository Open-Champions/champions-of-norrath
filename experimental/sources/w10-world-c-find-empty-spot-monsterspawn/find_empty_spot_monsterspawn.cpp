// Observed objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc at 0x00269b80, 1088 bytes. A sibling
// of objectFindEmptySpot__FG6Point3iiPc (0x0013f2d0). Arguments: a hidden result pointer
// (a 12-byte point is written through it and the pointer is returned), the centre point (a
// 12-byte copy), four ints (scale, range, angle, radius) and an optional byte pointer.
// Up to 50 attempts; each one:
//  1. shift: eRandState (0x483e58) advances by x * 0x19660d + 0x3c6ef35f; t = (state >> 16) * 2^-16;
//     m = (t * range) * 2 - range; the planar shift is (isin(angle) * m, icos(angle) * m, 0).
//  2. unit point: syncRandState (0x483e5c) is read and three steps of the same generator give
//     u = 2 * (step >> 16) * 2^-16 - 1 for x, y and z; the three draws are redone from the carried
//     state while 1.0 < (x*x + y*y) + z*z; the state is written back once a draw is accepted.
//  3. candidate = ((centre.x + shift.x) + x * scale, (centre.y + shift.y) + y * scale,
//     (centre.z + 0) + 0). It is stored in the result slot before any test, so the result is
//     the last candidate tried, including after a total failure.
//  4. worldTestForbidden(world, candidate, radius) must return exactly zero, else the attempt is spent.
//  5. worldFindHeight(world, candidate, radius) - centre.z must be below 6.0, else the attempt is spent.
//     The height itself is not stored in the candidate (this differs from the sibling).
//  6. objectFindInBox(low, high, list, 128) with low/high = candidate -/+ (radius, radius, 100000.0)
//     (the float at 0x0048209c). Objects whose word at +12 has bit 0 set are ignored; any other
//     object whose squared planar distance ((x at +16, y at +20) to the candidate) is not above
//     (radius + float at +28) squared spends the attempt. If none does, the candidate is accepted.
// When the byte pointer is nonzero it receives 1 if fewer than 50 attempts were spent, else 0.
// Inferred: this places a monster spawn near a point; the angle and range pair describe an
// offset along a direction. The isin/icos argument is read as an integer angle only because
// of the callee names.
// Unknown: the meaning of the scale and range ints beyond their arithmetic; the object layout
// beyond the three words and one float used.
extern "C" {
extern unsigned eRandState;
extern unsigned syncRandState;
extern unsigned char* world;
float isin__Fi(int angle);
float icos__Fi(int angle);
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
struct ListedObject {
    unsigned char before_flags[12];
    unsigned flags;
    float x, y;
    unsigned char gap[4];
    float edge;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(ListedObject, flags) == 12 && __builtin_offsetof(ListedObject, x) == 16 &&
                  __builtin_offsetof(ListedObject, y) == 20 && __builtin_offsetof(ListedObject, edge) == 28,
              "observed object offsets");
#endif

// List entries are 32-bit target addresses. The host check maps them through a hook.
#if defined(NORRATH_HOST_ARENA)
extern "C" const ListedObject* norrath_host_listed_object(unsigned address);
inline const ListedObject* object_at(unsigned address) { return norrath_host_listed_object(address); }
#else
inline const ListedObject* object_at(unsigned address) {
    return reinterpret_cast<const ListedObject*>(static_cast<__UINTPTR_TYPE__>(address));
}
#endif
}  // namespace

extern "C" float* objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(float* result, const float* centre_in, int scale, int range,
                                                                 int angle, int radius_int, char* found) {
    const float centre[3] = {centre_in[0], centre_in[1], centre_in[2]};
    const float scale_f = static_cast<float>(scale);
    const float range_f = static_cast<float>(range);
    const float radius = static_cast<float>(radius_int);
    float spot[3];
    int attempts = 0;
    do {
        const float sine = isin__Fi(angle);
        const float cosine = icos__Fi(angle);
        eRandState = step(eRandState);
        float along = static_cast<float>(eRandState >> 16) * 0x1p-16f;
        along = along * range_f;
        along = along + along;
        along = along - range_f;
        const float shift_x = sine * along;
        const float shift_y = cosine * along;
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
        const float offset_x = x * scale_f;
        const float offset_y = y * scale_f;
        const float base_x = centre[0] + shift_x;
        const float base_y = centre[1] + shift_y;
        const float base_z = centre[2] + 0.0f;
        spot[0] = base_x + offset_x;
        spot[1] = base_y + offset_y;
        spot[2] = base_z + 0.0f;
        if (worldTestForbidden__FP12_worldHeaderG6Point3f(world, spot, radius) != 0.0f) { ++attempts; continue; }
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, spot, radius);
        if (!(height - centre[2] < 6.0f)) { ++attempts; continue; }
        float low[3] = {spot[0] - radius, spot[1] - radius, spot[2] - 100000.0f};
        float high[3] = {spot[0] + radius, spot[1] + radius, spot[2] + 100000.0f};
        unsigned list[128];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
        bool blocked = false;
        for (unsigned i = 0; list[i] != 0; ++i) {
            const ListedObject* object = object_at(list[i]);
            if ((object->flags & 1u) != 0) continue;
            const float dy = object->y - spot[1];
            const float dx = object->x - spot[0];
            const float reach = radius + object->edge;
            if (dx * dx + dy * dy <= reach * reach) { blocked = true; break; }
        }
        if (!blocked) break;
        ++attempts;
    } while (attempts < 50);
    if (found != nullptr) *found = attempts != 50;
    result[0] = spot[0];
    result[1] = spot[1];
    result[2] = spot[2];
    return result;
}
