// Observed msg_draw__8HolyBolt at 0x002bb2f0, 444 bytes. O is the object and P the point (three singles) at O+16; frameNm is the global
// frame counter. Two function-local ResourceMap objects of the image (16 bytes each, 0x008d7ef0 and 0x008d7f00) are built on first use
// behind two flag words (0x00486508 and 0x0048650c): when a flag is zero, the ResourceMap constructor __11ResourceMapPcT1 runs on its
// object with ("fx.lmp", "skull.vif") (first) or ("fx.lmp", "skull.tex") (second), and then the flag becomes 1; the first object is built
// before the second is tested. Steps in order after that:
//  1. The model is get__11ResourceMap(first object) and the texture get__11ResourceMap(second object), in that order.
//  2. modelSetHalo__FP7HaloDefi runs with a 16-byte local record copied from the image (four words 0x64a80000, 0x44480000, 0x64003c3c,
//     0x43c80000; the second and fourth read as the singles 800.0 and 400.0) and the argument 2.
//  3. lightSetAmbientOffset__FPs is called with four halfwords: 51, 59, 128, 48 (copied from a constant table).
//  4. A 64-byte local matrix is built with matMakeTransYaw__FP8Matrix34G6Point3s from a copy of P and the yaw (short)(the unsigned
//     halfword at O+56 + 16384); matRot__FP8Matrix34is rotates it on axis 0 by (short)(frameNm << 10).
//  5. modelDraw runs with the model, the texture, mode 3, a second copy of P, the matrix, a null animation, all-ones flags, a null draw
//     record and a final 0; lightResetOptionalParams__Fv is called last.
// Inferred: the argument roles from the mangled callee names; a spinning skull model with a halo. Unknown: the HaloDef fields. Nothing here
// ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
extern int frameNm;
extern unsigned char norrath_w16_spellmsg_a_view_8d7ef0[16];
extern unsigned char norrath_w16_spellmsg_a_view_8d7f00[16];
extern int norrath_w16_spellmsg_a_view_486508;
extern int norrath_w16_spellmsg_a_view_48650c;
void resource_map_construct(void* map, char* lump, char* name) __asm__("__11ResourceMapPcT1");
void* get__11ResourceMap(void* map);
void modelSetHalo__FP7HaloDefi(void* halo, int mode);
void lightSetAmbientOffset__FPs(short* offset);
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 p, short yaw);
void matRot__FP8Matrix34is(Matrix34* m, int axis, short angle);
void lightResetOptionalParams__Fv();
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags,
    void* drawRecord, int last);

void msg_draw__8HolyBolt(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (norrath_w16_spellmsg_a_view_486508 == 0) {
        resource_map_construct(norrath_w16_spellmsg_a_view_8d7ef0, const_cast<char*>("fx.lmp"), const_cast<char*>("skull.vif"));
        norrath_w16_spellmsg_a_view_486508 = 1;
    }
    if (norrath_w16_spellmsg_a_view_48650c == 0) {
        resource_map_construct(norrath_w16_spellmsg_a_view_8d7f00, const_cast<char*>("fx.lmp"), const_cast<char*>("skull.tex"));
        norrath_w16_spellmsg_a_view_48650c = 1;
    }
    void* const model = get__11ResourceMap(norrath_w16_spellmsg_a_view_8d7ef0);
    void* const texture = get__11ResourceMap(norrath_w16_spellmsg_a_view_8d7f00);
    unsigned halo[4];
    halo[0] = 0x64a80000u;
    halo[1] = 0x44480000u;
    halo[2] = 0x64003c3cu;
    halo[3] = 0x43c80000u;
    modelSetHalo__FP7HaloDefi(halo, 2);
    short offset[4];
    offset[0] = 51;
    offset[1] = 59;
    offset[2] = 128;
    offset[3] = 48;
    lightSetAmbientOffset__FPs(offset);
    Matrix34 m;
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    matMakeTransYaw__FP8Matrix34G6Point3s(&m, first, static_cast<short>(*reinterpret_cast<unsigned short*>(o + 56) + 16384));
    matRot__FP8Matrix34is(&m, 0, static_cast<short>(frameNm << 10));
    Point3 second = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 3, second, &m, nullptr, static_cast<unsigned long>(-1), nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
