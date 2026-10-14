// Observed worldFindHeight__FP12_worldHeaderG6Point3f at 0x00176378, 840 bytes.
// Arguments: the world header, a Point3 (passed by address) and a float radius.
// Returns, as a float, the highest ground height found near the point's x and y.
//
// Steps:
//  1. The global int collisionFrame is incremented.
//  2. makeCollisionList__FP12_worldHeaderffffPP13collisionParti is called with
//     (world, x - r, y - r, x + r, y + r, list, 64); the list is a zero-terminated
//     array of part pointers.
//  3. n = (int) ceilf(r * 0.25) and limit = (r * 0.25)^2. The best height starts at
//     -1000000 (0xfff0bdc0).
//  4. For each part (offsets in bytes): the grid pointer is at 12 and a height offset
//     word z at 28. When best >= grid.s16[18] + z the part is skipped. Otherwise the
//     point is moved into the part's frame: dx = x - (float) word[20], dy = y - (float) word[24],
//     c = s16[32] * K and s = s16[34] * K with K the float 0x38000100 (about 1/32767),
//     and when bit 0 of u16[16] is set x' = dx*c + dy*s, y' = dx*s - dy*c, else
//     x' = dx*c + dy*s, y' = (-dx)*s + dy*c. Both are scaled by 0.25 and floored to
//     cell indices gx, gy (floorf).
//  5. The grid holds words origin_x at 0, origin_y at 4, width at 8, rows at 12 and u16
//     cells from byte 20, row-major. Cells with x in [gx-n, gx+n+1] and y in
//     [gy-n, gy+n+1] that lie inside the grid are visited, row by row. A cell value is
//     (s16 value >> 4) + z. When that is greater than best and the squared distance
//     from (x', y') (scaled) to the cell index is below limit, best takes it, and if
//     best >= grid.s16[18] + z the rest of the part is abandoned.
//  6. The result is (float) best.
// Inferred: a terrain/collision heightfield lookup. Unknown: the struct names and what
// bit 0 of the flag halfword selects (a mirrored orientation).
// Hosts: with NORRATH_HOST_HEIGHT the stored 32-bit grid pointers are offsets into an arena.
#if defined(NORRATH_HOST_HEIGHT)
extern unsigned char* norrath_height_arena;
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
#if defined(NORRATH_HOST_HEIGHT)
    return norrath_height_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius) {
    unsigned char* list[65];
    const float px = point[0];
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
