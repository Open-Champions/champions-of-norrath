// Observed msg_run__10CloudGiant at 0x001e70b0, 7304 bytes.
// Notation: o is the object, m the pointer word at o+36, w(k) the int word at o+k, A the
// AnimationState at o+288, setState(n) the call of virtual slot byte 184 of the table at o+72 with
// the int n, bit8 the bit 8 of the halfword +294 and h the signed halfword +56.
// Order: (1) two weapon-effect calls, (2) the common entry, (3) a case by state, (4) a common tail.
// (1) If w(984) is nonzero and the signed byte cutMode is zero: fxRunWeapon(o+1056, word m+196, o+988,
//     copy of the Point3 at +16, h, word m+12, A, 0.5, state == 6, 1, float +224, null). If w(980) is
//     nonzero and cutMode is zero: fxRunWeapon(o+1192, word m+180, o+1128, copy of the Point3 at +16,
//     h, word m+12, A, 1.0, state is 15 or 17, 0, float +224, null).
// (2) newDamageHandler(o, word m+12); everyFrame(o, word m+12) nonzero ends the function; when w(976)
//     is nonzero objectActivationCheck(o, 1200) zero ends it; objectActivationCheck(o, 600) zero ends
//     it; S = creatureAdvanceAnimation(o, 1.0) (the image keeps this result, not slideBack's, as the
//     flag word); slideBack(o, 0.0); virtual slot byte 168 is called (no second argument).
// (3) The signed halfword +572 picks the case (unsigned compare against 19; 2, 4, 7, 11, 12 and
//     larger values only run the tail). zone(h, d) = enemyInZone(o, float +28 + h, d).
//   (Where a call is written enemyInZone(o, h, d) below, h is passed unchanged, without the +28 float.)
//   0: enemyInZone(o, 400.0, 32767) -> setState(3); else w(256) < w(808) -> setState(1).
//   1: enemyInZone(o, 400.0, 32767) -> setState(3); else w(256) < w(808) -> setState(0); else
//      smartWander(o).
//   3: w(576) zero: setState(0). Else if w(976) nonzero and w(980) zero and w(1304) zero:
//      setState(18). Else if zone(30, 3000): setState(16 if (eRand & 3) == 0 else 6). Else if
//      w(960) < w(808): r = eRand % 3; r 0 and enemyInZone(o, 250.0, 3000) -> next 13; r 1 and not
//      enemyInZone(o, 200.0, 3000) and enemyInZone2(o, 0, 500, h, 3000, 0.0, 1) -> next 15; r 2 -> next 17 if
//      w(980) else 14; a chosen next does setState(next) and w(960) = w(808) + 360. Else
//      zone(30, 32767) ? turnToFaceEnemy(o) : steerAndRouteToEnemy(o).
//   5: if zone(30, 3000): setState(16 if eRand % 5 == 0 else 6). Else if w(960) < w(808): r = eRand %
//      3; r 0 and enemyInZone(o, 250.0, 3000) -> 13; r 1 and not enemyInZone(o, 200.0, 3000) and enemyInZone(o, 500.0, 3000) -> 15; r 2
//      -> 17 if w(980) else 14; next does setState(next), w(960) = w(808) + 480. Else
//      bExitStatePersueIdle(o) -> setState(3).
//   6: move2(o, zero Point3). With w(576) nonzero: if S bit 0: description D (as in msg_poke__5Chesti);
//      when w(208) is 1: val1 = virtual slot byte 96 of the object whose word is at skills+48
//      (called with (object + adjustment, 0, (int)worldDifficulty)), val2 = val1 + (int)((float)val1 *
//      0.2 * (eRand * 2^-16)); the first slot among D+36+8i that holds -1 or 24 becomes 24, with
//      halfwords D+38+8i = val1, D+40+8i = val2, D+42+8i = 0; produceMeleeDamage(o, 36.0, (short)+200,
//      (short)+202, (short)+204, 8, &D). If bit8: turnToFaceEnemy(o). Then bit8 stops; else
//      setState(3).
//   8: move2(o, zero Point3); unless bit 4 of the halfword +294 is set: setState(0).
//   9: w(804) < w(808) -> setState(3).
//   10: bit 7 of the halfword +294 -> soundFadeStream(0) and objectRemoveFromRunList(o). If S bit 0
//      and bit 1 of w(12) clear: floorAdd(copy of the Point3 at +16 moved by 50*(icos(h), isin(h)),
//      (short)eRand1, (float)(eRand2 % 20 + 50), 0, 1). If S bit 1 and bit 1 of w(12) clear: bit 0 of
//      w(12) is set.
//   13: move2(o, zero Point3). If S bit 0: w(1308) = soundPlaySequence(word m+260, o, 1.0) and byte
//      +964 = 1. When byte +964 is nonzero: attachment points P1 (out 1) and P2 (out 2) from
//      modelGetCharAttachmentPos(word m+12, copy of the Point3 at +16, h, A, 2, &P1, &P2, float +224);
//      P2 *= 100; 16 times a breath particle (definition at 0x7d6c40) at 4*unitBall + P1 with velocity
//      30*unitBall + P2; every time w(808) & 31 == 0 the objects in a 250-wide box (height +-float
//      +32) are searched (objectFindInBox, 128 entries): for each one that the creature's virtual
//      slot 160 accepts and that is within pointInZone(o, its Point3, 0, 250, h, 3000), a
//      description with slot 22 carrying w = skill-at-skills+56(...)/5 on both values, then
//      gameAttackResolver(obj, o, (short)+200/10, (short)+202/10, 100000, 0, 0, 0x40440, &D, 0);
//      w(968) = 0 when none was accepted. Finally, when w(968) < w(808): soundStopSound(w(1308)),
//      w(1308) = -1, setState(3).
//   14: move2; with S bit 0: gameSetCameraShake(0, 6.0); every object of a 256-wide box with flags 0x20
//      set and 8 clear and a type other than CloudGiant gets gameAttackResolver(obj, o,
//      (short)+200/2, (short)+202/2, 100000, 255, 0, 0x4000000, null, 0) and its virtual slot byte
//      208 called with 60. Then bit8 stops; else setState(3).
//   15: move2; with w(576) nonzero: if S bit 0, either the beam (w(980) nonzero) or the missile (w(980)
//      zero) is fired; turnToFaceEnemy(o); then bit8 stops, else setState(3). Without w(576): the
//      same tail test only.
//   16: move2; with w(576): S bit 0 -> produceMeleeDamage(o, 36.0, (short)+200, (short)+202,
//      (short)+204, 0x100041, null); bit8 -> turnToFaceEnemy(o). bit8 stops; else setState(3).
//   17: with S bit 0: a 156-byte BoltGoRound at (x + 50*icos(h-5000), y + 50*isin(h-5000), ground
//      height from worldFindHeight(world, Point3, 6.0)), (short)h, 240, word m+244; w(980) = 0,
//      w(1304) = 0, w(972) = w(808) + 240. bit8 stops, else setState(3).
//   18: with S bit 0: w(980) = 1, halfwords +1208 = 150, +1210 = 150, +1212 = 200, +1214 = 30 and
//      soundPlaySequence(word m+228, o, 1.0). bit8 stops, else setState(3).
// (4) Tail: with w(980) nonzero the weapon's lightning bolt follows the attachment matrix; with it
//     zero and w(1264) nonzero the bolt flies, hits and ends (see the code).
// Inferred: the CloudGiant boss body. Unknown: field and state names, most constants' meaning,
// which slot is "isEnemy", and the layout of the objects behind the descriptions. The two particle
// and weapon definitions are statics of its translation unit. Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

#if defined(W10_ACTOR_D_HOST)
extern int w10_actor_d_skill_function(unsigned function, void* skill, int a, int b);
extern const void* w10_actor_d_type_slot(unsigned function);
extern int w10_actor_d_slot_call(unsigned function, void* object, int argument);
#endif

struct Point3 {
    float x, y, z;
};
struct Matrix34 {
    float m[12];
};

extern "C" {
void fxRunWeapon(void* description, void* model, void* weaponState, const Point3* position, short heading, void* model2, void* state, float a, int b, int c, float d, void* matrix)
    __asm__("fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34");
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float value);
int slideBack__8Creaturef(void* self, float value);
int enemyInZone__8Creaturefs(void* self, float height, short distance);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short c, short d, float e, int f);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void move2__8CreatureG6Point3(void* self, const Point3* point);
int eRand__10GameObject(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float a, int b, int c, int d, int e, void* description);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const Point3* point, float value);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float a, float b);
void* blockAlloc__Fi(int size);
void boltGoRound_construct(void* self, const Point3* point, short heading, int a, void* header) __asm__("__11BoltGoRoundG6Point3siP15AnimationHeader");
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void soundStopSound__Fi(int value);
void soundFadeStream__Fi(int value);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, const Point3* position, short type, void* state, int a, Point3* out, Point3* second, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float value);
void objectFindInBox__FG6Point3T0PP10GameObjecti(const Point3* lower, const Point3* upper, unsigned* list, int count);
int pointInZone__8CreatureG6Point3ffss(void* self, const Point3* point, float a, float b, short c, short d);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* target, void* attacker, float a, float b, int c, int d, int e, int f, void* description, bool g);
void gameSetCameraShake__Ffb(float amount, bool flag);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float value, Point3* point);
int P_StartLightning__FG6Point3T0iiffUi(const Point3* from, const Point3* to, int a, int b, float c, float d, unsigned color);
void P_MoveLightning__FiG6Point3T1ii(int handle, const Point3* from, const Point3* to, int a, int b);
void missileWeapon_construct(void* self, int a, void* model, void* texture, const Point3* position, const Point3* velocity, int angle, void* creature, void* vag1, void* vag2,
                             int i1, int i2, int i3, int i4, int i5, int i6, void* description)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const Point3* position, short a, float b, int c, int d);
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* model, void* state, int a, Matrix34* matrix, float scale);
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* matrix, const Point3* point, short yaw);
void matMul__FP8Matrix34N20(Matrix34* destination, Matrix34* left, Matrix34* right);
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(void* model, Matrix34* matrix, int a, Point3* out, float* second);
int worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void* world, const Point3* from, const Point3* direction, float length, void* ignore, unsigned* hit, int a, int b);
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
const void* __tf10CloudGiant();
extern signed char cutMode;
extern float worldDifficulty;
extern unsigned world;
extern unsigned char skills[];
extern unsigned char norrath_w10_actor_d_view_7d6c40[80];
}

namespace {
template <typename T>
inline __attribute__((always_inline)) T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }

inline __attribute__((always_inline)) unsigned char* word_at(const unsigned char* base, unsigned offset) { return w10_actor_d::word_at(base, offset); }
inline __attribute__((always_inline)) unsigned char* block(unsigned char* o) { return word_at(o, 36); }
inline __attribute__((always_inline)) void set_state(void* self, int state) { w10_actor_d::call_slot(self, 184, state); }
inline __attribute__((always_inline)) bool zone(unsigned char* o, float height, short distance) { return enemyInZone__8Creaturefs(o, field<float>(o, 28) + height, distance) != 0; }
inline __attribute__((always_inline)) bool bit8(unsigned char* o) { return (field<unsigned short>(o, 294) & 8u) != 0; }
inline __attribute__((always_inline)) void* game_world() { return w10_actor_d::from_word(world); }

struct ItemDescription {
    unsigned char bytes[80];
};

inline __attribute__((always_inline)) void init_description(ItemDescription& d) {
    for (unsigned i = 36; i <= 60; i += 8) *reinterpret_cast<unsigned short*>(d.bytes + i) = 0xffffu;
    d.bytes[32] = 0xff;
    d.bytes[33] = 0xff;
    for (unsigned i = 16; i < 32; i += 2) *reinterpret_cast<unsigned short*>(d.bytes + i) = 0;
}

// Sets the first of the four slots holding -1 or marker to marker with the given values.
inline __attribute__((always_inline)) void put_slot(ItemDescription& d, unsigned short marker, short first, short second) {
    for (int i = 0; i < 4; ++i) {
        unsigned short& slot = *reinterpret_cast<unsigned short*>(d.bytes + 36 + 8 * i);
        if (slot == 0xffffu || slot == marker) {
            slot = marker;
            *reinterpret_cast<unsigned short*>(d.bytes + 42 + 8 * i) = 0;
            *reinterpret_cast<short*>(d.bytes + 38 + 8 * i) = first;
            *reinterpret_cast<short*>(d.bytes + 40 + 8 * i) = second;
            return;
        }
    }
}

// The skill object is the pointer word at skills + offset; its table pointer is at +96 and the
// call is made through the slot at table byte 96: (object + adjustment, 0, (int)worldDifficulty).
inline __attribute__((always_inline)) int skill_value(unsigned offset) {
    unsigned char* skill = word_at(skills, offset);
    const unsigned char* entry = word_at(skill, 96) + 96;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_skill_function(function, skill + adjustment, 0, static_cast<int>(worldDifficulty));
#else
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(skill + adjustment, 0, static_cast<int>(worldDifficulty));
#endif
}

inline __attribute__((always_inline)) const void* dynamic_type(unsigned char* object) {
    if (object == nullptr) __throw_bad_typeid();
    const unsigned char* table = word_at(object, 72);
    const unsigned function = *reinterpret_cast<const unsigned*>(table + 4);
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_type_slot(function);
#else
    return reinterpret_cast<const void* (*)()>(static_cast<__UINTPTR_TYPE__>(function))();
#endif
}

inline __attribute__((always_inline)) void unit_ball(unsigned char* o, float* v) {
    do {
        float t = static_cast<float>(eRand__10GameObject(o)) * 1.52587890625e-05f;
        v[0] = t + t - 1.0f;
        t = static_cast<float>(eRand__10GameObject(o)) * 1.52587890625e-05f;
        v[1] = t + t - 1.0f;
        t = static_cast<float>(eRand__10GameObject(o)) * 1.52587890625e-05f;
        v[2] = t + t - 1.0f;
    } while (1.0f < v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline __attribute__((always_inline)) float rsqrt_one(float length_squared) {
#if defined(__mips__)
    float result;
    const float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);  // host stand-in for rsqrt.s
#endif
}

inline __attribute__((always_inline)) void zero_move(void* self) {
    const Point3 zero = {0.0f, 0.0f, 0.0f};
    move2__8CreatureG6Point3(self, &zero);
}

inline __attribute__((always_inline)) short clamp_heading(unsigned char* o, short wanted) {
    const int current = field<unsigned short>(o, 56);
    const short difference = static_cast<short>(wanted - current);
    if (!(difference < 4001)) return static_cast<short>(current + 4000);
    if (difference < -4000) return static_cast<short>(current - 4000);
    return wanted;
}

// Common ending of the cases that fall back to state 3 when bit8 is clear.
inline __attribute__((always_inline)) void finish(void* self, unsigned char* o) {
    if (!bit8(o)) set_state(self, 3);
}
}  // namespace


namespace {
// Virtual slot at table byte 160 of the object: returns an int and takes the other object's word.
inline __attribute__((always_inline)) int slot_with_result(void* self, unsigned slot_byte_offset, unsigned char* argument) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* entry = word_at(object, 72) + slot_byte_offset;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_slot_call(function, object + adjustment, static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(argument) - reinterpret_cast<__UINTPTR_TYPE__>(w10_actor_d_host_base)));
#else
    return reinterpret_cast<int (*)(void*, unsigned char*)>(static_cast<__UINTPTR_TYPE__>(function))(object + adjustment, argument);
#endif
}

inline __attribute__((always_inline)) void tail(void* self, unsigned char* o) {
    if (field<int>(o, 980) != 0) {
        Matrix34 matrix;
        modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(word_at(block(o), 12), o + 288, 0, &matrix, field<float>(o, 224));
        Point3* const first = reinterpret_cast<Point3*>(o + 1268);
        Point3* const second = reinterpret_cast<Point3*>(o + 1280);
        const Point3 position = field<Point3>(o, 16);
        Matrix34 turn;
        matMakeTransYaw__FP8Matrix34G6Point3s(&turn, &position, static_cast<short>(field<unsigned short>(o, 56) + 16384));
        matMul__FP8Matrix34N20(&matrix, &matrix, &turn);
        float scratch[3];
        modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(word_at(block(o), 180), &matrix, 0, first, scratch);
        modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(word_at(block(o), 180), &matrix, 80, second, scratch);
        int handle = field<int>(o, 1304);
        if (handle == 0) {
            const Point3 a = *first;
            const Point3 b = *second;
            handle = P_StartLightning__FG6Point3T0iiffUi(&a, &b, 200, 10, 4.0f, 4.5f, 0x5e808080u);
            field<int>(o, 1304) = handle;
        }
        const Point3 a = *first;
        const Point3 b = *second;
        P_MoveLightning__FiG6Point3T1ii(handle, &a, &b, 94, -1);
        return;
    }
    if (field<int>(o, 1264) == 0) return;
    Point3* const first = reinterpret_cast<Point3*>(o + 1268);
    Point3* const second = reinterpret_cast<Point3*>(o + 1280);
    const Point3 direction = {second->x - first->x, second->y - first->y, second->z - first->z};
    unsigned hit = 0;
    const Point3 start = *first;
    worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(game_world(), &start, &direction, 96.0f, self, &hit, 0, 0);
    if (hit != 0) {
        unsigned char* target = w10_actor_d::from_word(hit);
        if ((field<unsigned>(target, 12) & 0x48u) == 0x40u) {
            ItemDescription d;
            init_description(d);
            put_slot(d, 24, field<short>(o, 200), field<short>(o, 202));
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(target, nullptr, 0.0f, 0.0f, field<short>(o, 204), 0, 0, 0x40, &d, false);
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(word_at(block(o), 236), self, 1.0f);
        }
    }
    const float* velocity = reinterpret_cast<const float*>(o + 1292);
    const Point3 oldSecond = *second;
    first->x = first->x + velocity[0];
    first->y = first->y + velocity[1];
    first->z = first->z + velocity[2];
    const float dx = first->x - oldSecond.x;
    const float dy = first->y - oldSecond.y;
    const float dz = first->z - oldSecond.z;
    if (96.0f < __builtin_sqrtf(dx * dx + dy * dy + dz * dz)) {
        second->x = oldSecond.x + velocity[0];
        second->y = oldSecond.y + velocity[1];
        second->z = oldSecond.z + velocity[2];
    }
    {
        const Point3 a = *first;
        const Point3 b = *second;
        P_MoveLightning__FiG6Point3T1ii(field<int>(o, 1304), &a, &b, 94, -1);
    }
    const Point3* position = reinterpret_cast<const Point3*>(o + 16);
    const float ex = first->x - position->x;
    const float ey = first->y - position->y;
    const float ez = first->z - position->z;
    if (1000.0f < __builtin_sqrtf(ex * ex + ey * ey + ez * ez)) {
        field<int>(o, 1264) = 0;
        field<int>(o, 1304) = 0;
    }
    const Point3 probe = *second;
    const float ground = worldFindHeight__FP12_worldHeaderG6Point3f(game_world(), &probe, 6.0f);
    if (field<float>(o, 1288) < ground) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(word_at(block(o), 236), self, 1.0f);
        field<int>(o, 1304) = 0;
        field<int>(o, 1264) = 0;
    }
}

inline __attribute__((always_inline)) void next_attack(void* self, unsigned char* o, int next, int pause) {
    if (next == 0) return;
    set_state(self, next);
    field<int>(o, 960) = field<int>(o, 808) + pause;
}
}  // namespace

extern "C" void msg_run__10CloudGiant(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (field<int>(o, 984) != 0 && cutMode == 0) {
        const Point3 position = field<Point3>(o, 16);
        fxRunWeapon(o + 1056, word_at(block(o), 196), o + 988, &position, field<short>(o, 56), word_at(block(o), 12), o + 288, 0.5f,
                    field<short>(o, 572) == 6 ? 1 : 0, 1, field<float>(o, 224), nullptr);
    }
    if (field<int>(o, 980) != 0 && cutMode == 0) {
        const Point3 position = field<Point3>(o, 16);
        const short now = field<short>(o, 572);
        fxRunWeapon(o + 1192, word_at(block(o), 180), o + 1128, &position, field<short>(o, 56), word_at(block(o), 12), o + 288, 1.0f,
                    (now == 17 || now == 15) ? 1 : 0, 0, field<float>(o, 224), nullptr);
    }
    newDamageHandler__8CreatureP12_modelHeader(self, word_at(block(o), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, word_at(block(o), 12)) != 0) return;
    if (field<int>(o, 976) != 0) {
        if (objectActivationCheck__8Creaturei(self, 1200) == 0) return;
    }
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int S = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.0f);
    w10_actor_d::call_slot(self, 168, 0);
    const int state = field<short>(o, 572);
    switch (static_cast<unsigned>(state) < 19u ? state : -1) {
        case 0:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) set_state(self, 3);
            else if (field<int>(o, 256) < field<int>(o, 808)) set_state(self, 1);
            break;
        case 1:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) set_state(self, 3);
            else if (field<int>(o, 256) < field<int>(o, 808)) set_state(self, 0);
            else smartWander__8Creature(self);
            break;
        case 3:
            if (field<int>(o, 576) == 0) {
                set_state(self, 0);
            } else if (field<int>(o, 976) != 0 && field<int>(o, 980) == 0 && field<int>(o, 1304) == 0) {
                set_state(self, 18);
            } else if (zone(o, 30.0f, 3000)) {
                set_state(self, (static_cast<unsigned>(eRand__10GameObject(self)) & 3u) == 0 ? 16 : 6);
            } else if (field<int>(o, 960) < field<int>(o, 808)) {
                const unsigned pick = static_cast<unsigned short>(static_cast<unsigned>(eRand__10GameObject(self)) % 3u);
                int next = 0;
                if (pick == 0) {
                    if (enemyInZone__8Creaturefs(o, 250.0f, 3000) != 0) next = 13;
                } else if (pick == 1) {
                    if (enemyInZone__8Creaturefs(o, 200.0f, 3000) == 0 && enemyInZone2__8Creatureffssfi(self, 0.0f, 500.0f, field<short>(o, 56), 3000, 0.0f, 1) != 0) next = 15;
                } else if (pick == 2) {
                    next = field<int>(o, 980) != 0 ? 17 : 14;
                }
                next_attack(self, o, next, 360);
            } else if (zone(o, 30.0f, 32767)) {
                turnToFaceEnemy__8Creature(self);
            } else {
                steerAndRouteToEnemy__8Creature(self);
            }
            break;
        case 5:
            if (zone(o, 30.0f, 3000)) {
                set_state(self, static_cast<unsigned>(eRand__10GameObject(self)) % 5u == 0 ? 16 : 6);
            } else if (field<int>(o, 960) < field<int>(o, 808)) {
                const unsigned pick = static_cast<unsigned short>(static_cast<unsigned>(eRand__10GameObject(self)) % 3u);
                int next = 0;
                if (pick == 0) {
                    if (enemyInZone__8Creaturefs(o, 250.0f, 3000) != 0) next = 13;
                } else if (pick == 1) {
                    if (enemyInZone__8Creaturefs(o, 200.0f, 3000) == 0 && enemyInZone__8Creaturefs(o, 500.0f, 3000) != 0) next = 15;
                } else if (pick == 2) {
                    next = field<int>(o, 980) != 0 ? 17 : 14;
                }
                next_attack(self, o, next, 480);
            } else if (bExitStatePersueIdle__8Creature(self) != 0) {
                set_state(self, 3);
            }
            break;
        case 6:
            zero_move(self);
            if (field<int>(o, 576) != 0) {
                if ((S & 1) != 0) {
                    ItemDescription d;
                    init_description(d);
                    if (field<int>(o, 208) == 1) {
                        const int first = skill_value(48);
                        float scaled = static_cast<float>(first) * 0.2f;
                        scaled = scaled * (static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f);
                        put_slot(d, 24, static_cast<short>(first), static_cast<short>(first + static_cast<int>(scaled)));
                    }
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, field<short>(o, 200), field<short>(o, 202), field<short>(o, 204), 8, &d);
                }
                if (bit8(o)) turnToFaceEnemy__8Creature(self);
            }
            finish(self, o);
            break;
        case 8:
            zero_move(self);
            if ((field<unsigned short>(o, 294) & 0x10u) == 0) set_state(self, 0);
            break;
        case 9:
            if (field<int>(o, 804) < field<int>(o, 808)) set_state(self, 3);
            break;
        case 10:
            if ((field<unsigned short>(o, 294) & 0x80u) != 0) {
                soundFadeStream__Fi(0);
                objectRemoveFromRunList__FP10GameObject(self);
            }
            if ((S & 1) != 0 && (field<unsigned>(o, 12) & 2u) == 0) {
                Point3 where = field<Point3>(o, 16);
                where.x = where.x + icos__Fi(field<short>(o, 56)) * 50.0f;
                where.y = field<float>(o, 20) + isin__Fi(field<short>(o, 56)) * 50.0f;
                where.z = field<float>(o, 24);
                const int first = eRand__10GameObject(self);
                const int second = eRand__10GameObject(self);
                floorAdd__FG6Point3sfii(&where, static_cast<short>(first), static_cast<float>(static_cast<int>(static_cast<unsigned>(second) % 20u) + 50), 0, 1);
            }
            if ((S & 2) != 0 && (field<unsigned>(o, 12) & 2u) == 0) field<unsigned>(o, 12) = field<unsigned>(o, 12) | 1u;
            break;
        case 13: {
            zero_move(self);
            if ((S & 1) != 0) {
                field<int>(o, 1308) = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(word_at(block(o), 260), self, 1.0f);
                o[964] = 1;
            }
            if (static_cast<signed char>(o[964]) != 0) {
                Point3 outer;
                Point3 inner;
                const Point3 position = field<Point3>(o, 16);
                modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(word_at(block(o), 12), &position, field<short>(o, 56), o + 288, 2, &inner, &outer, field<float>(o, 224));
                outer.x = outer.x * 100.0f;
                outer.y = outer.y * 100.0f;
                outer.z = outer.z * 100.0f;
                for (int i = 0; i < 16; ++i) {
                    float v[3];
                    unit_ball(o, v);
                    v[0] = v[0] * 4.0f;
                    v[1] = v[1] * 4.0f;
                    v[2] = v[2] * 4.0f;
                    v[0] = v[0] + inner.x;
                    v[1] = v[1] + inner.y;
                    v[2] = v[2] + inner.z;
                    float w[3];
                    unit_ball(o, w);
                    w[0] = w[0] * 30.0f;
                    w[1] = w[1] * 30.0f;
                    w[2] = w[2] * 30.0f;
                    w[0] = w[0] + outer.x;
                    w[1] = w[1] + outer.y;
                    w[2] = w[2] + outer.z;
                    P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w10_actor_d_view_7d6c40, reinterpret_cast<Point3*>(v), reinterpret_cast<Point3*>(w), 0.0f);
                }
                bool anyone = false;
                if ((field<int>(o, 808) & 0x1f) == 0) {
                    const float x = field<float>(o, 16);
                    const float y = field<float>(o, 20);
                    const float z = field<float>(o, 24);
                    const float height = field<float>(o, 32);
                    const Point3 upper = {x + 250.0f, y + 250.0f, z + height};
                    const Point3 lower = {x - 250.0f, y - 250.0f, z - height};
                    unsigned list[130];
                    objectFindInBox__FG6Point3T0PP10GameObjecti(&lower, &upper, list, 128);
                    if (list[0] != 0) {
                        for (int i = 0; list[i] != 0; ++i) {
                            unsigned char* other = w10_actor_d::from_word(list[i]);
                            if (slot_with_result(self, 160, other) == 0) continue;
                            const Point3 where = field<Point3>(other, 16);
                            if (pointInZone__8CreatureG6Point3ffss(self, &where, 0.0f, 250.0f, field<short>(o, 56), 3000) == 0) continue;
                            anyone = true;
                            ItemDescription d;
                            init_description(d);
                            const int amount = skill_value(56) / 5;
                            put_slot(d, 22, static_cast<short>(amount), static_cast<short>(amount));
                            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(other, self, static_cast<float>(field<short>(o, 200) / 10), static_cast<float>(field<short>(o, 202) / 10),
                                                                                              100000, 0, 0, 0x40440, &d, false);
                        }
                    }
                    if (!anyone) field<int>(o, 968) = 0;
                }
            }
            if (field<int>(o, 968) < field<int>(o, 808)) {
                soundStopSound__Fi(field<int>(o, 1308));
                field<int>(o, 1308) = -1;
                set_state(self, 3);
            }
            break;
        }
        case 14: {
            zero_move(self);
            if ((S & 1) != 0) {
                gameSetCameraShake__Ffb(6.0f, false);
                const float x = field<float>(o, 16);
                const float y = field<float>(o, 20);
                const float z = field<float>(o, 24);
                const Point3 upper = {x + 256.0f, y + 256.0f, z + 256.0f};
                const Point3 lower = {x - 256.0f, y - 256.0f, z - 256.0f};
                unsigned list[130];
                objectFindInBox__FG6Point3T0PP10GameObjecti(&lower, &upper, list, 128);
                if (list[0] != 0) {
                    for (int i = 0; list[i] != 0; ++i) {
                        unsigned char* other = w10_actor_d::from_word(list[i]);
                        const unsigned flags = field<unsigned>(other, 12);
                        if ((flags & 0x20u) == 0) continue;
                        if ((flags & 8u) != 0) continue;
                        const void* dynamic = dynamic_type(other);
                        const void* giant = __tf10CloudGiant();
                        if (__eq__C9type_infoRC9type_info(dynamic, giant) != 0) continue;
                        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(other, self, static_cast<float>(field<short>(o, 200) / 2), static_cast<float>(field<short>(o, 202) / 2),
                                                                                          100000, 255, 0, 0x4000000, nullptr, false);
                        w10_actor_d::call_slot(other, 208, 60);
                    }
                }
            }
            finish(self, o);
            break;
        }
        case 15: {
            zero_move(self);
            if (field<int>(o, 576) != 0) {
                if ((S & 1) != 0) {
                    if (field<int>(o, 980) != 0) {
                        const float c = icos__Fi(field<short>(o, 56));
                        const float s = isin__Fi(field<short>(o, 56));
                        const Point3 direction = {c, s, 0.0f};
                        Point3 target = {field<float>(o, 16) + direction.x * 300.0f, field<float>(o, 20) + s * 300.0f, field<float>(o, 24)};
                        findEnemyLeadPosition__8CreaturefP6Point3(self, 300.0f, &target);
                        Point3 out;
                        const Point3 position = field<Point3>(o, 16);
                        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(word_at(block(o), 12), &position, field<short>(o, 56), o + 288, 0, &out, nullptr, field<float>(o, 224));
                        Point3 beam = {target.x - out.x, target.y - out.y, target.z + 36.0f - out.z};
                        field<Point3>(o, 1292) = beam;
                        float* v = reinterpret_cast<float*>(o + 1292);
                        const float length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
                        const float scale = rsqrt_one(length);
                        const float z = v[2] * scale;
                        const float x = v[0] * scale;
                        const float y = v[1] * scale;
                        v[2] = z * 12.0f;
                        v[0] = x * 12.0f;
                        v[1] = y * 12.0f;
                        field<Point3>(o, 1268) = out;
                        field<Point3>(o, 1280) = out;
                        const Point3 a = field<Point3>(o, 1268);
                        const Point3 b = field<Point3>(o, 1280);
                        field<int>(o, 1304) = P_StartLightning__FG6Point3T0iiffUi(&a, &b, 200, 10, 8.0f, 16.5f, 0x5e808080u);
                        field<int>(o, 1264) = 1;
                        field<int>(o, 980) = 0;
                        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(word_at(block(o), 252), self, 1.0f);
                    } else {
                        const float c = icos__Fi(field<short>(o, 56));
                        const float s = isin__Fi(field<short>(o, 56));
                        const Point3 direction = {c, s, 0.0f};
                        Point3 target = {field<float>(o, 16) + direction.x * 300.0f, field<float>(o, 20) + s * 300.0f, field<float>(o, 24)};
                        findEnemyLeadPosition__8CreaturefP6Point3(self, 400.0f, &target);
                        Point3 out;
                        const Point3 position = field<Point3>(o, 16);
                        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(word_at(block(o), 12), &position, field<short>(o, 56), o + 288, 0, &out, nullptr, field<float>(o, 224));
                        const short angle = clamp_heading(o, static_cast<short>(16384 - iatan2__Fff(target.x - out.x, target.y - out.y)));
                        Point3 beam = {target.x - out.x, target.y - out.y, target.z + 36.0f - out.z};
                        const float length = beam.x * beam.x + beam.y * beam.y + beam.z * beam.z;
                        const float scale = rsqrt_one(length);
                        beam.x = beam.x * scale;
                        beam.y = beam.y * scale;
                        beam.z = beam.z * scale;
                        beam.x = beam.x * 400.0f;
                        beam.y = beam.y * 400.0f;
                        beam.z = beam.z * 400.0f;
                        void* missile = blockAlloc__Fi(452);
                        if (missile != nullptr) {
                            const unsigned char* b = block(o);
                            missileWeapon_construct(missile, 0, word_at(b, 212), word_at(b, 220), &out, &beam, angle - 16384, self, nullptr, nullptr, field<short>(o, 200),
                                                    field<short>(o, 202) / 2, 0, 0, field<short>(o, 204), 4, nullptr);
                        }
                    }
                }
                turnToFaceEnemy__8Creature(self);
            }
            finish(self, o);
            break;
        }
        case 16:
            zero_move(self);
            if (field<int>(o, 576) != 0) {
                if ((S & 1) != 0) produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, field<short>(o, 200), field<short>(o, 202), field<short>(o, 204), 0x100041, nullptr);
                if (bit8(o)) turnToFaceEnemy__8Creature(self);
            }
            finish(self, o);
            break;
        case 17:
            if ((S & 1) != 0) {
                const Point3 where = field<Point3>(o, 16);
                const float ground = worldFindHeight__FP12_worldHeaderG6Point3f(game_world(), &where, 6.0f);
                float x = field<float>(o, 16) + icos__Fi(field<short>(o, 56) - 5000) * 50.0f;
                float y = field<float>(o, 20) + isin__Fi(field<short>(o, 56) - 5000) * 50.0f;
                const Point3 bolt = {x, y, ground};
                void* p = blockAlloc__Fi(156);
                if (p != nullptr) boltGoRound_construct(p, &bolt, field<short>(o, 56), 240, word_at(block(o), 244));
                field<int>(o, 980) = 0;
                field<int>(o, 1304) = 0;
                field<int>(o, 972) = field<int>(o, 808) + 240;
            }
            finish(self, o);
            break;
        case 18:
            if ((S & 1) != 0) {
                field<int>(o, 980) = 1;
                field<short>(o, 1210) = 150;
                field<short>(o, 1212) = 200;
                field<short>(o, 1214) = 30;
                field<short>(o, 1208) = 150;
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(word_at(block(o), 228), self, 1.0f);
            }
            finish(self, o);
            break;
        default:
            break;
    }
    tail(self, o);
}
