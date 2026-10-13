// Observed worldPerturbWater__FP12_worldHeaderG6Point3ff at 0x0017a818, 792 bytes.
// Arguments: the world header, a Point3 (passed by address), a float strength and a float
// radius. Returns, as an int, the number of water patches it visited (those skipped
// for a missing cell array included). It does nothing and returns 0 while the byte
// cutSkipMode is nonzero.
//
// A box (x - radius, y - radius, z - 1.0) to (x + radius, y + radius, z + 60.0) is passed to
// worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi with a 16-entry
// output array. The array is read until a null entry. For every patch:
//  - the patch's first word points to a record whose word at offset 8 points to a base
//    address; a zero base skips the patch. The 32 x 32 array of signed halfwords that
//    is changed lives at base + 2704 (row index from the first axis, 32 columns).
//  - inv = 1 / ((float) (int) ((patch.f[20] - patch.f[8]) * 0x3daaaaaa + 0.5) * 12.0)
//    where the float 0x3daaaaaa is about 1/12 and the conversion truncates.
//  - lx = x - (float) patch.i[40] * 0.25 and ly = y - (float) patch.i[44] * 0.25 (the
//    third offset, patch.i[48], is combined with z but the result is never read).
//  - the cell-space centre is gx = lx * 32 * inv + 16, gy = ly * 32 * inv + 16, and the
//    squared radius in cells is rs2 = (radius * 32 * inv)^2.
//  - the cell box is [(int) (gx - radius - 1), (int) (gx + radius + 1)] by
//    [(int) (gy - radius - 1), (int) (gy + radius + 1)], clamped to 0 below and 31 above
//    (the radius itself, not the cell-space radius, widens the box).
//  - every cell in the box with dx^2 + dy^2 <= rs2 (dx, dy from the cell index to gx, gy)
//    gets (int) (strength - d2 * (strength / rs2)) added, truncating, and the sum is
//    capped at 32767 (no lower cap).
// Inferred: a ripple disturbance added to a water height grid. Unknown: the patch and
// header struct names, what the offsets 8, 20, 40, 44 and 48 hold, and whether the
// callee always terminates the list when 16 patches are found.
// Hosts: with NORRATH_HOST_PERTURB the stored 32-bit addresses are offsets into an arena.
#if defined(NORRATH_HOST_PERTURB)
extern unsigned char* norrath_perturb_arena;
#endif
extern signed char cutSkipMode;
extern "C" void worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
    void* world, const float* low, const float* high, unsigned char** out, int limit);

namespace {
inline unsigned char* resolve(unsigned bits) {
#if defined(NORRATH_HOST_PERTURB)
    return norrath_perturb_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned word_at(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
inline float float_at(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const float*>(base + offset); }
inline int int_at(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const int*>(base + offset); }
}

extern "C" int worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, const float* point, float strength, float radius) {
    unsigned char* patches[17];
    const float px = point[0], py = point[1], pz = point[2];
    if (cutSkipMode != 0) return 0;
    float low[3] = {px - radius, py - radius, pz - 1.0f};
    float high[3] = {px + radius, py + radius, pz + 60.0f};
    worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(world, low, high, patches, 16);
    int visited = 0;
    for (int i = 0; patches[i] != nullptr; ++i, ++visited) {
        const unsigned char* patch = patches[i];
        const unsigned char* record = resolve(word_at(patch, 0));
        const unsigned base = word_at(record, 8);
        if (base == 0u) continue;
        short* cells = reinterpret_cast<short*>(resolve(base) + 2704);
        const float lx = px - static_cast<float>(int_at(patch, 40)) * 0.25f;
        const float ly = py - static_cast<float>(int_at(patch, 44)) * 0.25f;
        const float span = (float_at(patch, 20) - float_at(patch, 8)) * __builtin_bit_cast(float, 0x3daaaaaau) + 0.5f;
        const float inv = 1.0f / (static_cast<float>(static_cast<int>(span)) * 12.0f);
        const float scaled_radius = radius * 32.0f * inv;
        const float gx = lx * 32.0f * inv + 16.0f;
        const float gy = ly * 32.0f * inv + 16.0f;
        const float rs2 = scaled_radius * scaled_radius;
        int x_low = static_cast<int>(gx - radius - 1.0f);
        int x_high = static_cast<int>(gx + radius + 1.0f);
        int y_low = static_cast<int>(gy - radius - 1.0f);
        int y_high = static_cast<int>(gy + radius + 1.0f);
        const float gain = strength / rs2;
        if (x_low < 0) x_low = 0;
        if (!(x_high < 32)) x_high = 31;
        if (y_low < 0) y_low = 0;
        if (!(y_high < 32)) y_high = 31;
        for (int x = x_low; x <= x_high; ++x) {
            const float dx = static_cast<float>(x) - gx;
            const float dx2 = dx * dx;
            for (int y = y_low; y <= y_high; ++y) {
                const float dy = static_cast<float>(y) - gy;
                const float d2 = dy * dy + dx2;
                if (rs2 < d2) continue;
                int value = static_cast<int>(strength - d2 * gain) + cells[x * 32 + y];
                if (32767 < value) value = 32767;
                cells[x * 32 + y] = static_cast<short>(value);
            }
        }
    }
    return visited;
}
