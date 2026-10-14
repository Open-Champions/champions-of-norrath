// Observed msg_run__6Cthulu at 0x00280da8, 3448 bytes.
// Notation: o is the object, w(k) the int word at o+k, setState(n) the call of virtual slot byte
// 184 of the table at o+72 with the int n, zone(h, d) = enemyInZone__8Creaturefs(o, float +28 + h,
// d), and bit8 the bit 8 of the halfword +294.
// Entry: newDamageHandler(o, word at (word +36)+12); everyFrame(o, the same word) returning nonzero
// ends the function, as does objectActivationCheck(o, 600) returning zero; then
// S = creatureAdvanceAnimation(o, 1.0) (its result, which the image keeps in a saved register
// before calling slideBack); slideBack(o, 0.5) (result unused); virtual slot byte 168 is called (no second
// argument); the signed halfword +572 picks a case (unsigned compare against 17; states 2, 4, 7, 11,
// 12 and anything above 16 do nothing).
//  0: enemyInZone(o, 400.0, 32767) (the height is 400.0 itself, without the +28 float) ->
//     setState(3); else if w(256) < w(808): setState(1).
//  1: enemyInZone(o, 400.0, 32767) -> setState(3); else if w(256) < w(808): setState(0); else
//     smartWander(o).
//  3: w(576) zero: setState(0). Otherwise near = zone(40, 5000) != 0; if w(964) nonzero and near:
//     setState(6). If w(964) is zero: if zone(40, 5000) again is nonzero then r = eRand % 3 (low
//     16 bits) picks setState(6), setState(13) or setState(14) for 0, 1, 2 and the function ends;
//     else if w(972) is zero and zone(100, 5000) is nonzero: setState(15), end. Then, if w(968) <
//     w(808) and zone(300, 5000): setState(16), end. Finally zone(40, 32767) ? turnToFaceEnemy(o) :
//     steerAndRouteToEnemy(o).
//  5: zone(40, 5000) -> setState(6); else zone(300, 5000) -> setState(16); else
//     bExitStatePersueIdle(o) nonzero -> setState(3).
//  6, 15: move2(o, a zero Point3). 13, 14: no move2. Then, when w(576) is nonzero, the melee block
//     runs if bit 0 of S is set, and turnToFaceEnemy(o) runs if bit8 is set. Then bit8 ends the
//     function. State 6: if w(964) is nonzero, eRand is called and w(960) = 0; (6, 13, 14, 15:)
//     setState(3).
//  16: move2 as above. When w(576) is nonzero: P = (x + 300*icos(h), y + 300*isin(h), z) with h the
//     signed halfword +56; findEnemyLeadPosition(o, 400.0, &P); when bit 0 of S is set one
//     poison projectile is fired (see below); bit8 -> turnToFaceEnemy(o). Then bit8 ends the
//     function, else setState(3).
//  8: unless bit 4 of the halfword +294 is set: setState(0).
//  9: if w(804) < w(808): setState(3).
//  10: bit 7 (0x80) of the halfword +294 -> objectRemoveFromRunList(o). If bit 2 of S is set and
//     bit 2 of w(12) is clear: r1 = eRand, r2 = eRand, floorAdd(copy of the Point3 at +16, (short)r1,
//     (float)(r2 % 10 + 30), 3, 1).
// Melee block: an ItemDescription D (initialised as in msg_poke__5Chesti); r = eRand % 10; when r is
// zero, val = virtual slot byte 96 of the object whose word is at skills+104 (table pointer at
// that object+96; called with (object + adjustment, 0, (int)worldDifficulty)), and the first of the
// four slots (halfword at D+36+8i) holding -1 or 23 is set to 23 with halfwords D+38+8i and D+40+8i =
// val and D+42+8i = 0 (the image tests for 23 as well, which cannot be hit because all four slots
// start at -1). Then produceMeleeDamage(o, 36.0, (short)+200, (short)+202, (short)+204,
// 0x10008, &D).
// Fire block (state 16): R = (x + 10*icos(h), y + 10*isin(h), z + 24); a = (short)(16384 -
// iatan2(P.x - x, P.y - y)); d = (short)(a - (unsigned)h); a is replaced by (short)(h + 4000) when d >
// 4000 and by (short)(h - 4000) when d < -4000; a = (short)(a + eRand % 2000 - 1000); V = (400 *
// icos(a), 400 * isin(a), (eRand % 30 + 80) * 0.0025 * |P - o|); a PoisonProjectile of 168 bytes is
// built (if the allocation works) from the resource map word at +36, 21, 22, 23, 25, R, V,
// h - 16384, a scale of eRand * 2^-16 * 0.5 + 0.5 and the object.
// Inferred: this is the Cthulu AI body (melee in close range, spitting projectiles, charging).
// Unknown: state and field names, the meaning of the constants, the size of ItemDescription (at
// most 80 bytes, only the initialised bytes are reproduced). Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

#if defined(W10_ACTOR_D_HOST)
extern int w10_actor_d_skill_function(unsigned function, void* skill, int a, int b);
#endif

struct Point3 {
    float x, y, z;
};

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float value);
int slideBack__8Creaturef(void* self, float value);
int enemyInZone__8Creaturefs(void* self, float height, short distance);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void move2__8CreatureG6Point3(void* self, const Point3* point);
int eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float a, float b);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float value, Point3* point);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* description);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const Point3* position, short a, float b, int c, int d);
void* blockAlloc__Fi(int size);
void poisonProjectile_construct(void* self, void* map, int a, int b, int c, int d, const Point3* at, const Point3* velocity, int angle, float scale, void* creature)
    __asm__("__16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature");
extern unsigned char skills[];
extern float worldDifficulty;
}

namespace {
template <typename T>
inline T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }

struct ItemDescription {
    unsigned char bytes[80];
};

inline void set_state(void* self, int state) { w10_actor_d::call_slot(self, 184, state); }

inline bool zone(unsigned char* o, float height, short distance) { return enemyInZone__8Creaturefs(o, field<float>(o, 28) + height, distance) != 0; }

inline void init_description(ItemDescription& d) {
    for (unsigned i = 36; i <= 60; i += 8) *reinterpret_cast<unsigned short*>(d.bytes + i) = 0xffffu;
    d.bytes[32] = 0xff;
    d.bytes[33] = 0xff;
    for (unsigned i = 16; i < 32; i += 2) *reinterpret_cast<unsigned short*>(d.bytes + i) = 0;
}

inline void melee(unsigned char* o) {
    ItemDescription d;
    init_description(d);
    if (static_cast<unsigned>(eRand__10GameObject(o)) % 10u == 0) {
        unsigned char* skill = w10_actor_d::word_at(skills, 104);
        const unsigned char* entry = w10_actor_d::word_at(skill, 96) + 96;
        const short adjustment = *reinterpret_cast<const short*>(entry);
        const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W10_ACTOR_D_HOST)
        const int value = w10_actor_d_skill_function(function, skill + adjustment, 0, static_cast<int>(worldDifficulty));
#else
        const int value = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(skill + adjustment, 0, static_cast<int>(worldDifficulty));
#endif
        for (int i = 0; i < 4; ++i) {
            unsigned short& slot = *reinterpret_cast<unsigned short*>(d.bytes + 36 + 8 * i);
            if (slot == 0xffffu || slot == 23u) {
                slot = 23;
                *reinterpret_cast<unsigned short*>(d.bytes + 42 + 8 * i) = 0;
                *reinterpret_cast<short*>(d.bytes + 40 + 8 * i) = static_cast<short>(value);
                *reinterpret_cast<short*>(d.bytes + 38 + 8 * i) = static_cast<short>(value);
                break;
            }
        }
    }
    produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, 36.0f, field<short>(o, 200), field<short>(o, 202), field<short>(o, 204), 0x10008, &d);
}

inline bool bit8(unsigned char* o) { return (field<unsigned short>(o, 294) & 8u) != 0; }

inline void fire(unsigned char* o, const Point3& target) {
    const short heading = field<short>(o, 56);
    Point3 spawn;
    spawn.x = field<float>(o, 16) + icos__Fi(field<short>(o, 56)) * 10.0f;
    spawn.y = field<float>(o, 20) + isin__Fi(field<short>(o, 56)) * 10.0f;
    spawn.z = field<float>(o, 24) + 24.0f;
    (void)heading;
    short angle = static_cast<short>(16384 - iatan2__Fff(target.x - field<float>(o, 16), target.y - field<float>(o, 20)));
    const int current = field<unsigned short>(o, 56);
    const short difference = static_cast<short>(angle - current);
    if (!(difference < 4001)) angle = static_cast<short>(current + 4000);
    else if (difference < -4000) angle = static_cast<short>(current - 4000);
    angle = static_cast<short>(angle + static_cast<int>(static_cast<unsigned>(eRand__10GameObject(o)) % 2000u) - 1000);
    Point3 velocity;
    velocity.x = icos__Fi(angle) * 400.0f;
    velocity.y = isin__Fi(angle) * 400.0f;
    velocity.z = 0.0f;
    const float dx = field<float>(o, 16) - target.x;
    const float dy = field<float>(o, 20) - target.y;
    const float dz = field<float>(o, 24) - target.z;
    float speed = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    speed = speed * 0.0025f;
    const int roll = eRand__10GameObject(o);
    velocity.z = (static_cast<float>(static_cast<unsigned>(roll) % 30u) + 80.0f) * speed;
    void* projectile = blockAlloc__Fi(168);
    if (projectile != nullptr) {
        const float scale = static_cast<float>(eRand__10GameObject(o)) * 1.52587890625e-05f * 0.5f + 0.5f;
        poisonProjectile_construct(projectile, w10_actor_d::word_at(o, 36), 21, 22, 23, 25, &spawn, &velocity, field<short>(o, 56) - 16384, scale, o);
    }
}
}  // namespace

extern "C" void msg_run__6Cthulu(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, w10_actor_d::word_at(w10_actor_d::word_at(o, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, w10_actor_d::word_at(w10_actor_d::word_at(o, 36), 12)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int slide = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.5f);
    w10_actor_d::call_slot(self, 168, 0);
    const int state = field<short>(o, 572);
    if (static_cast<unsigned>(state) > 16u) return;
    switch (state) {
        case 0:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) set_state(self, 3);
            else if (field<int>(o, 256) < field<int>(o, 808)) set_state(self, 1);
            return;
        case 1:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) set_state(self, 3);
            else if (field<int>(o, 256) < field<int>(o, 808)) set_state(self, 0);
            else smartWander__8Creature(self);
            return;
        case 3: {
            if (field<int>(o, 576) == 0) {
                set_state(self, 0);
                return;
            }
            const bool near = zone(o, 40.0f, 5000);
            if (field<int>(o, 964) != 0 && near) {
                set_state(self, 6);
                return;
            }
            if (field<int>(o, 964) == 0) {
                if (zone(o, 40.0f, 5000)) {
                    const unsigned short pick = static_cast<unsigned short>(static_cast<unsigned>(eRand__10GameObject(self)) % 3u);
                    if (pick == 0) set_state(self, 6);
                    else if (pick == 1) set_state(self, 13);
                    else set_state(self, 14);
                    return;
                }
                if (field<int>(o, 972) == 0 && zone(o, 100.0f, 5000)) {
                    set_state(self, 15);
                    return;
                }
            }
            if (field<int>(o, 968) < field<int>(o, 808) && zone(o, 300.0f, 5000)) {
                set_state(self, 16);
                return;
            }
            if (zone(o, 40.0f, 32767)) turnToFaceEnemy__8Creature(self);
            else steerAndRouteToEnemy__8Creature(self);
            return;
        }
        case 5:
            if (zone(o, 40.0f, 5000)) set_state(self, 6);
            else if (zone(o, 300.0f, 5000)) set_state(self, 16);
            else if (bExitStatePersueIdle__8Creature(self) != 0) set_state(self, 3);
            return;
        case 6:
        case 13:
        case 14:
        case 15: {
            if (state == 6 || state == 15) {
                const Point3 zero = {0.0f, 0.0f, 0.0f};
                move2__8CreatureG6Point3(self, &zero);
            }
            if (field<int>(o, 576) != 0) {
                if ((slide & 1) != 0) melee(o);
                if (bit8(o)) turnToFaceEnemy__8Creature(self);
            }
            if (bit8(o)) return;
            if (state == 6 && field<int>(o, 964) != 0) {
                eRand__10GameObject(self);
                field<int>(o, 960) = 0;
            }
            set_state(self, 3);
            return;
        }
        case 16: {
            const Point3 zero = {0.0f, 0.0f, 0.0f};
            move2__8CreatureG6Point3(self, &zero);
            if (field<int>(o, 576) != 0) {
                const float c = icos__Fi(field<short>(o, 56));
                const float s = isin__Fi(field<short>(o, 56));
                Point3 target = {field<float>(o, 16) + c * 300.0f, field<float>(o, 20) + s * 300.0f, field<float>(o, 24)};
                findEnemyLeadPosition__8CreaturefP6Point3(self, 400.0f, &target);
                if ((slide & 1) != 0) fire(o, target);
                if (bit8(o)) turnToFaceEnemy__8Creature(self);
            }
            if (bit8(o)) return;
            set_state(self, 3);
            return;
        }
        case 8:
            if ((field<unsigned short>(o, 294) & 0x10u) == 0) set_state(self, 0);
            return;
        case 9:
            if (field<int>(o, 804) < field<int>(o, 808)) set_state(self, 3);
            return;
        case 10:
            if ((field<unsigned short>(o, 294) & 0x80u) != 0) objectRemoveFromRunList__FP10GameObject(self);
            if ((slide & 4) != 0 && (field<unsigned>(o, 12) & 2u) == 0) {
                const int first = eRand__10GameObject(self);
                const int second = eRand__10GameObject(self);
                const Point3 where = field<Point3>(o, 16);
                floorAdd__FG6Point3sfii(&where, static_cast<short>(first), static_cast<float>(static_cast<int>(static_cast<unsigned>(second) % 10u) + 30), 3, 1);
            }
            return;
        default:
            return;
    }
}
