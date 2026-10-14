// Observed SetClearColor__8VIRasterRC10VIColor32F at 0x0110a070, 476 bytes.
// It copies the 16 bytes (four floats) of the color to offset 17788 of the
// raster, converts each of the four stored floats to a byte with
// (value * 255 + 0.5) truncated as an unsigned conversion, packs them as
// r | g<<8 | b<<16 | a<<24 with 0x3f800000 in the upper half, and stores the
// packed word at offsets 752 and 384 of _8VIRaster$DBuffer (752 first), calls
// FlushCache(0) and returns zero. Init__8VIRasterPviii writes the same words
// from the same stored floats. The VIColor32F field order is inferred from
// the byte positions only.
extern "C" {
struct VIRaster;
struct VIColor32F {
    float r, g, b, a;
};

extern unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");

void FlushCache(int mode);

static unsigned char to_byte(float value) {
    return static_cast<unsigned char>(static_cast<unsigned>(value * 255.0f + 0.5f));
}

int SetClearColor__8VIRasterRC10VIColor32F(VIRaster* raster, const VIColor32F* color) {
    float* stored = reinterpret_cast<float*>(reinterpret_cast<char*>(raster) + 17788);
    stored[0] = color->r;
    stored[1] = color->g;
    stored[2] = color->b;
    stored[3] = color->a;
    unsigned char rgba[4];
    rgba[3] = 0xff;
    rgba[0] = 0;
    rgba[1] = 0;
    rgba[2] = 0;
    rgba[0] = to_byte(stored[0]);
    rgba[1] = to_byte(stored[1]);
    rgba[2] = to_byte(stored[2]);
    rgba[3] = to_byte(stored[3]);
    unsigned long long packed = static_cast<unsigned long long>(rgba[0]) | 0x3f80000000000000ull |
                                (static_cast<unsigned long long>(rgba[2]) << 16 |
                                 static_cast<unsigned long long>(rgba[1]) << 8) |
                                (static_cast<unsigned long long>(rgba[3]) << 24);
    *reinterpret_cast<unsigned long long*>(norrath_raster_dbuffer + 752) = packed;
    *reinterpret_cast<unsigned long long*>(norrath_raster_dbuffer + 384) = packed;
    FlushCache(0);
    return 0;
}
}
