// Observed worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii at
// 0x0017bf90, 980 bytes. Casts a ray through the world and through the objects near its path.
// Arguments: world, start and direction (Point3 values passed by reference; the image copies both),
// a distance limit `limit` (a multiple of the direction vector), an object to ignore, an optional
// result slot for the object hit, an integer passed on to worldHitscan, and an integer flag
// `allow`. The direction's length is not 1; distances are fractions of the direction vector.
//   1. limit <= 0 (also NaN is not <= 0, so it continues): return -1.0.
//   2. w = worldHitscan(world, start, direction, flags). When a slot was given it is cleared.
//   3. len = sqrt(dx * dx + dy * dy) of the direction. When len < 0.01 the result is -1.0.
//   4. inverse = 1 / len. u = (dx, dy, dz) * inverse (a planar unit vector, z carried along); the
//      direction copy is scaled by 240 * inverse, so one step moves 240 planar units. When w is not
//      below zero (a wall hit), limit becomes w.
//   5. Steps = (int)(limit * len * (1/240 as a float constant)) + 1, run while positive. A cursor
//      starts at the start point; each step builds the box between the cursor and cursor + step,
//      widened in z to -10000..10000, asks objectFindInBox for up to 256 objects (null terminated),
//      and for each object o in order:
//        - o is skipped when its flag word (+12) has bit 0x1 or 0x4 set, unless `allow` is nonzero
//          and bit 0x40000 is set; and when it is the ignored object;
//        - radius = 60.0 when bit 0x40000 is set, otherwise the float at o+28;
//        - with d = o.xy - start.xy: perp = |u.y * d.x - u.x * d.y|; skipped when radius < perp;
//        - along = u.x * d.x + u.y * d.y; entry = along - sqrt(radius^2 - perp^2); skipped when
//          entry < 0; the candidate distance is entry * inverse; it replaces `limit` and records o
//          as the nearest hit when it is smaller than the current `limit`.
//      the cursor then advances to the end of the box (cursor + step).
//   6. The slot (when given) receives the nearest object or null. The result is `limit` when w was
//      not negative or an object was hit, and -1.0 otherwise.
// Inferred: the 10000 values only widen the box to every height, and the 60.0 radius is a
// stand-in for objects without their own radius. Unknown: the exact meaning of the flag bits
// 0x1, 0x4 and 0x40000 and of `allow`. The image keeps the cursor and box points in 16-byte
// spill slots (quad moves); this has no effect on behaviour. Nothing here ran on the EE.
extern "C" {
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, const float* start, const float* direction, int flags);
void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, void** list, int capacity);
}

namespace {
// The reviewed compiler truncates decimal float literals, so the step constant is given by its bits.
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
}  // namespace

extern "C" float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
    void* world, const float* start_point, const float* direction_point, float limit, void* ignore, void** hit,
    int flags, int allow) {
    float start[3] = {start_point[0], start_point[1], start_point[2]};
    float direction[3] = {direction_point[0], direction_point[1], direction_point[2]};
    if (limit <= 0.0f) return -1.0f;
    float wall = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, start, direction, flags);
    if (hit != nullptr) *hit = nullptr;
    float length = __builtin_sqrtf(direction[0] * direction[0] + direction[1] * direction[1]);
    if (length < 0.01f) return -1.0f;
    float inverse = 1.0f / length;
    float unit_x = direction[0] * inverse;
    float unit_y = direction[1] * inverse;
    float scale = inverse * 240.0f;
    direction[0] *= scale;
    direction[1] *= scale;
    direction[2] *= scale;
    float cursor[3] = {start[0], start[1], start[2]};
    if (!(wall < 0.0f)) limit = wall;
    int steps = static_cast<int>(limit * length * constant(0x3b888888u)) + 1;
    void* nearest = nullptr;
    for (; steps > 0; --steps) {
        float end[3] = {cursor[0] + direction[0], cursor[1] + direction[1], cursor[2] + direction[2]};
        float low[3] = {end[0] < cursor[0] ? end[0] : cursor[0], end[1] < cursor[1] ? end[1] : cursor[1], -10000.0f};
        float high[3] = {end[0] > cursor[0] ? end[0] : cursor[0], end[1] > cursor[1] ? end[1] : cursor[1], 10000.0f};
        void* list[256];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 256);
        for (int i = 0; list[i] != nullptr; ++i) {
            unsigned char* object = static_cast<unsigned char*>(list[i]);
            unsigned object_flags = *reinterpret_cast<unsigned*>(object + 12);
            if ((object_flags & 5) != 0 && (allow == 0 || (object_flags & 0x40000) == 0)) continue;
            if (object == ignore) continue;
            float radius = (object_flags & 0x40000) != 0 ? 60.0f : *reinterpret_cast<float*>(object + 28);
            float dx = *reinterpret_cast<float*>(object + 16) - start[0];
            float dy = *reinterpret_cast<float*>(object + 20) - start[1];
            float cross = unit_y * dx - unit_x * dy;
            float perp = cross < 0.0f ? -cross : cross;
            if (radius < perp) continue;
            float entry = (unit_x * dx + unit_y * dy) - __builtin_sqrtf(radius * radius - perp * perp);
            if (entry < 0.0f) continue;
            float candidate = entry * inverse;
            if (!(candidate < limit)) continue;
            limit = candidate;
            nearest = object;
        }
        cursor[0] = end[0];
        cursor[1] = end[1];
        cursor[2] = end[2];
    }
    if (hit != nullptr) *hit = nearest;
    if (wall < 0.0f && nearest == nullptr) return -1.0f;
    return limit;
}
