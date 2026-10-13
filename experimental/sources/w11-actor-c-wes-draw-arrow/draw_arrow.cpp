#include "../w11-actor-c-common/c.hpp"
// Observed drawArrow__14WoodElfSoldier at 0x00249eb0, 192 bytes. Does nothing when the word at self+988 is
// zero. Otherwise it builds a 64-byte local matrix with matMakeTransYaw from the three floats at self+972
// and the unsigned halfword at self+984 minus 16384 (taken as a signed halfword), applies matRot with axis
// 0 and the angle -7192, and calls modelDraw with the model (word at +8) and texture (word at +12) of
// the global item record _4item$globalItem, the mode 3, the same three floats, the matrix, no animation
// state, the 64-bit flags value all ones, no draw record and a final 0 argument.
// Inferred: this draws the soldier's arrow (the item model) at a stored position and heading while the
// word at +988 is set; it is called from msg_draw__14WoodElfSoldier. Unknown: what the three floats and the
// halfword are named, and what sets the word at +988. Nothing here ran on the EE.
struct Point3 { float x, y, z; };
struct Matrix34 { float m[16]; };

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* matrix, Point3 position, short angle);
void matRot__FP8Matrix34is(Matrix34* matrix, int axis, short angle);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, Matrix34* matrix, void* animation,
    unsigned long long flags, void* record, int extra);
extern unsigned char globalItem[] __asm__("_4item$globalItem");

void drawArrow__14WoodElfSoldier(void* self) {
    using namespace w11c;
    if (word(self, 988) == 0) return;
    Matrix34 matrix;
    const Point3 position = *reinterpret_cast<const Point3*>(bytes(self) + 972);
    matMakeTransYaw__FP8Matrix34G6Point3s(&matrix, position, static_cast<short>(half(self, 984) - 16384u));
    matRot__FP8Matrix34is(&matrix, 0, -7192);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        link(globalItem, 8), link(globalItem, 12), 3, *reinterpret_cast<const Point3*>(bytes(self) + 972), &matrix, nullptr,
        ~0ull, nullptr, 0);
}
}
