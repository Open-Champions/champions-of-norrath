// Observed msg_draw__21PoisonArrowProjectile at 0x002a0500, 308 bytes. O is the object, H the signed halfword at O+100
// (a heading) and P the three floats at O+16. Steps in order:
//  1. A local vector D = (icos__Fi(H) * 8.0, isin__Fi(H) * 8.0, 0.0) is stored directly after the local matrix M (byte 48 of what
//     matMakeTransYaw__FP8Matrix34G6Point3s receives; the image's Matrix34 is twelve floats, so D is a separate local that follows
//     it; this source keeps both in one sixteen-float struct so that the callee sees the same memory). Then
//     matMakeTransYaw__FP8Matrix34G6Point3s(M, Q, (short)(unsigned H - 16384)) runs with Q = (P.x + D.x, P.y + D.y, P.z).
//  2. An eight-byte local array of halfwords is zeroed with memset, its halfword at byte 4 set to 128, and
//     lightSetAmbientOffset__FPs is called on it.
//  3. modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi is called with the
//     model (word at +8) and texture (word at +12) of the item record _4item$globalItem, mode 1, P (not Q), M, no animation
//     state, the 64-bit flags value all ones, no draw record and a final 0.
//  4. lightResetOptionalParams__Fv is called.
// Unknown: what the heading and the 128 ambient value mean. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

struct Matrix34 { float m[16]; };

extern "C" {
extern unsigned char globalItem[] __asm__("_4item$globalItem");
float icos__Fi(int angle);
float isin__Fi(int angle);
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* matrix, Point3 position, short angle);
void* memset(void* destination, int value, unsigned long count);
void lightSetAmbientOffset__FPs(short* offsets);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, Matrix34* matrix, void* animation,
    unsigned long long flags, void* record, int extra);
void lightResetOptionalParams__Fv();

void msg_draw__21PoisonArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    void* const model = from_word(word(globalItem, 8));
    void* const texture = from_word(word(globalItem, 12));
    const float scale = 8.0f;
    Matrix34 matrix;
    matrix.m[12] = icos__Fi(half(o, 100)) * scale;
    matrix.m[13] = isin__Fi(half(o, 100)) * scale;
    matrix.m[14] = 0.0f;
    Point3 shifted;
    shifted.x = position->x + matrix.m[12];
    shifted.y = position->y + matrix.m[13];
    shifted.z = position->z;
    matMakeTransYaw__FP8Matrix34G6Point3s(&matrix, shifted, static_cast<short>(static_cast<unsigned short>(half(o, 100)) - 16384u));
    short ambient[4];
    memset(ambient, 0, 8);
    ambient[2] = 128;
    lightSetAmbientOffset__FPs(ambient);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(model, texture, 1, *position, &matrix, nullptr, ~0ull, nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
