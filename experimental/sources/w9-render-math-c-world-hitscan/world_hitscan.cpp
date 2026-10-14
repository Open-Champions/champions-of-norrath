// Observed worldHitscan__FP12_worldHeaderG6Point3T1fi at 0x0017b7e0, 1968 bytes.
// Arguments: the world header, a start Point3 and a direction Point3 (both by address and
// copied first), a float maximum distance and an int mode. Returns a float: the distance
// along the direction as a multiple of the direction vector's horizontal length, or
// -1.0 when nothing is hit. The same entry is the target of the placeholder name
// norrath_world_hitscan used by the Web Trap constructor; both names are defined below
// with the same body.
//
// Working scale: the horizontal direction is multiplied by 0.25 (collision cells are 4 world
// units), len = sqrt(dx^2 + dy^2) of that scaled direction, limit = max_distance * len.
//  - len < 0.01 (a vertical ray): returns start.z - worldFindPointHeight(world, start) when the
//    direction z is negative, otherwise -1.0.
//  - otherwise the direction is divided by len (x and y from the scaled values, z from the
//    raw value), so its horizontal length is 1. If limit is not above zero, -1.0 is returned.
//  - The ray is cut into segments of 240 units. collisionFrame is incremented once, then for
//    each segment (at most 1001): makeCollisionList__FP12_worldHeaderffffPP13collisionParti is
//    called with the segment's x/y bounding box and a 256-entry list. For each listed part
//    (a mode that is nonzero in its low 16 bits together with part flag 4 skips the first
//    test; otherwise a part whose top, grid.s16[18] + word[28], is below a z floor is skipped;
//    the floor is -10000000 for downward rays and the segment start z for others):
//     * the start and direction are moved into the part's frame the same way as in
//       worldFindHeight (offsets words 20 and 24, z offset word 28, rotation from halfwords 32
//       and 34 times 0x38000100, bit 0 of halfword 16 selects the mirrored form), the xy
//       position is scaled by 0.25 (cell units), and the position is clipped to the grid box
//       (origin words 0 and 4, size words 8 and 12): rays pointing away from the box are
//       rejected with the 0.01 slack, otherwise the position is advanced to the box edge, the
//       advance adding to the distance t and dz (z advances by direction z per unit).
//     * the ray then steps one cell length at a time (at most 1001 steps; up to four steps
//       outside the grid are tolerated): inside the grid, the u16 cell at row-major index is
//       decoded as (cell<<16 >> 20) + ((cell<<16) & (mode<<16)); when the current z is below
//       that value the hit is recorded if t is below the best and the part ends.
//    After the parts, if the best t is below 1.0e10, the result is
//    (best + 0.25 * travelled) / len when that is below limit, else -1.0, where travelled is 240 per
//    completed segment. Otherwise the next segment starts at the previous end point, until
//    0.25 * travelled reaches limit, which returns -1.0.
// Float constants (patterns): 0.01 = 0x3c23d70a, -0.01 = 0xbc23d70a, 1/32767 = 0x38000100,
// initial best = 0x51ba43b7, found limit = 0x501502f9, downward z floor = 0xcb189680.
// Inferred: a ray cast through the terrain collision grids. Unknown: names of the part and
// grid structs and the meaning of the cell low four bits and the mode mask.
// Hosts: with NORRATH_HOST_HITSCAN the stored 32-bit grid pointers are offsets into an arena.
#if defined(NORRATH_HOST_HITSCAN)
extern unsigned char* norrath_hitscan_arena;
#endif
extern int collisionFrame;
extern "C" {
void makeCollisionList__FP12_worldHeaderffffPP13collisionParti(void* world, float x0, float y0, float x1, float y1,
                                                                unsigned char** list, int limit);
float worldFindPointHeight__FP12_worldHeaderG6Point3(void* world, const float* point);
}

namespace {
inline int word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const int*>(base + offset); }
inline short half(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const short*>(base + offset); }
inline unsigned char* grid_of(const unsigned char* part) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(part + 12);
#if defined(NORRATH_HOST_HITSCAN)
    return norrath_hitscan_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline float pattern(unsigned bits) { return __builtin_bit_cast(float, bits); }

__attribute__((always_inline)) inline float hitscan(void* world, const float* from, const float* direction, float max_distance, int mode) {
    float start[3] = {from[0], from[1], from[2]};
    float dir[3] = {direction[0], direction[1], direction[2]};
    unsigned char* list[256];
    const unsigned mask = static_cast<unsigned>(mode) << 16;
    dir[0] = dir[0] * 0.25f;
    dir[1] = dir[1] * 0.25f;
    const float len = __builtin_sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);
    if (len < pattern(0x3c23d70au)) {
        if (dir[2] < 0.0f) return start[2] - worldFindPointHeight__FP12_worldHeaderG6Point3(world, start);
        return -1.0f;
    }
    const float limit = max_distance * len;
    const float inverse = 1.0f / len;
    collisionFrame = collisionFrame + 1;
    dir[0] = dir[0] * inverse;
    dir[1] = dir[1] * inverse;
    dir[2] = dir[2] * inverse;
    if (!(0.0f < limit)) return -1.0f;
    float travelled = 0.0f;
    int segments = 0;
    const float scale = pattern(0x38000100u);
    for (;;) {
        if (!(segments < 1001)) return -1.0f;
        ++segments;
        const float step_x = dir[0] * 240.0f;
        const float step_y = dir[1] * 240.0f;
        const float step_z = dir[2] * 240.0f;
        float end[3] = {start[0] + step_x, start[1] + step_y, start[2] + step_z};
        const float lo_x = start[0] < end[0] ? start[0] : end[0];
        const float hi_x = start[0] > end[0] ? start[0] : end[0];
        const float hi_y = start[1] > end[1] ? start[1] : end[1];
        const float lo_y = start[1] < end[1] ? start[1] : end[1];
        makeCollisionList__FP12_worldHeaderffffPP13collisionParti(world, lo_x, lo_y, hi_x, hi_y, list, 256);
        float best = pattern(0x51ba43b7u);
        const float floor_z = dir[2] < 0.0f ? pattern(0xcb189680u) : start[2];
        for (int i = 0; list[i] != nullptr; ++i) {
            const unsigned char* part = list[i];
            const unsigned char* grid = grid_of(part);
            const unsigned flags = *reinterpret_cast<const unsigned short*>(part + 16);
            const int z_offset = word(part, 28);
            if (!(mask != 0u && (flags & 4u) != 0u)) {
                if (static_cast<float>(half(grid, 18) + z_offset) < floor_z) continue;
            }
            const float dx = start[0] - static_cast<float>(word(part, 20));
            const float dy = start[1] - static_cast<float>(word(part, 24));
            float dz = start[2] - static_cast<float>(z_offset);
            const float s = static_cast<float>(half(part, 34)) * scale;
            const float c = static_cast<float>(half(part, 32)) * scale;
            float lx, ly, ldx, ldy;
            if (flags & 1u) {
                ly = dx * s - dy * c;
                lx = dx * c + dy * s;
                ldx = dir[0] * c + dir[1] * s;
                ldy = dir[0] * s - dir[1] * c;
            } else {
                lx = dx * c + dy * s;
                ly = (-dx) * s + dy * c;
                ldx = dir[0] * c + dir[1] * s;
                ldy = (-dir[0]) * s + dir[1] * c;
            }
            lx = lx * 0.25f;
            ly = ly * 0.25f;
            const int origin_x = word(grid, 0);
            const int origin_y = word(grid, 4);
            const int width = word(grid, 8);
            const int rows = word(grid, 12);
            const int limit_x = origin_x + width;
            const int limit_y = origin_y + rows;
            float t = 0.0f;
            if (lx < static_cast<float>(origin_x)) {
                if (ldx <= pattern(0x3c23d70au)) continue;
                const float advance = (static_cast<float>(origin_x) - lx) / ldx;
                t = advance;
                const float mx = ldx * advance;
                const float my = ldy * advance;
                lx = lx + mx;
                ly = ly + my;
                dz = dz + dir[2] * advance;
            }
            if (static_cast<float>(limit_x) < lx) {
                if (pattern(0xbc23d70au) <= ldx) continue;
                const float advance = (static_cast<float>(limit_x) - lx) / ldx;
                t = t + advance;
                const float mx = ldx * advance;
                const float my = ldy * advance;
                lx = lx + mx;
                ly = ly + my;
                dz = dz + dir[2] * advance;
            }
            if (ly < static_cast<float>(origin_y)) {
                if (ldy <= pattern(0x3c23d70au)) continue;
                const float advance = (static_cast<float>(origin_y) - ly) / ldy;
                t = t + advance;
                const float mx = ldx * advance;
                const float my = ldy * advance;
                lx = lx + mx;
                ly = ly + my;
                dz = dz + dir[2] * advance;
            }
            if (static_cast<float>(limit_y) < ly) {
                if (pattern(0xbc23d70au) <= ldy) continue;
                const float advance = (static_cast<float>(limit_y) - ly) / ldy;
                t = t + advance;
                const float mx = ldx * advance;
                const float my = ldy * advance;
                lx = lx + mx;
                ly = ly + my;
                dz = dz + dir[2] * advance;
            }
            const unsigned short* cells = reinterpret_cast<const unsigned short*>(grid + 20);
            int cell_x = static_cast<int>(lx) - origin_x;
            int cell_y = static_cast<int>(ly) - origin_y;
            int outside = 0;
            int walked = 1;
            for (;;) {
                if (cell_x >= 0 && cell_y >= 0 && cell_x < width && cell_y < rows) {
                    const int cell = static_cast<int>(static_cast<unsigned>(cells[cell_y * width + cell_x]) << 16);
                    const int height = (cell >> 20) + static_cast<int>(static_cast<unsigned>(cell) & mask);
                    if (dz < static_cast<float>(height)) {
                        if (t < best) best = t;
                        break;
                    }
                } else {
                    if (!(outside < 4)) break;
                    ++outside;
                }
                const bool more = walked < 1001;
                t = t + 1.0f;
                ++walked;
                lx = lx + ldx;
                ly = ly + ldy;
                cell_x = static_cast<int>(lx) - origin_x;
                cell_y = static_cast<int>(ly) - origin_y;
                dz = dz + dir[2];
                if (!more) break;
            }
        }
        if (best < pattern(0x501502f9u)) {
            const float distance = best + travelled * 0.25f;
            if (distance < limit) return distance * inverse;
            return -1.0f;
        }
        travelled = travelled + 240.0f;
        start[0] = end[0];
        start[1] = end[1];
        start[2] = end[2];
        if (!(travelled * 0.25f < limit)) return -1.0f;
    }
}
}  // namespace

extern "C" float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, const float* from, const float* direction, float max_distance, int mode) {
    return hitscan(world, from, direction, max_distance, mode);
}

// Keeps this a complete second copy rather than a jump to the first (no identical-code
// folding, no tail call); the compiled bodies are identical apart from branch targets.
#if defined(__GNUC__) && !defined(__clang__)
#define NORRATH_KEEP_COPY __attribute__((optimize("no-ipa-icf", "no-optimize-sibling-calls")))
#else
#define NORRATH_KEEP_COPY
#endif
extern "C" NORRATH_KEEP_COPY float norrath_world_hitscan(void* world, const void* from, const void* direction, float max_distance, int mode) {
    return hitscan(world, static_cast<const float*>(from), static_cast<const float*>(direction), max_distance, mode);
}
