#include "../w10-actor-m-common/access.hpp"
// Observed msg_draw__4Soul at 0x1f3110, 508 bytes. Nothing is returned. The word at object+36 points to a
// table of resource pointers (byte 12 and 20 for the plain model and texture, 28 and 36 for the Soul's
// shadow-capable model and texture). Matrix34 is 48 bytes (three 16-byte rows are assumed from the size only).
//  A. With a minion (word at +960 nonzero): getSoulAttachMatrix__6MinionP8Matrix34(word at minion+12, M1);
//     modelGetAttachmentMatrix(model at 28, animation state at object+288, 0, M2, 1.0f);
//     matInvert(M2, M2); matMul(M1, M2, M1). The position is (x, y, z + 0.5*w) where x, y, z are the floats at
//     +16, +20, +24 and w the float at +32. r = modelDrawShadow(model at 28, 1, M1, animation, 0), then
//     modelDraw(model at 28, texture at 36, 1, position, M1, animation, doubleword at +552, r, 0).
//  B. Without a minion and with the signed halfword at +572 equal to 17: the same position, a matrix built by
//     matMakeTransYawScale(M, copy of the floats at +16, (short)(halfword at +56 + 24384), float at +224),
//     then matRot(M, 0, 20384), r = modelDrawShadow(model at 28, 1, M, animation, 0), and the same modelDraw
//     with M.
//  C. Otherwise: draw__8CreatureP12_modelHeaderP8_textureT1(object, model at 12, texture at 20, 0).
// The doubleword at +552 is passed in full (the image loads it with a 64-bit load into the unsigned long
// argument). Unknown: what the matrices and the 24384 / 20384 angle units mean (they look like 16-bit angles),
// what the doubleword at +552 holds, and what state 17 is.
struct Matrix34 { float v[12]; };
extern "C" {
void getSoulAttachMatrix__6MinionP8Matrix34(void* minion, Matrix34* out);
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* model, void* animation, int index, Matrix34* out, float scale);
void matInvert__FP8Matrix34T0(Matrix34* out, Matrix34* in);
void matMul__FP8Matrix34N20(Matrix34* out, Matrix34* a, Matrix34* b);
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int flag, Matrix34* matrix, void* animation, char extra);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int flag, Point3 position, Matrix34* matrix, void* animation, unsigned long long bits, void* record, int zero);
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* out, Point3 position, short yaw, float scale);
void matRot__FP8Matrix34is(Matrix34* matrix, int axis, short angle);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
}

extern "C" void msg_draw__4Soul(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* animation = o + 288;
    if (w10m::word(o, 960) != 0) {
        Matrix34 attach, inverse;
        getSoulAttachMatrix__6MinionP8Matrix34(w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 960)), 12)), &attach);
        modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(
            w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 28)), animation, 0, &inverse, 1.0f);
        matInvert__FP8Matrix34T0(&inverse, &inverse);
        matMul__FP8Matrix34N20(&attach, &inverse, &attach);
        Point3 position;
        position.x = w10m::real(o, 16);
        position.y = w10m::real(o, 20);
        position.z = w10m::real(o, 24) + w10m::real(o, 32) * 0.5f;
        void* record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
            w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 28)), 1, &attach, animation, 0);
        unsigned char* table = w10m::from_word(w10m::word(o, 36));
        modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
            w10m::from_word(w10m::word(table, 28)), w10m::from_word(w10m::word(table, 36)), 1, position, &attach, animation,
            w10m::dword(o, 552), record, 0);
    } else if (w10m::half(o, 572) == 17) {
        Point3 position;
        position.x = w10m::real(o, 16);
        position.y = w10m::real(o, 20);
        position.z = w10m::real(o, 24) + w10m::real(o, 32) * 0.5f;
        Matrix34 matrix;
        matMakeTransYawScale__FP8Matrix34G6Point3sf(&matrix, *reinterpret_cast<Point3*>(o + 16),
                                                    static_cast<short>(static_cast<unsigned>(static_cast<unsigned short>(w10m::half(o, 56))) + 24384u),
                                                    w10m::real(o, 224));
        matRot__FP8Matrix34is(&matrix, 0, 20384);
        void* record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
            w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 28)), 1, &matrix, animation, 0);
        unsigned char* table = w10m::from_word(w10m::word(o, 36));
        modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
            w10m::from_word(w10m::word(table, 28)), w10m::from_word(w10m::word(table, 36)), 1, position, &matrix, animation,
            w10m::dword(o, 552), record, 0);
    } else {
        unsigned char* table = w10m::from_word(w10m::word(o, 36));
        draw__8CreatureP12_modelHeaderP8_textureT1(self, w10m::from_word(w10m::word(table, 12)), w10m::from_word(w10m::word(table, 20)), nullptr);
    }
}
