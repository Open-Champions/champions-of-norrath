// Observed worldTestForbidden__FP12_worldHeaderG6Point3f at 0x001766c8, 788 bytes.
// Arguments: the world header, a Point3 (passed by address) and a float radius. Returns a
// float: 1.0 when a marked cell is found near the point, otherwise 0.0.
//
// Steps: the int collisionFrame is incremented; makeCollisionList__FP12_worldHeaderffffPP13collisionParti
// is called with (world, x - r, y - r, x + r, y + r, list, 64) to fill a zero-terminated list of
// part pointers. n = (int) ceilf(r * 0.25) and limit = (r * 0.25)^2. For each part whose grid
// (pointer at part+12) has s16 at +18 plus the word at part+28 greater than -1000000: the point is
// moved into the part's frame as in worldFindHeight (dx = x - (float) word[+20], dy = y - (float)
// word[+24], c and s from the s16 at +32/+34 times the float 0x38000100; bit 0 of the halfword at
// +16 picks x' = dx*c + dy*s, y' = dx*s - dy*c over x' = dx*c + dy*s, y' = (-dx)*s + dy*c), scaled
// by 0.25 and floored to cell indices gx, gy. Cells with x in [gx-n, gx+n+1] and y in
// [gy-n, gy+n+1] that lie inside the grid (origin x at +0, origin y at +4, width at +8, rows at
// +12, s16 cells from +20) are visited row by row; a cell whose value has bit 0 set and whose
// squared distance from (x', y') to the cell index is below limit makes the result 1.0 at once.
// Inferred: bit 0 of a cell marks a forbidden spot. Unknown: structure names and the bit's
// meaning beyond this test.
// Hosts: with NORRATH_HOST_ARENA stored 32-bit pointers are offsets into an arena.
#if defined(NORRATH_HOST_ARENA)
extern unsigned char* norrath_height_arena;
#endif
extern int collisionFrame;
extern "C" {
float ceilf(float);
float floorf(float);
}
struct _worldHeader;
struct collisionPart;
struct Point3 { float x, y, z; };
extern "C" void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(
    _worldHeader* world, float x0, float y0, float x1, float y1, collisionPart** list, int limit);

namespace {
inline int word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const int*>(base + offset); }
inline short half(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const short*>(base + offset); }
inline unsigned char* grid_of(const unsigned char* part) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(part + 12);
#if defined(NORRATH_HOST_ARENA)
    return norrath_height_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" float worldTestForbidden__FP12_worldHeaderG6Point3f(_worldHeader* world, Point3 point, float radius) {
    collisionPart* list[64];
    const float px = point.x;
    const float py = point.y;
    collisionFrame = collisionFrame + 1;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(world, px - radius, py - radius, px + radius, py + radius, list, 64);
    const float quarter = radius * 0.25f;
    const int reach = static_cast<int>(ceilf(quarter));
    const float limit = quarter * quarter;
    const float scale = __builtin_bit_cast(float, 0x38000100u);
    for (int i = 0; list[i] != nullptr; ++i) {
        const unsigned char* part = reinterpret_cast<const unsigned char*>(list[i]);
        const unsigned char* grid = grid_of(part);
        if (!(-1000000 < half(grid, 18) + word(part, 28))) continue;
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
        for (int y = gy - reach; y <= ymax; ++y) {
            const int row = y - origin_y;
            if (row < 0 || row >= word(grid, 12)) continue;
            for (int x = xmin; x <= xmax; ++x) {
                const int column = x - origin_x;
                if (column < 0 || column >= width) continue;
                if (!(*reinterpret_cast<const unsigned short*>(grid + 20 + 2 * (row * width + column)) & 1u)) continue;
                const float fx = static_cast<float>(x) - rx;
                const float fy = static_cast<float>(y) - ry;
                if (fx * fx + fy * fy < limit) return 1.0f;
            }
        }
    }
    return 0.0f;
}
