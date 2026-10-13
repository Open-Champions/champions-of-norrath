// Observed: Vertex__8VIRasterRC8VIVect2I at 0x0110d398 is 292 bytes. The point argument holds two
// 32-bit integers (x, y). The raster holds a write pointer at offset 52 and a vertex count at
// 48. Four 16-byte entries are written at the pointer, which advances by 16 after each of the
// first three and a fourth time at the end (the count is incremented once):
//   entry 0: [0] = ((float)x - F17732 + 0.5f) * F17740
//            [4] = ((float)y - F17736 + 0.5f) * -F17744
//            [8] = 0 (word), [12] = 1.0f
//   entry 1: [0] = F12, [4] = F16, [8] = 1.0f, [12] = 0 (word)
//   entry 2: [0] = F20, [4] = F24, [8] = F28, [12] = 0 (word)
//   entry 3: [0] = F32, [4] = F36, [8] = F40, [12] = F44
// where Fn is the float at offset n of the raster. The result is zero. Entry 0 is stored through
// the pointer before it advances; entries 1 to 3 are stored through the advanced pointer.
// Inferred: entry 0 is a screen-space position (pixel centre offset, scale and flipped y);
// F32 to F44 are the current color words (Color__8VIRasterRC10VIColor32F writes 16 bytes at
// offset 32); F12 to F28 are the other current vertex attributes.
// Unknown: what offsets 12 to 28 and 17732 to 17744 are called, and the packet format consumer.
extern "C" {
struct VIRaster;
struct VIVect2I {
    int x;
    int y;
};

int Vertex__8VIRasterRC8VIVect2I(VIRaster* raster, const VIVect2I& point) {
    char* base = reinterpret_cast<char*>(raster);
    const float* f = reinterpret_cast<const float*>(base);
    float* const* cursor = reinterpret_cast<float* const*>(base + 52);
    float* p = *cursor;
    p[0] = (static_cast<float>(point.x) - f[17732 / 4] + 0.5f) * f[17740 / 4];
    p[1] = (static_cast<float>(point.y) - f[17736 / 4] + 0.5f) * -f[17744 / 4];
    reinterpret_cast<int*>(p)[2] = 0;
    p[3] = 1.0f;
    p += 4;
    p[0] = f[12 / 4];
    p[1] = f[16 / 4];
    p[2] = 1.0f;
    reinterpret_cast<int*>(p)[3] = 0;
    p += 4;
    p[0] = f[20 / 4];
    p[1] = f[24 / 4];
    p[2] = f[28 / 4];
    reinterpret_cast<int*>(p)[3] = 0;
    p += 4;
    p[0] = f[32 / 4];
    p[1] = f[36 / 4];
    p[2] = f[40 / 4];
    p[3] = f[44 / 4];
    p += 4;
    *reinterpret_cast<float**>(base + 52) = p;
    *reinterpret_cast<int*>(base + 48) += 1;
    return 0;
}
}
