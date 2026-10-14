// Observed shopDrawItemBitmapGlow__FP8_textureii at 0x001bc488, 344 bytes, 128-byte frame. Arguments:
// a texture pointer and two ints (x, y). It draws a 10 by 10 grid of soft-edged dots around (x, y):
// for dy and then dx each from -5 to 4 (dy is the outer loop) it computes
//   d = (int) sqrt((float)(dx*dx + dy*dy))          (sqrt.s on the EE, truncated)
//   t = (float)(-(d*d)) * K                        K = the float 0.064 (word 0x3d83126e, an
//                                                  unnamed gp-relative word at 0x004805f4)
//   a = dptoli(dpmul(fptodp(expf(t)), C))          C = the double with bits 0x402b722d0e560418
//                                                  (about 13.723, read from the image's data)
// a is limited to 0..255 (minimum with 255, then maximum with 0), and textureDrawColorSpecialHalo
// __FP8_textureiiiUiiiiii(texture, x + dx, y + dy, 5, 0x80000000 | (a << 8), 50, 0, 102, 640, 600)
// is called for each of the 100 cells. The doubles use the original's soft-float routines. Nothing
// is returned.
// Inferred: a radial falloff glow behind an item bitmap, with 0x80000000 | (a<<8) the colour word
// for the halo draw. Unknown: the meanings of 5, 50, 0, 102, 640 and 600 (the last two look like a
// screen size), and the colour-word layout.
typedef unsigned long long Double;

extern "C" {
struct _texture;
float expf(float value);
Double fptodp(float value);
Double dpmul(Double a, Double b);
int dptoli(Double value);
void textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(_texture* texture, int x, int y, int a3, unsigned color,
                                                        int a5, int a6, int a7, int a8, int a9);
}

namespace {
inline float squareRoot(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}
}  // namespace

extern "C" void shopDrawItemBitmapGlow__FP8_textureii(_texture* texture, int x, int y) {
    const float scale = __builtin_bit_cast(float, 0x3d83126eu);
    const Double peak = 0x402b722d0e560418ULL;
    for (int dy = -5; dy < 5; ++dy) {
        for (int dx = -5; dx < 5; ++dx) {
            int distance = static_cast<int>(squareRoot(static_cast<float>(dx * dx + dy * dy)));
            int alpha = dptoli(dpmul(fptodp(expf(static_cast<float>(-(distance * distance)) * scale)), peak));
            if (alpha > 255) alpha = 255;
            if (alpha < 0) alpha = 0;
            textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(
                texture, x + dx, y + dy, 5, (static_cast<unsigned>(alpha) << 8) | 0x80000000u, 50, 0, 102, 640, 600);
        }
    }
}
