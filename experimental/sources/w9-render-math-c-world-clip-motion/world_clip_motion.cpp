// Observed worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3 at 0x001770f8, 1812 bytes.
// Arguments: the world header, a start Point3 and a motion Point3 (both by address, copied
// first), a float radius, an int mask and an optional output Point3. Returns a float: how
// far along the motion the mover gets, as a fraction (1.0 when it is not blocked).
//
// Steps:
//  1. collisionFrame is incremented and makeCollisionList__FP12_worldHeaderffffPP13collisionParti
//     is called with the motion's x/y bounding box widened by the radius and a 256-entry list.
//  2. Working scale is 0.25 (cells): the horizontal motion and the radius are multiplied by
//     0.25, len = sqrt(mx^2 + my^2) of that scaled motion, inv = 1 / len (no zero check), and
//     dir = (mx * inv, my * inv, raw motion z * inv). reach = (int) ceilf(radius). zlow =
//     (int) floorf(min(z, z + motion z)). The best distance starts at len.
//  3. For every listed part, unless flag bit 2 (halfword 16) is set, it is skipped when its top
//     (grid.s16[18] + word[28]) is below zlow + 12. The start and direction are moved into the part's
//     frame exactly as in worldHitscan (offset words 20 and 24, rotation from halfwords 32 and 34
//     times 0x38000100, bit 0 of halfword 16 selects the mirrored form), and the position is
//     scaled by 0.25 and floored to cell indices. The cell box spanning the start and end cells
//     widened by reach (end exclusive: +1 on the high side) is visited row by row inside the grid
//     (origin words 0 and 4, width word 8, rows word 12, cells from byte 20).
//  4. A cell is solid at height 32000 when (signed value & mask) is nonzero, otherwise at
//     (value >> 4) + word[28]. It is ignored when that height is below zlow + 12. For the remaining
//     cells, cross = (cx - px)*dy - (cy - py)*dx (the cell index relative to the local start,
//     with the local direction), and the cell is ignored when |cross| > scaled radius + 0.01,
//     when along = (cx - px)*dx + (cy - py)*dy is negative, or when z + along * dir.z + 12 is above
//     the cell height. Otherwise the hit distance is along - sqrt(r^2 - cross^2) (just along when
//     r^2 is not above cross^2); if it is below the best, it becomes the best and the cell's world
//     x/y (cell index * 4, rotated back and offset) are remembered.
//  5. If best >= len the result is 1.0. Otherwise, when an output pointer is given: out.z = 0 and
//     out.x/out.y receive the unit xy vector from the remembered cell to (start + dir * best),
//     using reciprocal square root; the result is best * inv.
// Inferred: a swept-circle test of a character against terrain cells. The point start + dir *
// best mixes the scaled direction with world units; this is reproduced as found.
// Unknown: struct names, the meaning of mask bits and flag bit 2, and 0x3c23d70a's role beyond
// a radius slack.
// Hosts: with NORRATH_HOST_CLIP the stored 32-bit grid pointers are offsets into an arena.
#if defined(NORRATH_HOST_CLIP)
extern unsigned char* norrath_clip_arena;
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
#if defined(NORRATH_HOST_CLIP)
    return norrath_clip_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline float pattern(unsigned bits) { return __builtin_bit_cast(float, bits); }
}

extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void* world, const float* position, const float* motion,
                                                                      float radius, int mask, float* out) {
    unsigned char* list[256];
    const float px = position[0], py = position[1], pz = position[2];
    float mx = motion[0], my = motion[1];
    const float mz = motion[2];
    const float tx = px + mx;
    const float ty = py + my;
    collisionFrame = collisionFrame + 1;
    makeCollisionList__FP12_worldHeaderffffPP13collisionParti(world, (px < tx ? px : tx) - radius, (py < ty ? py : ty) - radius,
                                                              (px > tx ? px : tx) + radius, (py > ty ? py : ty) + radius, list, 256);
    mx = mx * 0.25f;
    const float r4 = radius * 0.25f;
    const float r4sq = r4 * r4;
    my = my * 0.25f;
    const float len = __builtin_sqrtf(mx * mx + my * my);
    const float inv = 1.0f / len;
    const float dirx = mx * inv;
    const float diry = my * inv;
    const float dirz = mz * inv;
    const int reach = static_cast<int>(ceilf(r4));
    const float zsum = pz + mz;
    const int zlow = static_cast<int>(floorf(pz < zsum ? pz : zsum));
    float best = len;
    float hit_x = 0.0f, hit_y = 0.0f;
    const float scale = pattern(0x38000100u);
    for (int i = 0; list[i] != nullptr; ++i) {
        const unsigned char* part = list[i];
        const unsigned flags = *reinterpret_cast<const unsigned short*>(part + 16);
        const unsigned char* grid = grid_of(part);
        if (!(flags & 4u)) {
            if (half(grid, 18) + word(part, 28) < zlow + 12) continue;
        }
        const float dx = px - static_cast<float>(word(part, 20));
        const float dy = py - static_cast<float>(word(part, 24));
        const float c = static_cast<float>(half(part, 32)) * scale;
        const float s = static_cast<float>(half(part, 34)) * scale;
        float lx, ly, ldx, ldy;
        if (flags & 1u) {
            ly = dx * s - dy * c;
            lx = dx * c + dy * s;
            ldy = dirx * s - diry * c;
            ldx = dirx * c + diry * s;
        } else {
            ly = (-dx) * s + dy * c;
            lx = dx * c + dy * s;
            ldy = (-dirx) * s + diry * c;
            ldx = dirx * c + diry * s;
        }
        lx = lx * 0.25f;
        ly = ly * 0.25f;
        const int start_x = static_cast<int>(floorf(lx));
        const int start_y = static_cast<int>(floorf(ly));
        const int end_x = static_cast<int>(floorf(lx + ldx * len));
        const int end_y = static_cast<int>(floorf(ly + ldy * len));
        const int x_low = (start_x < end_x ? start_x : end_x) - reach;
        const int x_high = (end_x < start_x ? start_x : end_x) + reach + 1;
        const int y_high = (end_y < start_y ? start_y : end_y) + reach + 1;
        const int y_low = (start_y < end_y ? start_y : end_y) - reach;
        if (y_high < y_low) continue;
        const int origin_x = word(grid, 0);
        const int origin_y = word(grid, 4);
        const int width = word(grid, 8);
        for (int y = y_low; !(y_high < y); ++y) {
            const int row = y - origin_y;
            if (row < 0) continue;
            if (!(row < word(grid, 12))) continue;
            if (x_high < x_low) continue;
            const unsigned short* cell = reinterpret_cast<const unsigned short*>(grid + 20) + (row * width + (x_low - origin_x));
            for (int x = x_low; !(x_high < x); ++x, ++cell) {
                const int column = x - origin_x;
                if (column < 0) continue;
                if (!(column < width)) continue;
                const int shifted = static_cast<int>(static_cast<unsigned>(*cell) << 16);
                const int signed_value = shifted >> 16;
                int height;
                if ((signed_value & mask) != 0) height = 32000;
                else height = (shifted >> 20) + word(part, 28);
                if (height < zlow + 12) continue;
                const float cx = static_cast<float>(x) - lx;
                const float cy = static_cast<float>(y) - ly;
                const float cross = cx * ldy - cy * ldx;
                const float abs_cross = __builtin_fabsf(cross);
                if (r4 + pattern(0x3c23d70au) < abs_cross) continue;
                const float along = cx * ldx + cy * ldy;
                if (along < 0.0f) continue;
                if (static_cast<float>(height) < pz + along * dirz + 12.0f) continue;
                float root = 0.0f;
                const float remaining = r4sq - cross * cross;
                if (0.0f < remaining) root = __builtin_sqrtf(remaining);
                const float distance = along - root;
                if (!(distance < best)) continue;
                best = distance;
                const float fx = static_cast<float>(x * 4);
                const float fy = static_cast<float>(y * 4);
                if (flags & 1u) {
                    hit_y = (fx * s - fy * c) + static_cast<float>(word(part, 24));
                    hit_x = (fx * c + fy * s) + static_cast<float>(word(part, 20));
                } else {
                    hit_y = (fx * s + fy * c) + static_cast<float>(word(part, 24));
                    hit_x = (fx * c - fy * s) + static_cast<float>(word(part, 20));
                }
            }
        }
    }
    if (len <= best) return 1.0f;
    if (out != nullptr) {
        float along_vector[3] = {dirx * best, diry * best, dirz * best};
        out[2] = 0.0f;
        float point[3] = {px + along_vector[0], py + along_vector[1], pz + along_vector[2]};
        const float ox = point[0] - hit_x;
        const float oy = point[1] - hit_y;
        const float norm = 1.0f / __builtin_sqrtf(ox * ox + oy * oy);
        out[1] = oy * norm;
        out[0] = ox * norm;
    }
    return best * inv;
}
