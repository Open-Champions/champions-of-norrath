// Observed initializeMe__4Line at 0x279470, extent 1124. Computes the per-point
// geometry of a polyline after its points are set. Line words used: +4 point count,
// +8 the "continous" word (zero means an open line, nonzero a closed loop),
// +16 total length (written), +20 the point array. A point is 64 bytes:
//   +0 position, +12 direction, +24 side, +36 blend (three floats each),
//   +48 segment length, +52 distance along the line, +56 corner (written here),
//   +60 slot index (written by the constructor, not touched here).
// Algorithm, with the up vector (0, 0, 1) held in stack memory so the cross
// products keep their multiplies:
//   total = 0. For each point i, next = 0 if i is the last point, else i + 1:
//   - direction = position[next] - position[i], component by component.
//   - If the line is closed or next != 0: length = sqrt((x*x + y*y) + z*z), the
//     direction is scaled by 1.0 / length, segment length = length, distance =
//     the total so far, and the total grows by the length. Otherwise (the last
//     point of an open line) segment length = 0 and distance = the total.
//   - side = cross(direction, up) = (dy*uz - dz*uy, dz*ux - dx*uz, dx*uy - dy*ux),
//     scaled by rsqrt.s(1.0, (x*x + y*y) + z*z).
//   - Open line, i == 0: blend = side. Then, still for an open line and the last
//     point (which for a one-point line is also point 0): blend = side of the
//     point 64 bytes earlier, and nothing else is done for this point (a
//     one-point open line therefore reads before its array). Point 0 of a closed
//     line stops here as well.
//   - Otherwise (i > 0): corner = 1.0 - max(0.0, dot(side, previous side)), with the
//     dot product summed left to right over the previous side's components first;
//     blend = side + previous side, scaled by rsqrt.s(1.0, its squared length).
//   After the loop, for a closed line, point 0 gets the same corner and blend
//   against the last point's side. The routine returns nothing.
// The EE uses sqrt.s, rsqrt.s and max.s instructions directly; the host check
// builds with standard-library stand-ins and compares with tolerances.
#include "../w8-object-list-math-common/object_types.hpp"

struct LinePoint {
    Point3 position;
    Point3 direction;
    Point3 side;
    Point3 blend;
    float segment_length;
    float distance;
    float corner;
    int slot;
};

struct Line {
    unsigned word0;
    int count;
    int continuous;
    unsigned flags;
    float total_length;
    LinePoint* points;
};

#if defined(__mips__)
static_assert(sizeof(LinePoint) == 64 && __builtin_offsetof(LinePoint, side) == 24 && __builtin_offsetof(LinePoint, blend) == 36 &&
                  __builtin_offsetof(LinePoint, corner) == 56 && __builtin_offsetof(LinePoint, slot) == 60, "point layout");
static_assert(__builtin_offsetof(Line, count) == 4 && __builtin_offsetof(Line, continuous) == 8 && __builtin_offsetof(Line, total_length) == 16 &&
                  __builtin_offsetof(Line, points) == 20, "line layout");
#endif

namespace {
inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}

// rsqrt.s fd, fs, ft computes fs / sqrt(ft).
inline float reciprocal_root(float numerator, float value) {
#if defined(__mips__)
    float result;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(numerator), "f"(value));
    return result;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}

inline float maximum(float left, float right) {
#if defined(__mips__)
    float result;
    __asm__("max.s %0, %1, %2" : "=f"(result) : "f"(left), "f"(right));
    return result;
#else
    return left > right ? left : right;
#endif
}

inline float length_squared(const Point3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void normalize_with_rsqrt(Point3& v, float one) {
    const float scale = reciprocal_root(one, length_squared(v));
    v.z *= scale;
    v.x *= scale;
    v.y *= scale;
}
}  // namespace

extern "C" void initializeMe__4Line(Line* line) {
    float up[3] = {0.0f, 0.0f, 1.0f};
    __asm__("" : "+m"(up));  // keep the up vector in memory like the original
    const float one = 1.0f;
    const int count = line->count;
    line->total_length = 0.0f;
    for (int i = 0; i < count; ++i) {
        LinePoint* points = line->points;
        const int next = (i == count - 1) ? 0 : i + 1;
        LinePoint& point = points[i];
        const LinePoint& target = points[next];
        point.direction.x = target.position.x - point.position.x;
        point.direction.y = target.position.y - point.position.y;
        point.direction.z = target.position.z - point.position.z;
        if (line->continuous != 0 || next != 0) {
            Point3& d = point.direction;
            const float length = square_root(length_squared(d));
            const float inverse = one / length;
            d.z *= inverse;
            d.x *= inverse;
            d.y *= inverse;
            point.segment_length = length;
            point.distance = line->total_length;
            line->total_length = line->total_length + length;
        } else {
            point.segment_length = 0.0f;
            point.distance = line->total_length;
        }
        const Point3& d = line->points[i].direction;
        Point3 side;
        side.x = d.y * up[2] - d.z * up[1];
        side.y = d.z * up[0] - d.x * up[2];
        side.z = d.x * up[1] - d.y * up[0];
        line->points[i].side = side;
        normalize_with_rsqrt(line->points[i].side, one);
        LinePoint& current = line->points[i];
        if (i == 0 && line->continuous == 0) current.blend = current.side;
        if (i == line->count - 1 && line->continuous == 0) {
            const LinePoint& before = *reinterpret_cast<const LinePoint*>(reinterpret_cast<const char*>(&current) - sizeof(LinePoint));
            current.blend = before.side;
            continue;
        }
        if (i == 0) continue;
        const LinePoint& previous = *reinterpret_cast<const LinePoint*>(reinterpret_cast<const char*>(&current) - sizeof(LinePoint));
        const float dot = (current.side.x * previous.side.x + current.side.y * previous.side.y) + current.side.z * previous.side.z;
        current.corner = one - maximum(0.0f, dot);
        current.blend.x = current.side.x + previous.side.x;
        current.blend.z = current.side.z + previous.side.z;
        current.blend.y = current.side.y + previous.side.y;
        normalize_with_rsqrt(current.blend, one);
    }
    if (line->continuous != 0) {
        LinePoint& first = line->points[0];
        const LinePoint& before_end = line->points[line->count - 1];
        const float dot = (before_end.side.x * first.side.x + before_end.side.y * first.side.y) + before_end.side.z * first.side.z;
        first.corner = one - maximum(0.0f, dot);
        first.blend.x = first.side.x + before_end.side.x;
        first.blend.z = first.side.z + before_end.side.z;
        first.blend.y = first.side.y + before_end.side.y;
        normalize_with_rsqrt(first.blend, one);
    }
}
