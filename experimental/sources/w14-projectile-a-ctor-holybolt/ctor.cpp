// Observed __8HolyBoltG6Point3iiP8CreatureP5Skill at 0x002ba788, 1348 bytes. Arguments: the object, a position P (by value), integers A and
// B, a creature C (the caster) and a skill K. Returns nothing. T is the object's own position, the singles at +16, +20 and +24; entries
// of the found list are objects with a flag word at +12 and a position at +16. In order:
//  1. The Projectile constructor runs with (object, P, C, null). _vt$8HolyBolt goes to +72. A light record at +116 is filled: byte +9 and
//     +8 = 0, halfwords +10 and +18 = 172, +12 and +20 = 252, +14 and +22 = 500, +16 and +24 = -32, the singles +28 and +36 = 120.0, +32 and
//     +40 = the single 0x3891a2b3, +44 = 0.0. P goes to the three words at +164 and lightAdd runs on the address object+116.
//  2. The halfword +56 becomes A and the single +28 becomes 18.0. (icos(A) * 350.0, isin(A) * 350.0, 0.0), using A as a signed halfword,
//     go to the three singles at +88. The words +184 = K, +188 = B and the flag word +12 gets 0x4001 set. The entry at byte 88 of K's
//     table (the word K+96) is called on K with (2, B), giving R, and the word +84 becomes frameNm + 60 * R; objectAddToRunList runs.
//  3. objectFindInBox runs with the corners T - 360.0 and T + 360.0, a 32-word list on the frame and capacity 32. Two best distances
//     start at 10000000.0 (0x4b189680, called nearFront and nearBack here) and two indices at -1. Each entry in turn, with its index j, is
//     skipped when it is this object or C, when bit 0x20 of its flags is clear, when neither the distance d (the length of the vector
//     from T to the entry's position) is below nearFront nor below nearBack, when the signed byte at entry+172 is not 16, or when bit 0x08
//     of its flags is set. Otherwise cos and sin of the halfword at C+56 (icos__Fi, isin__Fi) are formed and the dot product with
//     (entry.x - C.x, entry.y - C.y) is taken: when above 0.0 nearFront = d and the front index = j, else nearBack = d and the back index = j.
//  4. The word +100 becomes the entry at the front index, or at the back index when the front index is -1 and the back index is not,
//     or 0 when both are -1.
//  5. With +100 equal to 0 the singles +104 and +108 become T.x + icos(C.angle) * 12.0 * 4000.0 and T.y + isin(C.angle) * 12.0 * 4000.0 and
//     +112 becomes T.z + 36.0; C.angle is the halfword at C+56.
//  6. P_StartTrail(4, 750, the single 0x3ea4dd2f, 4, 107, 100, 234, 171) returns an id stored in +180. P_ExtendTrail(id, (T.x + icos(A') *
//     10.0, T.y + isin(A') * 10.0, T.z)) runs, with A' the halfword at +56.
//  7. The resource "spel_he_dismiss.anm" is looked up in "fx.lmp" and played with soundPlaySequence(result, object, 1.0).
// Inferred: a bolt that homes on the nearest suitable object in front of or behind the caster; the byte value 16 at +172 is a type tag
// shared with the radial damage routine. Unknown: the meaning of A, B and the fields set in 2, and of the 4000.0 aim distance. Nothing
// here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char vt_HolyBolt[] __asm__("_vt$8HolyBolt");
void projectile_construct(void* self, Point3 position, void* owner, void* map) __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void lightAdd__FP5Light(void* light);
float icos__Fi(int angle);
float isin__Fi(int angle);
void objectAddToRunList__FP10GameObject(void* object);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void holy_bolt_construct(void* self, Point3 position, int a, int b, void* creature, void* skill) __asm__("__8HolyBoltG6Point3iiP8CreatureP5Skill");
}

static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_he_dismiss.anm";

extern "C" void holy_bolt_construct(void* self, Point3 position, int a, int b, void* creature, void* skill) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const c = static_cast<unsigned char*>(creature);
    unsigned char* const k = static_cast<unsigned char*>(skill);
    projectile_construct(self, position, creature, nullptr);
    unsigned char* const light = o + 116;
    word(o, 72) = to_word(vt_HolyBolt);
    light[9] = 0;
    *reinterpret_cast<Point3*>(o + 164) = position;
    real(light, 36) = 120.0f;
    half(light, 20) = 252;
    half(light, 22) = 500;
    half(light, 24) = -32;
    real(light, 28) = 120.0f;
    half(light, 12) = 252;
    half(light, 14) = 500;
    half(light, 16) = -32;
    word(light, 40) = 0x3891a2b3u;
    half(light, 18) = 172;
    word(light, 32) = 0x3891a2b3u;
    half(light, 10) = 172;
    real(light, 44) = 0.0f;
    light[8] = 0;
    lightAdd__FP5Light(light);
    half(o, 56) = static_cast<short>(a);
    real(o, 28) = 18.0f;
    const float reach = bits(0x43af0000u);
    const float bolt_x = icos__Fi(static_cast<short>(a)) * reach;
    const float bolt_y = isin__Fi(half(o, 56)) * reach;
    Point3 velocity;
    velocity.x = bolt_x;
    velocity.y = bolt_y;
    velocity.z = 0.0f;
    *reinterpret_cast<Point3*>(o + 88) = velocity;
    word(o, 184) = to_word(skill);
    word(o, 188) = static_cast<unsigned>(b);
    word(o, 12) |= 0x4001u;
    const int r = static_cast<int>(call_entry_from(k, 96, 88, 2, static_cast<unsigned>(b)));
    word(o, 84) = static_cast<unsigned>(frameNm) + static_cast<unsigned>(r) * 60u;
    objectAddToRunList__FP10GameObject(self);
    const float tx = real(o, 16), ty = real(o, 20), tz = real(o, 24);
    const float span = bits(0x43b40000u);
    Point3 low, high;
    low.x = tx - span;
    low.y = ty - span;
    low.z = tz - span;
    high.x = tx + span;
    high.y = ty + span;
    high.z = tz + span;
    unsigned list[32];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 32);
    float near_front = bits(0x4b189680u);
    float near_back = near_front;
    int front = -1, back = -1;
    for (int j = 0; list[j] != 0; ++j) {
        unsigned char* const e = from_word(list[j]);
        if (e == o || e == c) continue;
        const unsigned flags = word(e, 12);
        if ((flags & 0x20u) == 0) continue;
        const float dx = real(e, 16) - real(o, 16);
        const float dy = real(e, 20) - real(o, 20);
        const float dz = real(e, 24) - real(o, 24);
        const float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (!(distance < near_front) && !(distance < near_back)) continue;
        if (static_cast<signed char>(e[172]) != 16) continue;
        if ((flags & 0x8u) != 0) continue;
        const float cs = icos__Fi(half(c, 56));
        const float sn = isin__Fi(half(c, 56));
        const float ax = real(e, 16) - real(c, 16);
        const float ay = real(e, 20) - real(c, 20);
        if (0.0f < cs * ax + sn * ay) {
            near_front = distance;
            front = j;
        } else {
            near_back = distance;
            back = j;
        }
    }
    if (front != -1) word(o, 100) = list[front];
    else if (back != front) word(o, 100) = list[back];
    else word(o, 100) = 0;
    if (word(o, 100) == 0) {
        const float aim = bits(0x41400000u);
        const float far_distance = bits(0x457a0000u);
        real(o, 104) = real(o, 16) + icos__Fi(half(c, 56)) * aim * far_distance;
        real(o, 108) = real(o, 20) + isin__Fi(half(c, 56)) * aim * far_distance;
        real(o, 112) = real(o, 24) + bits(0x42100000u);
    }
    const int trail = P_StartTrail__Fiifiiiii(4, 750, bits(0x3ea4dd2fu), 4, 107, 100, 234, 171);
    word(o, 180) = static_cast<unsigned>(trail);
    const float step = bits(0x41200000u);
    const float sx = icos__Fi(half(o, 56)) * step;
    const float sy = isin__Fi(half(o, 56)) * step;
    Point3 tip;
    tip.x = real(o, 16) + sx;
    tip.y = real(o, 20) + sy;
    tip.z = real(o, 24);
    P_ExtendTrail__FiG6Point3(static_cast<int>(word(o, 180)), tip);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), self, 1.0f);
}
