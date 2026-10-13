// Observed msg_draw__16PoisonProjectile at 0x0020fd48, 200 bytes. O is the object.
// A 64-byte local matrix is built with matMakeTransYawScale__FP8Matrix34G6Point3sf from a copy of the
// 12-byte point at O+16, the signed halfword at O+56 plus 16384 (truncated to 16 bits) and the single
// at O+160. Then modelDraw runs once. The word at O+36 is the address of a table of 8-byte entries; the
// model is the word at +4 of entry [signed word at O+84] and the texture is the word at +4 of entry
// [signed word at O+88]. The other arguments are mode 1, a second copy of the point at O+16, the local
// matrix, the address O+100 (passed where the animation state goes), all-ones flags, a null draw
// record and a final 0.
// Inferred: the argument roles come from the mangled callee names. Unknown: what the table, the two
// indices and O+160 mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* m, Point3 p, short yaw, float scale);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags,
    void* drawRecord, int last);

void msg_draw__16PoisonProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Matrix34 m;
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    matMakeTransYawScale__FP8Matrix34G6Point3sf(&m, first, static_cast<short>(half(o, 56) + 16384), real(o, 160));
    unsigned char* const table = from_word(word(o, 36));
    void* const model = from_word(word(table + (sword(o, 84) << 3), 4));
    void* const texture = from_word(word(table + (sword(o, 88) << 3), 4));
    Point3 second = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 1, second, &m, o + 100, static_cast<unsigned long>(-1), nullptr, 0);
}
}
