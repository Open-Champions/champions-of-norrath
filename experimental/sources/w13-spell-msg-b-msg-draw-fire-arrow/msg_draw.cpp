// Observed msg_draw__19FireArrowProjectile at 0x0029e570, 112 bytes. O is the object. One call:
// P_FaceSprite__FG6Point3ffiiiiP8_texturesi with the position copied from O+16 (three floats), size 10.0, a second
// size of float(O+28) * 1408.0, colour components 147, 98, 73, 73, the texture haloTexture4, spin 0 and flag 6.
// Nothing else is read or written. Unknown: which of the four integers are colour and alpha (the order is that
// of the declaration only), and what the float at O+28 holds. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern void* haloTexture4;
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag);

void msg_draw__19FireArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 position = *reinterpret_cast<Point3*>(o + 16);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, 10.0f, real(o, 28) * 1408.0f, 147, 98, 73, 73, haloTexture4, 0, 6);
}
}
