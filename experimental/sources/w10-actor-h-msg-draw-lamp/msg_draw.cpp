// Observed msg_draw__4Lamp at 0x00231318, 140 bytes.
// Draws a facing sprite with P_FaceSprite__FG6Point3ffiiiiP8_texturesi: a copy of the 12 bytes at
// object+16, floats 13.0 and 7963.0, integers 128, 100, 90 and 115, the pointer variable
// haloTexture5, the short 0 and the integer 2. Then clears the byte at object+137; when the signed
// byte at object+136 is zero, it calls objectAddToRunList__FP10GameObject on the object and sets
// that byte to 1. Returns nothing.
// Unknown: what the sprite arguments and the two flag bytes mean. Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* position, float first, float second, int a, int b, int c,
                                               int d, void* texture, short e, int f);
void objectAddToRunList__FP10GameObject(void* object);
extern void* haloTexture5;
}

extern "C" void msg_draw__4Lamp(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char point[12];
    void* texture = haloTexture5;
    w10h::copy_bytes(point, object + 16, 12);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(point, 13.0f, 7963.0f, 128, 100, 90, 115, texture, 0, 2);
    const signed char queued = static_cast<signed char>(object[136]);
    object[137] = 0;
    if (queued == 0) {
        objectAddToRunList__FP10GameObject(self);
        object[136] = 1;
    }
}
