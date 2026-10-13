// Observed msg_draw__22ExplodeArrowProjectile at 0x002a2a58, 192 bytes. O is the object, H the signed halfword at O+100 (a
// heading) and P the three floats at O+16. Steps in order: matMakeTransYaw__FP8Matrix34G6Point3s(M, P,
// (short)(unsigned H - 16384)) fills a local 64-byte matrix M; then
// modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi is called with the model
// (word at +8) and texture (word at +12) of the item record _4item$globalItem, mode 1, P, M, no animation state, the
// 64-bit flags value all ones, no draw record and a final 0. Nothing else is read or written.
// Unknown: what the heading means. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

struct Matrix34 { float m[16]; };

extern "C" {
extern unsigned char globalItem[] __asm__("_4item$globalItem");
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* matrix, Point3 position, short angle);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, Matrix34* matrix, void* animation,
    unsigned long long flags, void* record, int extra);

void msg_draw__22ExplodeArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    void* const model = from_word(word(globalItem, 8));
    void* const texture = from_word(word(globalItem, 12));
    Matrix34 matrix;
    matMakeTransYaw__FP8Matrix34G6Point3s(&matrix, *position, static_cast<short>(static_cast<unsigned short>(half(o, 100)) - 16384u));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(model, texture, 1, *position, &matrix, nullptr, ~0ull, nullptr, 0);
}
}
