// Observed __13FireBombletJrP11FireBomblet at 0x0028aca8, 1040 bytes: the constructor of the small fire bomblet spawned from
// a FireBomblet. Arguments: the object and the parent FireBomblet. Returns nothing. R() below is the sync random
// generator: syncRandState = syncRandState * 0x19660d + 0x3c6ef35f, taking the upper 16 bits. Steps, in order:
//  1. A vector V is drawn from the unit ball by rejection: each component is 2 * (R() * 2^-16) - 1, drawn x, y, z, redrawn
//     while 1.0 < x*x + y*y + z*z. The GameObject constructor then runs on the object with the position
//     (parent position at +16) + 5.0 * V, the resource map bombResMap and a null fourth argument.
//  2. Byte +40 is 6. A vector U = (parent float +76, parent float +80, 0.0) and a vector (0, -1, 0) are made. A roll
//     R() % 32000 (unsigned) is taken. _vt$13FireBombletJr goes to +72, the float 6.0 to +28 and 16385 to +12. The halfword
//     at +56 is the parent's halfword at +56 plus the roll minus 16384. Let L = sqrt(Ux*Ux + Uy*Uy + Uz*Uz).
//  3. A local Matrix33 is set to identity (matIdent__FP8Matrix33), rotated by matRot__FP8Matrix33is(matrix, 2, the new
//     halfword at +56), and matTransform__FP6Point3P8Matrix33T0 applies it to (0, -1, 0), writing the object's vector at
//     +76. A speed S = L * 0.5 + (R() * 2^-16) * 0.5 scales that vector. If its dot product with the parent's floats at
//     +76..+84 is below the single 0.707 the x and y components are negated.
//  4. Byte +88 and halfword +90 are cleared. Float +84 (the z slot of the vector at +76, overwritten) = parent float +84 * (0.5 + 0.5 * (R() * 2^-16)). Halfword +92
//     = (short)((R() * 2^-16 + 0.5) * 2615.0). Float +96 = parent float +164. Byte +89 = parent byte +157. The 64-bit
//     value at +104 = 1 << (R() & 3). objectAddToRunList runs on the object.
// Inferred: this throws a small burning piece away from the parent in a random direction. Unknown: the meanings of
// the fields. Nothing here ran on the EE.
#include "../w10-actor-m-common/access.hpp"

extern "C" {
extern unsigned char vt_FireBombletJr[] __asm__("_vt$13FireBombletJr");
extern unsigned char bombResMap[];
extern unsigned syncRandState;
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void objectAddToRunList__FP10GameObject(void* object);
void matIdent__FP8Matrix33(void* matrix);
void matRot__FP8Matrix33is(void* matrix, int axis, short angle);
void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* matrix, Point3* in);
void __13FireBombletJrP11FireBomblet(void* self, void* parent);
}

namespace {
inline unsigned roll16() {
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    return syncRandState >> 16;
}
}

extern "C" void __13FireBombletJrP11FireBomblet(void* self, void* parent) {
    using namespace w10m;
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* p = static_cast<unsigned char*>(parent);
    Point3 v;
    unit_ball(o, v, [](unsigned char*) { return static_cast<int>(roll16()); });
    Point3 offset;
    offset.x = v.x * 5.0f;
    offset.y = v.y * 5.0f;
    offset.z = v.z * 5.0f;
    Point3 start;
    start.x = real(p, 16) + offset.x;
    start.y = real(p, 20) + offset.y;
    start.z = real(p, 24) + offset.z;
    game_object_construct(self, start, bombResMap, nullptr);
    o[40] = 6;
    Point3 u;
    u.x = real(p, 76);
    u.y = real(p, 80);
    u.z = 0.0f;
    Point3 down;
    down.x = 0.0f;
    down.y = -1.0f;
    down.z = 0.0f;
    const unsigned roll = roll16() % 32000u;
    word(o, 72) = to_word(vt_FireBombletJr);
    real(o, 28) = 6.0f;
    word(o, 12) = 16385u;
    *reinterpret_cast<unsigned short*>(o + 56) = static_cast<unsigned short>(*reinterpret_cast<unsigned short*>(p + 56) + roll - 16384u);
    float length = __builtin_sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
    unsigned char matrix[36];
    matIdent__FP8Matrix33(matrix);
    matRot__FP8Matrix33is(matrix, 2, half(o, 56));
    matTransform__FP6Point3P8Matrix33T0(reinterpret_cast<Point3*>(o + 76), matrix, &down);
    length = length * 0.5f;
    length = length + static_cast<float>(static_cast<int>(roll16())) * bits(0x37800000u) * 0.5f;
    Point3& direction = *reinterpret_cast<Point3*>(o + 76);
    const Point3 old = direction;
    direction.x = old.x * length;
    direction.z = old.z * length;
    direction.y = old.y * length;
    const float dot = direction.x * real(p, 76) + direction.y * real(p, 80) + direction.z * real(p, 84);
    if (dot < bits(0x3f34fdf3u)) {
        direction.x = -direction.x;
        real(o, 80) = -real(o, 80);
    }
    const float scale = static_cast<float>(static_cast<int>(roll16())) * bits(0x37800000u) * 0.5f + 0.5f;
    o[88] = 0;
    *reinterpret_cast<short*>(o + 90) = 0;
    real(o, 84) = real(p, 84) * scale;
    const float spread = (static_cast<float>(static_cast<int>(roll16())) * bits(0x37800000u) + 0.5f) * bits(0x45237000u);
    real(o, 96) = real(p, 164);
    half(o, 92) = static_cast<short>(static_cast<int>(spread));
    o[89] = p[157];
    dword(o, 104) = static_cast<unsigned long long>(1 << (roll16() & 3u));
    objectAddToRunList__FP10GameObject(self);
}
