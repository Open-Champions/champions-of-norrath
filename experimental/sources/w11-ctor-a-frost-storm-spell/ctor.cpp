// Observed __15FrostStormSpellG6Point3ifiP10GameObjectT1 at 0x1e1fe8, 1036 bytes. Arguments: the
// object, a position P (by value), an int A, a float F, an int B, an owner GameObject (dereferenced,
// so non-null) and a second Point3 Q (by value; the mangled T1 repeats the Point3 type). Returns
// nothing useful. Steps, in order:
//  1. The GameObject constructor runs with (object, P, null map, null names).
//  2. c = max(79.0, min(300.0, F * 1.71596)) using the hardware min.s and max.s (the single factor
//     has bits 0x3fdba483). Word +108 (a list head) = 0, word +72 = _vt$15FrostStormSpell.
//     The light record at object+112 is filled: byte +8 and +9 = 0, halfwords +10 and +18 = 0, +12
//     and +20 = 128, +14 and +22 = 255, +16 and +24 = 150, floats +28 and +36 = c, +32 and +40 =
//     1.0 / (c * c) (a run-time divide), float +44 = 0.7 (bits 0x3f333333) and P at +48 (object+160).
//     lightAdd__FP5Light(object+112) runs.
//  3. Bit 0x1 of the word at +12 is set and float +84 = F.
//  4. If (word at owner+12 & 0x60) == 0x20: word +80 = (int)((float)frameNm + 40.5) (bits
//     0x4221ffff) and word +76 = that + A, then blockAlloc__Fi(188) provides storage. When the storage
//     is non-null a second object is built in it with inline code (no LightEffect constructor call is
//     made): g = 2 * float(+84); GameObject constructor on the storage with (P.x + 0.0, P.y, P.z + 12.0),
//     null map and names; word +72 = _vt$11LightEffect; its light record at storage+76 gets byte +8/+9
//     = 0, halfwords +10/+18 = 0, +12/+20 = 128, +14/+22 = 255, +16/+24 = 100, floats +28/+36 = g,
//     +32/+40 = 1.0/(g*g), float +44 = 0.65 (bits 0x3f266666) and the point at +48 (storage+124);
//     lightAdd__FP5Light(storage+76); then word +136 = 0, bits 0x3 set in the word at +12, word +180 =
//     frameNm + 80, floats +144 = 128, +148 = 255, +152 = 100, +176 = -g * 0.0125 (bits 0x3c4ccccc),
//     +140, +156, +160, +164 and +168 = 0, +172 = g; objectAddToRunList(storage); and only after it
//     returns, word +184 = 1.
//     Otherwise word +80 = 0 and word +76 = frameNm + A.
//  5. Word +28 = 0, float +88 = 1.0, the vector Q * 0.0166667 (bits 0x3c888888) is stored at +96, +100,
//     +104, word +92 = B, listOnewayLink__FP8ListHeadT0Pv(object+108, owner, owner) runs and
//     objectAddToRunList(object) finishes.
// Inferred: the second object is a light effect (its class table name); its 100/255 shorts and floats
// are used like the other light records. Unknown: what A, B, F, Q and the many +140..+184 floats mean.
// Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

namespace {
float fp_min(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a < b ? a : b;
#endif
}
float fp_max(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a > b ? a : b;
#endif
}
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
}  // namespace

extern "C" {
extern unsigned char vt_FrostStormSpell[] __asm__("_vt$15FrostStormSpell");
extern unsigned char vt_LightEffect[] __asm__("_vt$11LightEffect");
void __15FrostStormSpellG6Point3ifiP10GameObjectT1(void* self, Point3 p, int a, float f, int b, void* owner, Point3 q);
}

extern "C" void __15FrostStormSpellG6Point3ifiP10GameObjectT1(void* self, Point3 p, int a, float f, int b, void* owner, Point3 q) {
    using namespace w11;
    unsigned char* const light = static_cast<unsigned char*>(self) + 112;
    unsigned char* const head = static_cast<unsigned char*>(self) + 108;
    game_object_construct(self, p, nullptr, nullptr);
    const float c = fp_max(79.0f, fp_min(300.0f, f * bits(0x3fdba483u)));
    const float c_squared = c * c;
    u32(self, 108) = 0;
    const float quotient = 1.0f / c_squared;
    u32(self, 72) = word(vt_FrostStormSpell);
    u8(light, 9) = 0;
    f32(self, 160) = p.x;
    f32(self, 164) = p.y;
    f32(self, 168) = p.z;
    f32(light, 44) = bits(0x3f333333u);
    i16(light, 24) = 150;
    f32(light, 28) = c;
    f32(light, 36) = c;
    f32(light, 40) = quotient;
    f32(light, 32) = quotient;
    i16(light, 10) = 0;
    i16(light, 12) = 128;
    i16(light, 14) = 255;
    i16(light, 16) = 150;
    i16(light, 18) = 0;
    i16(light, 20) = 128;
    i16(light, 22) = 255;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    u32(self, 12) |= 0x1u;
    f32(self, 84) = f;
    if ((u32(owner, 12) & 0x60u) == 0x20u) {
        const int frame_based = static_cast<int>(static_cast<float>(frameNm) + bits(0x4221ffffu));
        i32(self, 80) = frame_based;
        i32(self, 76) = frame_based + a;
        void* const storage = blockAlloc__Fi(188);
        if (storage) {
            const Point3 where = {p.x + 0.0f, p.y, p.z + 12.0f};
            const float base = f32(self, 84);
            const float g = base + base;
            unsigned char* const inner = static_cast<unsigned char*>(storage) + 76;
            game_object_construct(storage, where, nullptr, nullptr);
            const float g_squared = g * g;
            u32(storage, 72) = word(vt_LightEffect);
            const float inner_quotient = 1.0f / g_squared;
            u8(inner, 9) = 0;
            f32(storage, 124) = where.x;
            f32(storage, 128) = where.y;
            f32(storage, 132) = where.z;
            f32(inner, 44) = bits(0x3f266666u);
            i16(inner, 24) = 100;
            f32(inner, 40) = inner_quotient;
            f32(inner, 32) = inner_quotient;
            i16(inner, 16) = 100;
            i16(inner, 20) = 128;
            i16(inner, 22) = 255;
            f32(inner, 28) = g;
            f32(inner, 36) = g;
            i16(inner, 10) = 0;
            i16(inner, 12) = 128;
            i16(inner, 14) = 255;
            i16(inner, 18) = 0;
            u8(inner, 8) = 0;
            lightAdd__FP5Light(inner);
            u32(storage, 136) = 0;
            const float negated = -g * bits(0x3c4ccccc);
            const unsigned flags = u32(storage, 12);
            const int stamp = frameNm + 80;
            f32(storage, 144) = 128.0f;
            u32(storage, 12) = flags | 0x3u;
            i32(storage, 180) = stamp;
            f32(storage, 148) = 255.0f;
            f32(storage, 152) = 100.0f;
            f32(storage, 168) = 0.0f;
            f32(storage, 176) = negated;
            f32(storage, 140) = 0.0f;
            f32(storage, 156) = 0.0f;
            f32(storage, 160) = 0.0f;
            f32(storage, 164) = 0.0f;
            f32(storage, 172) = g;
            objectAddToRunList__FP10GameObject(storage);
            u32(storage, 184) = 1;
        }
    } else {
        u32(self, 80) = 0;
        i32(self, 76) = frameNm + a;
    }
    u32(self, 28) = 0;
    f32(self, 88) = 1.0f;
    const float k = bits(0x3c888888u);
    const Point3 velocity = {q.x * k, q.y * k, q.z * k};
    u32(self, 92) = static_cast<unsigned>(b);
    f32(self, 96) = velocity.x;
    f32(self, 100) = velocity.y;
    f32(self, 104) = velocity.z;
    listOnewayLink__FP8ListHeadT0Pv(head, owner, owner);
    objectAddToRunList__FP10GameObject(self);
}
