#include "../w11-actor-c-common/c.hpp"
// Observed msg_draw__6Sparks at 0x0021d450, 144 bytes. With s the object and n the global frameNm:
//   a Point3 is copied from s+16; size2 = isin__Fi(n << 10) * 250.0 + 12583.0 (the constant is a float
//   literal in the image's small data); P_FaceSprite__FG6Point3ffiiiiP8_texturesi is called with that
//   position, 5.0, size2, the integers 255, 200, 50, 128, the texture haloTexture1, a short spin of
//   (n & 0x7f) << 9 (the low 16 bits of the shifted value, sign-extended), and the last argument 3.
// Inferred: a pulsing halo drawn at the object's position, orange by its colour numbers. The meaning of
// the numeric arguments is not established. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern int frameNm;
extern void* haloTexture1;
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag);
}

extern "C" void msg_draw__6Sparks(void* self) {
    using namespace w11c;
    const float pulse = isin__Fi(frameNm << 10) * 250.0f;
    Point3 position = {real(self, 16), real(self, 20), real(self, 24)};
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, 5.0f, pulse + 12583.0f, 255, 200, 50, 128, haloTexture1,
                                              static_cast<short>((static_cast<unsigned short>(frameNm) & 0x7fu) << 9), 3);
}
