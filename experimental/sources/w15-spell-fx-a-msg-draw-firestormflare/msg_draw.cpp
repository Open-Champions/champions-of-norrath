// Observed: msg_draw__14FireStormFlare at 0x0029cfd8 is 280 bytes in the pinned SLUS-20565 image. Argument: the object o. Nothing
// is returned. When the word at o+88 is nonzero the function returns at once. Otherwise, in order:
//  1. A four-halfword ambient offset (200, 0, -150, 50 in the image, read as 8 bytes from 0x00486240) is copied to the stack and
//     passed to lightSetAmbientOffset; lightSetAmbientOnly runs.
//  2. A 48-byte matrix on the stack is built by matMakeTransYawScale(matrix, copy of the 12 bytes at o+16, 0, scale) with scale = the
//     float at o+80 times 0.85 (the pool constant with bits 0x3f599999, no symbol names it).
//  3. lumpFindResource("fx.lmp", "boulder.vif") gives the model and lumpFindResource("fx.lmp", "boulder.tex") the texture.
//     modelDraw runs with (model, texture, 3, copy of the 12 bytes at o+16, the matrix, a null animation state, the 64-bit word
//     -1, a null draw record, the int 0).
//  4. lightResetOptionalParams runs.
// Inferred: the drawing of a fire storm's flare as the shared boulder model with brightened ambient light, from the strings.
// Unknown: what the word at o+88 is (it suppresses the draw) and what the float at o+80 scales. Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" {
void lightSetAmbientOffset__FPs(short* offsets);
void lightSetAmbientOnly__Fv();
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, Point3 position, short yaw, float scale);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, void* matrix, void* animation, unsigned long long flags, void* record, int extra);
void lightResetOptionalParams__Fv();

void msg_draw__14FireStormFlare(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 88) != 0u) return;
    short ambient[4] = {200, 0, -150, 50};
    lightSetAmbientOffset__FPs(ambient);
    lightSetAmbientOnly__Fv();
    float matrix[12];
    matMakeTransYawScale__FP8Matrix34G6Point3sf(matrix, *reinterpret_cast<Point3*>(o + 16), 0, real(o, 80) * bits(0x3f599999u));
    void* const model = lumpFindResource__FPcT0("fx.lmp", "boulder.vif");
    void* const texture = lumpFindResource__FPcT0("fx.lmp", "boulder.tex");
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 3, *reinterpret_cast<Point3*>(o + 16), matrix, nullptr, ~0ull, nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
