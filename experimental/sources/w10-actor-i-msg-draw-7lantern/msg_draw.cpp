// Observed msg_draw__7Lantern at 0x00231240, 212 bytes.
// Takes the same noise value as msg_run__7Lantern, n = ef1Noise(0x7efe41 + object * 1664525 +
// (frameNm << 4)), and draws a camera-facing sprite: P_FaceSprite with a stack copy of the 12 bytes at
// object+136, width 10.0f, height (float)(((int)(n * 22.0f) << 5) + 26084), the values 250, 150, 0 and
// 40, the texture pointer haloTexture4, angle 0 and mode 2. Then it clears the byte at object+149 and,
// when the signed byte at object+148 is zero, adds the object to the run list and sets that byte to 1.
// Inferred: byte 148 says the object is on the run list while it is being drawn. Unknown: the meaning
// of the four integer sprite values (the first three look like a colour, the fourth an alpha).
// Nothing here ran on the EE.
extern "C" {
extern int frameNm;
extern void* haloTexture4;
float ef1Noise__Fi(int seed);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int red, int green,
                                               int blue, int alpha, void* texture, short angle, int mode);
void objectAddToRunList__FP10GameObject(void* object);
}

namespace {
struct Point3 { float x, y, z; };
}

extern "C" void msg_draw__7Lantern(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const float noise = ef1Noise__Fi(0x7efe41 + static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(self)) * 0x19660d + (frameNm << 4));
    const Point3 position = *reinterpret_cast<const Point3*>(object + 136);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&position, 10.0f, static_cast<float>((static_cast<int>(noise * 22.0f) << 5) + 26084),
                                              250, 150, 0, 40, haloTexture4, 0, 2);
    object[149] = 0;
    if (static_cast<signed char>(object[148]) == 0) {
        objectAddToRunList__FP10GameObject(object);
        object[148] = 1;
    }
}
