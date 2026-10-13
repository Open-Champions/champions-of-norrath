// Steps shared by the two PhProp constructors that take a placement (a matrix, or a model and an
// animation bone). Offsets are bytes from the PhProp object; the roles of most fields are not
// established. Observed identically in both constructors (0x1489c0 and 0x148d08).
#pragma once
#include "ctor.hpp"

struct Matrix34 { float m[12]; };

#ifdef W11_HOST
#define sprintf w11_sprintf  // a host test supplies w11_sprintf(char*, const char*, ...)
#endif

extern "C" {
extern unsigned char vt_PhProp[] __asm__("_vt$6PhProp");
void prop_construct(void* self, Point3 position, char* a, char* b, char* c, long n, int flags) __asm__("__4PropG6Point3PcN22li");
void initStuff__6PhPropi(void* prop, int flags);
void matTransform__FP6Point3P8Matrix34T0(Point3* out, Matrix34* matrix, Point3* in);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
int sprintf(char* out, const char* format, ...);
}

namespace w11 {
struct Block72 { unsigned long long w[9]; };

// Word +72 = the PhProp table, halfwords at +364, +372, +380 and +388 = -1, bytes +360 and +361 =
// -1, and the eight halfwords at +344..+350 and +352..+358 = 0.
inline void phprop_defaults(void* self) {
    u32(self, 72) = word(vt_PhProp);
    for (int i = 3; i >= 0; i--) i16(self, 364 + 8 * i) = -1;
    u8(self, 361) = 0xff;
    u8(self, 360) = 0xff;
    for (int i = 0; i < 4; i++) {
        u16(self, 344 + 2 * i) = 0;
        u16(self, 352 + 2 * i) = 0;
    }
}

// A non-null description is copied (72 bytes) to +328, byte +568 = 1 and in the word at +12 bit
// 0x100000 is set and bit 0x800 cleared.
inline void phprop_description(void* self, const void* description) {
    if (description == nullptr) return;
    *reinterpret_cast<Block72*>(static_cast<unsigned char*>(self) + 328) = *static_cast<const Block72*>(description);
    u8(self, 568) = 1;
    u32(self, 12) = (u32(self, 12) | 0x100000u) & ~0x800u;
}

// Word +556 = -1, byte +570 = 0, word +564 = 0; then, when the 64-bit word at +232 is non-zero,
// word +556 = the index of its highest set bit.
inline void phprop_layer(void* self) {
    unsigned long long q = *reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(self) + 232);
    int n = -1;
    i32(self, 556) = -1;
    u8(self, 570) = 0;
    u32(self, 564) = 0;
    if (q != 0) {
        do {
            q >>= 1;
            n++;
        } while (q != 0);
        i32(self, 556) = n;
    }
}

// New position = P + matTransform(position - P), where P is the construction point.
inline void phprop_place(void* self, const Point3& p, Matrix34* matrix) {
    Point3 d;
    Point3& position = *reinterpret_cast<Point3*>(static_cast<unsigned char*>(self) + 16);
    d.x = position.x - p.x;
    d.y = position.y - p.y;
    d.z = position.z - p.z;
    matTransform__FP6Point3P8Matrix34T0(&d, matrix, &d);
    Point3 r;
    r.x = p.x + d.x;
    r.y = p.y + d.y;
    r.z = p.z + d.z;
    position = r;
}

// With byte +568 set and any of flag bits 0x19: word +68 = word +68 * 0x19660d + 0x3c6ef35f, the
// sound "game_swish_chest<n>.anm" (n = (word +68 >> 16) % 3 + 1) is looked up in "fx.lmp" and
// played on the object at volume 1.0. The word at +12 gets bit 0x2000 either way.
inline void phprop_finish(void* self, int flags) {
    if (static_cast<signed char>(u8(self, 568)) != 0 && (flags & 0x19) != 0) {
        char name[64];
        u32(self, 68) = u32(self, 68) * 0x19660du + 0x3c6ef35fu;
        sprintf(name, "game_swish_chest%d.anm", static_cast<int>((u32(self, 68) >> 16) % 3u) + 1);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, 1.0f);
    }
    u32(self, 12) |= 0x2000u;
}
}  // namespace w11
