// Observed worldFindStandHeight__FP12_worldHeaderG6Point3f at 0x00175fe0, 920 bytes.
// Same traversal, arguments and result as worldFindHeight__FP12_worldHeaderG6Point3f at
// 0x00176378 (see that file for the steps), with two extra rejections that keep the
// result at or below the point's height plus 12.0 (z is the third float of the point):
//  - a part is skipped (after the best >= grid.s16[18] + z test) when
//    z + 12.0 < (float) (grid.s16[16] + word[28]);
//  - a cell candidate is skipped (after the candidate > best test, before the distance
//    test) when z + 12.0 < (float) candidate.
// The scale float is read from a second copy of the same constant (0x38000100).
// Inferred: finds ground a creature can stand on, ignoring ledges above head height.
// Unknown: the exact meaning of grid.s16[16] (lowest height in the part, by use).
// Hosts: with NORRATH_HOST_STAND the stored 32-bit grid pointers are offsets into an arena.
#if defined(NORRATH_HOST_STAND)
extern unsigned char* norrath_stand_arena;
#endif
extern int collisionFrame;
extern "C" {
float ceilf(float);
float floorf(float);
void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(void* world, float x0, float y0, float x1, float y1,
                                                                unsigned char** list, int limit);
}

namespace {
inline int word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const int*>(base + offset); }
inline short half(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const short*>(base + offset); }
inline unsigned char* grid_of(const unsigned char* part) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(part + 12);
#if defined(NORRATH_HOST_STAND)
    return norrath_stand_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius) {
    unsigned char* list[65];
    const float px = point[0];
    const float pz = point[2];
    const float py = point[1];
    collisionFrame = collisionFrame + 1;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(world, px - radius, py - radius, px + radius, py + radius, list, 64);
    const float quarter = radius * 0.25f;
    const float limit = quarter * quarter;
    const int reach = static_cast<int>(ceilf(quarter));
    int best = -1000000;
    const float scale = __builtin_bit_cast(float, 0x38000100u);
    for (int i = 0; list[i] != nullptr; ++i) {
        const unsigned char* part = list[i];
        const unsigned char* grid = grid_of(part);
        const int z = word(part, 28);
        if (!(best < half(grid, 18) + z)) continue;
        if (pz + 12.0f < static_cast<float>(half(grid, 16) + z)) continue;
        const float dx = px - static_cast<float>(word(part, 20));
        const float dy = py - static_cast<float>(word(part, 24));
        const float c = static_cast<float>(half(part, 32)) * scale;
        const float s = static_cast<float>(half(part, 34)) * scale;
        float rx, ry;
        if (*reinterpret_cast<const unsigned short*>(part + 16) & 1u) {
            rx = dx * c + dy * s;
            ry = dx * s - dy * c;
        } else {
            rx = dx * c + dy * s;
            ry = (-dx) * s + dy * c;
        }
        rx = rx * 0.25f;
        ry = ry * 0.25f;
        const int gx = static_cast<int>(floorf(rx));
        const int gy = static_cast<int>(floorf(ry));
        const int origin_x = word(grid, 0);
        const int origin_y = word(grid, 4);
        const int width = word(grid, 8);
        const int xmin = gx - reach;
        const int xmax = gx + reach + 1;
        const int ymax = gy + reach + 1;
        bool done = false;
        for (int y = gy - reach; y <= ymax && !done; ++y) {
            const int row = y - origin_y;
            if (row < 0 || row >= word(grid, 12)) continue;
            const unsigned short* cell = reinterpret_cast<const unsigned short*>(grid + 20) + (row * width + (xmin - origin_x));
            for (int x = xmin; x <= xmax; ++x, ++cell) {
                const int column = x - origin_x;
                if (column < 0 || column >= width) continue;
                const int candidate = (static_cast<int>(static_cast<short>(*cell)) >> 4) + z;
                if (!(best < candidate)) continue;
                if (pz + 12.0f < static_cast<float>(candidate)) continue;
                const float fx = static_cast<float>(x) - rx;
                const float fy = static_cast<float>(y) - ry;
                if (fx * fx + fy * fy < limit) {
                    best = candidate;
                    if (!(best < half(grid, 18) + z)) { done = true; break; }
                }
            }
        }
    }
    return static_cast<float>(best);
}
