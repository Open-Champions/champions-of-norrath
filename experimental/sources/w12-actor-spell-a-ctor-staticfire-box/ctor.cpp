// Observed __10StaticFireG6Point3fif at 0x0028da68, 664 bytes. This is the StaticFire overload that takes a position,
// a float A, an int N and a float B (not the one taking a short and an argument list). Arguments: the object, a
// position P (by value), A, N and B. Returns nothing. Steps, in order:
//  1. The GameObject constructor runs on the object with P, a null resource map and a null fourth argument.
//  2. _vt$10StaticFire goes to the word at +72. A light record at +128 is filled in place and handed to lightAdd:
//     bytes +136 and +137 are 0; halfwords +138, +146 = 255, +140, +148 = 128, +142, +144, +150, +152 = 0;
//     floats +156 and +164 = 120.0 and +160 and +168 = the single 0x3891a2b3; the float +172 = 1.0; the three floats at
//     +176 = (P.x + 0.0, P.y, P.z + 36.0).
//  3. Bytes +111 and +112 are cleared and init__10StaticFire runs on the object. The word at +124 becomes N,
//     byte +110 becomes 1, the float at +80 becomes A and the float at +104 is multiplied by B.
//  4. With O the object's position at +16 after init, objectFindInBox__FG6Point3T0PP10GameObjecti is called with
//     the corners O - 30.0 and O + 30.0 (all three components), an output list of 128 object words and the
//     capacity 128. Every listed object other than this object counts one; when its type (the function word at
//     byte 4 of its table, called with no arguments) equals __tf10StaticFire(), compared with
//     __eq__C9type_infoRC9type_info, lightDeactivate runs on its light at +128. A null entry would call
//     __throw_bad_typeid. When the count is 4 or more the word at +124 is halved (signed, rounding toward zero).
// Inferred: the box search lets a new fire switch off nearby fires' lights; the 128-entry list ends at a null word.
// Unknown: the meaning of N, A and B, and of bytes +110..+112. Nothing here ran on the EE.
#include "../w10-actor-m-common/access.hpp"

extern "C" {
extern unsigned char vt_StaticFire[] __asm__("_vt$10StaticFire");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void lightAdd__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
void init__10StaticFire(void* self);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity);
void* __tf10StaticFire();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
__attribute__((noreturn)) void __throw_bad_typeid();
void __10StaticFireG6Point3fif(void* self, Point3 position, float a, int n, float b);
}

extern "C" void __10StaticFireG6Point3fif(void* self, Point3 position, float a, int n, float b) {
    using namespace w10m;
    unsigned char* o = static_cast<unsigned char*>(self);
    game_object_construct(self, position, nullptr, nullptr);
    Point3 glow;
    glow.x = position.x + 0.0f;
    glow.y = position.y;
    glow.z = position.z + 36.0f;
    word(o, 72) = to_word(vt_StaticFire);
    unsigned char* light = o + 128;
    light[9] = 0;
    *reinterpret_cast<Point3*>(o + 176) = glow;
    real(light, 36) = 120.0f;
    word(light, 40) = 0x3891a2b3u;
    real(light, 44) = 1.0f;
    *reinterpret_cast<unsigned short*>(light + 18) = 255;
    *reinterpret_cast<unsigned short*>(light + 20) = 128;
    real(light, 28) = 120.0f;
    word(light, 32) = 0x3891a2b3u;
    *reinterpret_cast<unsigned short*>(light + 10) = 255;
    *reinterpret_cast<unsigned short*>(light + 12) = 128;
    *reinterpret_cast<unsigned short*>(light + 14) = 0;
    *reinterpret_cast<unsigned short*>(light + 16) = 0;
    *reinterpret_cast<unsigned short*>(light + 22) = 0;
    *reinterpret_cast<unsigned short*>(light + 24) = 0;
    light[8] = 0;
    lightAdd__FP5Light(light);
    o[111] = 0;
    o[112] = 0;
    init__10StaticFire(self);
    word(o, 124) = static_cast<unsigned>(n);
    const Point3 here = *reinterpret_cast<Point3*>(o + 16);
    o[110] = 1;
    real(o, 80) = a;
    real(o, 104) = real(o, 104) * b;
    Point3 high;
    high.x = here.x + 30.0f;
    high.y = here.y + 30.0f;
    high.z = here.z + 30.0f;
    Point3 low;
    low.x = here.x - 30.0f;
    low.y = here.y - 30.0f;
    low.z = here.z - 30.0f;
    unsigned list[128];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
    int found = 0;
    for (int i = 0; list[i] != 0; ++i) {
        unsigned char* other = from_word(list[i]);
        if (other == o) continue;
        if (other == nullptr) __throw_bad_typeid();
        const unsigned type_word = call_entry_plain(other, 0, nullptr);
        if (__eq__C9type_infoRC9type_info(from_word(type_word), __tf10StaticFire()) != 0) lightDeactivate__FP5Light(other + 128);
        ++found;
    }
    if (found >= 4) sword(o, 124) = sword(o, 124) / 2;
}
