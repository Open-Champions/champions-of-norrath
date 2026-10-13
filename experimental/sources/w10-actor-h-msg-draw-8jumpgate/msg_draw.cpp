// Observed msg_draw__8JumpGate at 0x00282fc0, 276 bytes.
// Sets the byte at object+208 to 1. Builds a matrix with matMakeTransYaw from a copy of the 12
// bytes at object+16 and the signed halfword at object+56. The animation pointer is object+216
// when the signed byte at object+77 is nonzero, otherwise null (selected again from the same byte
// for the second use). modelDrawShadow gets the model word at object+348, 0, the matrix, the
// animation pointer and 0; its result is the draw record of modelDraw, which gets the words at
// object+348 and +352, mode 0, a second copy of the point, the matrix, the animation pointer, the
// 32-bit value -1, that record and a zero final argument. Finally, when the signed byte at
// object+78 is nonzero, P_FaceSprite__FG6Point3ffiiiiP8_texturesi draws a sprite at the global
// point jumpGatePos with floats 20.0 and 19000.0, integers 100, 100, 188 and 64, the pointer
// variable haloTexture2, the short 0 and the integer 2. Returns nothing.
// Unknown: what the bytes at +77, +78 and +208 mean. Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* position, float first, float second, int a, int b, int c,
                                               int d, void* texture, short e, int f);
extern unsigned char jumpGatePos[12];
extern void* haloTexture2;
}

extern "C" void msg_draw__8JumpGate(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    alignas(16) unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    object[208] = 1;
    w10h::copy_bytes(first_point, object + 16, 12);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, *reinterpret_cast<short*>(object + 56));
    void* animation = static_cast<signed char>(object[77]) != 0 ? object + 216 : nullptr;
    void* record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
        w10h::from_word(w10h::word_at(self, 348)), 0, matrix, animation, 0);
    animation = static_cast<signed char>(object[77]) != 0 ? object + 216 : nullptr;
    w10h::copy_bytes(second_point, object + 16, 12);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        w10h::from_word(w10h::word_at(self, 348)), w10h::from_word(w10h::word_at(self, 352)), 0,
        second_point, matrix, animation, 0xffffffffu, record, 0);
    if (static_cast<signed char>(object[78]) != 0) {
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(jumpGatePos, 20.0f, 19000.0f, 100, 100, 188, 64, haloTexture2, 0, 2);
    }
}
