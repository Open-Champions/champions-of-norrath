// Observed worldFindPointHeight__FP12_worldHeaderG6Point3 at 0x00175a60, 804 bytes.
// Arguments: the world header and a Point3 (passed by address). Returns a float.
//
// Header words used: cell origin x at +8 and y at +12, cell grid width at +16 and height at
// +20, a table at +24 of pointers to per-cell lists, and an array of 36-byte part records
// at +32. The point's x and y are multiplied by the float 0x3bcccccc (0.00625) and floored
// (floorf) to a cell; outside [origin, origin + size) the result is 0.0. Otherwise the cell's
// list (signed halfwords ended by -1) is walked. For each part index: skip the part when bit 1
// (0x02) of its halfword at +16 is set, or when x is below the float of the s16 at +0, y below
// the s16 at +2, x above the s16 at +4 or y above the s16 at +6. Otherwise the point is moved
// into the part's frame: dx = x - (float) word[+20], dy = y - (float) word[+24], c = s16[+32]
// and s = s16[+34] each times the float 0x38000100; with bit 0 of the halfword at +16 set
// x' = dx*c + dy*s and y' = dx*s - dy*c, else x' = dx*c + dy*s and y' = (-dx)*s + dy*c.
// Both are multiplied by 0.25 and floored to grid cells (gx, gy) of the part's grid, whose
// pointer is at part+12 (words: origin x at 0, origin y at 4, width at 8, rows at 12, s16
// cells from byte 20). If the cell is inside the grid, the candidate is
// (s16 >> 4) + the word at part+28, and the best value (starting at -1000000) keeps the larger.
// If the best value is below -100000 the result is 0.0, else (float) best.
// Inferred: a ground-height probe over placed parts. Unknown: structure names and what bits
// 0 and 1 of the part halfword mean beyond the behaviours above.
// Hosts: with NORRATH_HOST_ARENA stored 32-bit pointers are offsets into an arena.
#if defined(NORRATH_HOST_ARENA)
extern unsigned char* norrath_height_arena;
#endif
extern "C" float floorf(float);
struct _worldHeader;
struct Point3 { float x, y, z; };

namespace {
inline int word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const int*>(base + offset); }
inline short half(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const short*>(base + offset); }
inline unsigned char* link(const unsigned char* base, unsigned offset) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(base + offset);
#if defined(NORRATH_HOST_ARENA)
    return norrath_height_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
}

extern "C" float worldFindPointHeight__FP12_worldHeaderG6Point3(_worldHeader* world, Point3 point) {
    const unsigned char* header = reinterpret_cast<const unsigned char*>(world);
    const float px = point.x;
    const float py = point.y;
    const float cell_scale = __builtin_bit_cast(float, 0x3bccccccu);
    const int cx = static_cast<int>(floorf(px * cell_scale)) - word(header, 8);
    const int cy = static_cast<int>(floorf(py * cell_scale)) - word(header, 12);
    if (cx < 0 || cy < 0 || cx >= word(header, 16) || cy >= word(header, 20)) return 0.0f;
    const unsigned char* list = link(link(header, 24) + (cy * word(header, 16) + cx) * 4, 0);
    int best = -1000000;
    const float scale = __builtin_bit_cast(float, 0x38000100u);
    for (int i = 0; half(list, 2 * i) != -1; ++i) {
        const unsigned char* part = link(header, 32) + half(list, 2 * i) * 36;
        const unsigned flags = *reinterpret_cast<const unsigned short*>(part + 16);
        if (flags & 2u) continue;
        if (px < static_cast<float>(half(part, 0))) continue;
        if (py < static_cast<float>(half(part, 2))) continue;
        if (static_cast<float>(half(part, 4)) < px) continue;
        if (static_cast<float>(half(part, 6)) < py) continue;
        const float dx = px - static_cast<float>(word(part, 20));
        const float dy = py - static_cast<float>(word(part, 24));
        const float c = static_cast<float>(half(part, 32)) * scale;
        const float s = static_cast<float>(half(part, 34)) * scale;
        const unsigned char* grid = link(part, 12);
        float rx, ry;
        if (flags & 1u) {
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
        const int row = gy - word(grid, 4);
        if (row < 0 || row > word(grid, 12) - 1) continue;
        const int column = gx - word(grid, 0);
        if (column < 0 || column > word(grid, 8) - 1) continue;
        const int candidate = (static_cast<int>(static_cast<short>(
                                  *reinterpret_cast<const unsigned short*>(grid + 20 + 2 * (row * word(grid, 8) + column)))) >> 4) +
                              word(part, 28);
        if (best < candidate) best = candidate;
    }
    if (best < -100000) return 0.0f;
    return static_cast<float>(best);
}
