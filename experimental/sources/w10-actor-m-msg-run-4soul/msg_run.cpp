#include "../w10-actor-m-common/access.hpp"
// Observed msg_run__4Soul at 0x1f3310, 4384 bytes. Nothing is returned. "getModel" is table entry 128 (slot 16)
// called on the adjusted object; "setState(n)" is entry 184 (slot 23) with n; "turnTo(P)" is entry 176 (slot 22)
// called with (0, &P). T is the table at the word at object+36 (entry 28 is a model). h294 is the unsigned
// halfword at +294; word256 and word808 compare as signed. The unit-ball "burst" used below is the Soul particle
// effect: a velocity from three rolls (each 2*r*2^-16 - 1) redrawn while its squared length exceeds 1.0, z forced
// to zero, all components times 15.0, then P_AddParticle__FP11ParticleDefP6Point3T1f(soulParticle, position,
// velocity, 0.0f). Shadow points come from modelGetNmShadowPoints__FP12_modelHeader(getModel) and positions from
// modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, animation at
// +288, source point, short angle, index, out point, out float, null, float at +224).
//  1. newDamageHandler(object, getModel); everyFrame__8CreatureP12_modelHeader(object, getModel) nonzero ends;
//     objectActivationCheck__8Creaturei(object, 600) zero ends. s = creatureAdvanceAnimation__8Creaturef(object,
//     1.0f) (V0 is still the advance result when the image copies it to a saved register in the delay slot of the
//     slideBack call); slideBack__8Creaturef(object, 0.0f); entry 168 is called. With the word at +960 zero and a
//     state other than 17, move2__8CreatureG6Point3(object, zero vector) runs.
//  2. State is the signed halfword at +572; 0..24 only (jump table at 0x46e230). Let tick be
//     ((frameNm ^ signed byte at +569) & 15) == 0 and demonNear__4Soul(object) the Demon test.
//   0: tick and demonNear -> setState(14); else word256 < word808 or bFarFromHome__8Creature -> setState(1).
//   1: tick and demonNear -> setState(14); else steerAndRouteToPosition__8CreatureG6Point3(object, the 12 bytes at
//      +272) nonzero or word256 < word808 -> setState(0).
//   3: word576 == 0 -> setState(0).      5: bExitStatePersueIdle__8Creature nonzero -> setState(3).
//   8 and 15: bit 4 of h294 clear -> setState(14).     13: word256 < word808 -> setState(1).
//   14: tick and not demonNear -> setState(0).
//   10 and 24: unless bit 1 of the word at +12 is set: three bursts at random shadow points (roll % count; each
//      only when the signed byte at +570 is zero); then, when word256 < word808 or bit 7 of h294 is set, for each
//      shadow point in order two bursts (same suppression); then +12 |= 2 and, for state 10, objectAddToDeleteList; for
//      state 24 +12 becomes (old +12) | 0xb and objectRemoveFromRunList__FP10GameObject runs.
//   17: matMakeTransYawScale(M, copy of the point at +16, (short)(h56 + 24384), float at +224), matRot(M, 0, 20384);
//      up to three bursts at random shadow points (transformed by M); then with the word at +964 (a linked object L,
//      whose word at +12 is the target): the direction E is the target position + (floats at +816 and +820 of the
//      target)*0.2f shifted in x and y, turnTo(that), +168 = 0.4f, E = normalised (target - self) with its x and y
//      replaced by icos/isin of the halfword at +56 and everything times 400.0f, z limited to at most -60.0f; without
//      L, E = (icos*400, isin*400, -80.0f). Then four sub-steps: the position at +16 gains E * (1/240), and for each
//      player in nmPlayers (the table at player + 21872 + 23984*i, skipping null and entries whose word at +12 has
//      bit 3) a probe point 36.0f ahead of the object is compared with the player's position (z raised by 0.5 of
//      the float at +32) against the sum of the two floats at +28: inside -> gameAttackResolver (10.0f, 50.0f,
//      100000, 0, 0, 0x04000140, null, false), the hit flag is set and the player loop stops. Afterwards: hit, or
//      word256 < word808, or the object's z below worldFindHeight(world, position, 6.0f) -> objectAddToDeleteList and
//      two bursts per shadow point (at the transformed points); else nothing.
//   19: only when bit 0 of s is set: two bursts per shadow point (transformed through the minion attach matrices
//      when the word at +960 is set), then +12 |= 2 and objectAddToDeleteList.
//   2, 4, 6, 7, 9, 11, 12, 16, 18, 20..23: nothing.
// Inferred: Soul is a spirit effect creature; names are those of the callees. Unknown: the state meanings and the
// fields at +168, +256, +272, +569, +570, +576, +808, +816, +820, +960, +964.
extern "C" {
extern unsigned char soulParticle[];
extern int frameNm;
extern int nmPlayers;
extern unsigned char player[];
extern void* world;
int demonNear__4Soul(void* self);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void move2__8CreatureG6Point3(void* self, Point3 where);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, Point3 where);
int bFarFromHome__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* out_point, float* out_value, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float value);
int eRand__10GameObject(void* object);
void objectAddToDeleteList__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
void getSoulAttachMatrix__6MinionP8Matrix34(void* minion, void* matrix);
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* model, void* animation, int index, void* matrix, float scale);
void matInvert__FP8Matrix34T0(void* out, void* in);
void matMul__FP8Matrix34N20(void* out, void* a, void* b);
void matTransform__FP6Point3P8Matrix34T0(Point3* out, void* matrix, Point3* in);
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* out, Point3 position, short yaw, float scale);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 where, float range);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* target, void* attacker, float a, float b, int c, int d, int e,
                                                                       int f, void* description, bool flag);
}

namespace {
struct Matrix34 { float v[12]; };

inline void set_state(unsigned char* o, int state) { w10m::call_entry(o, 184, static_cast<unsigned long>(state)); }
inline void* model_of(unsigned char* o) { return w10m::from_word(w10m::call_entry(o, 128)); }
inline bool less(unsigned char* o, unsigned left, unsigned right) {
    return static_cast<int>(w10m::word(o, left)) < static_cast<int>(w10m::word(o, right));
}
inline bool tick(unsigned char* o) {
    return ((static_cast<unsigned>(frameNm) ^ static_cast<unsigned>(static_cast<int>(static_cast<signed char>(o[569])))) & 15u) == 0u;
}
// A random velocity of the effect: unit-ball vector with z forced to zero, times 15.0f.
inline Point3 burst_velocity(unsigned char* o) {
    Point3 sample;
    w10m::unit_ball(o, sample, [](unsigned char* object) { return eRand__10GameObject(object); });
    Point3 velocity;
    velocity.x = sample.x;
    velocity.y = sample.y;
    velocity.z = 0.0f;
    velocity.x = velocity.x * w10m::bits(0x41700000u);
    velocity.y = velocity.y * w10m::bits(0x41700000u);
    velocity.z = velocity.z * w10m::bits(0x41700000u);
    return velocity;
}
inline void emit(Point3* where, Point3* velocity) {
    P_AddParticle__FP11ParticleDefP6Point3T1f(soulParticle, where, velocity, 0.0f);
}
inline short shadow_angle(unsigned char* o) {
    return static_cast<short>(static_cast<unsigned>(static_cast<unsigned short>(w10m::half(o, 56))) + 24384u);
}

void shadow_bursts(unsigned char* o) {  // states 10 and 24
    if ((w10m::word(o, 12) & 2u) == 0u) {
        int shots = 0;
        const int count = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
        if (count != 0) {
            do {
                void* model = model_of(o);
                const int roll = eRand__10GameObject(o);
                Point3 source = *reinterpret_cast<Point3*>(o + 16);
                Point3 where;
                float value;
                modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                    model, o + 288, source, w10m::half(o, 56), roll % count, &where, &value, nullptr, w10m::real(o, 224));
                Point3 velocity = burst_velocity(o);
                if (static_cast<signed char>(o[570]) == 0) emit(&where, &velocity);
                ++shots;
            } while (shots < 3);
        }
        if (!(less(o, 256, 808) || (static_cast<unsigned short>(w10m::half(o, 294)) & 0x80u) != 0)) return;
        if (count > 0) {
            for (int index = 0; index < count; ++index) {
                void* model = model_of(o);
                Point3 source = *reinterpret_cast<Point3*>(o + 16);
                Point3 where;
                float value;
                modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                    model, o + 288, source, w10m::half(o, 56), index, &where, &value, nullptr, w10m::real(o, 224));
                for (int shot = 0; shot < 2; ++shot) {
                    Point3 velocity = burst_velocity(o);
                    if (static_cast<signed char>(o[570]) == 0) emit(&where, &velocity);
                }
            }
        }
        const unsigned old = w10m::word(o, 12);
        w10m::word(o, 12) = old | 2u;
        if (w10m::half(o, 572) == 10) {
            objectAddToDeleteList__FP10GameObject(o);
        } else {
            w10m::word(o, 12) = old | 0xbu;
            objectRemoveFromRunList__FP10GameObject(o);
        }
    }
}

void despawn_bursts(unsigned char* o, int count, Matrix34* matrix) {
    objectAddToDeleteList__FP10GameObject(o);
    for (int index = 0; index < count; ++index) {
        void* model = model_of(o);
        Point3 origin;
        origin.x = 0.0f;
        origin.y = 0.0f;
        origin.z = 0.0f;
        Point3 where;
        float value;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, o + 288, origin, -16384, index, &where, &value,
                                                                                         nullptr, w10m::real(o, 224));
        matTransform__FP6Point3P8Matrix34T0(&where, matrix, &where);
        for (int shot = 0; shot < 2; ++shot) {
            Point3 velocity = burst_velocity(o);
            emit(&where, &velocity);
        }
    }
}

void travelling_soul(unsigned char* o) {  // state 17
    unsigned char* position = o + 16;
    Matrix34 matrix;
    int shots = 0;
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
    matMakeTransYawScale__FP8Matrix34G6Point3sf(&matrix, *reinterpret_cast<Point3*>(position), shadow_angle(o), w10m::real(o, 224));
    matRot__FP8Matrix34is(&matrix, 0, 20384);
    if (count != 0) {
        do {
            void* model = model_of(o);
            Point3 origin;
            origin.x = 0.0f;
            origin.y = 0.0f;
            origin.z = 0.0f;
            const int roll = eRand__10GameObject(o);
            Point3 where;
            float value;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, o + 288, origin, -16384, roll % count, &where,
                                                                                             &value, nullptr, w10m::real(o, 224));
            matTransform__FP6Point3P8Matrix34T0(&where, &matrix, &where);
            Point3 velocity = burst_velocity(o);
            emit(&where, &velocity);
            ++shots;
        } while (shots < 3);
    }
    int hit = 0;
    Point3 push;
    unsigned char* linked = w10m::from_word(w10m::word(o, 964));
    if (w10m::word(o, 964) != 0) {
        unsigned char* target = w10m::from_word(w10m::word(linked, 12));
        Point3 aim = *reinterpret_cast<Point3*>(target + 16);
        aim.x = aim.x + w10m::real(w10m::from_word(w10m::word(linked, 12)), 816) * w10m::bits(0x3e4ccccc);
        aim.y = aim.y + w10m::real(w10m::from_word(w10m::word(linked, 12)), 820) * w10m::bits(0x3e4ccccc);
        w10m::real(o, 168) = w10m::bits(0x3ecccccc);
        w10m::call_entry(o, 176, 0, reinterpret_cast<__UINTPTR_TYPE__>(&aim));
        unsigned char* goal = w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 964)), 12));
        Point3 delta;
        delta.z = *reinterpret_cast<float*>(goal + 16 + 8) - *reinterpret_cast<float*>(position + 8);
        delta.x = *reinterpret_cast<float*>(goal + 16) - *reinterpret_cast<float*>(position);
        delta.y = *reinterpret_cast<float*>(goal + 16 + 4) - *reinterpret_cast<float*>(position + 4);
        Point3 unit = delta;
        const float inverse = [](float length_squared) {
            const float one = 1.0f;
#if defined(__mips__)
            float result;
            __asm__("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
            return result;
#else
            return one / __builtin_sqrtf(length_squared);
#endif
        }(unit.x * unit.x + unit.y * unit.y + unit.z * unit.z);
        unit.x = unit.x * inverse;
        unit.z = unit.z * inverse;
        unit.y = unit.y * inverse;
        push.x = icos__Fi(w10m::half(o, 56));
        push.y = isin__Fi(w10m::half(o, 56));
        push.x = push.x * w10m::bits(0x43c80000u);
        const float z = unit.z * w10m::bits(0x43c80000u);
        push.y = push.y * w10m::bits(0x43c80000u);
        push.z = w10m::bits(0xc2700000u) < z ? w10m::bits(0xc2700000u) : z;
    } else {
        push.x = icos__Fi(w10m::half(o, 56)) * w10m::bits(0x43c80000u);
        push.y = isin__Fi(w10m::half(o, 56)) * w10m::bits(0x43c80000u);
        push.z = w10m::bits(0xc2a00000u);
    }
    const float step = w10m::bits(0x3b888888u);
    for (int substep = 3; substep >= 0; --substep) {
        Point3 delta;
        delta.x = push.x * step;
        delta.y = push.y * step;
        delta.z = push.z * step;
        w10m::real(o, 20) = w10m::real(o, 20) + delta.y;
        w10m::real(o, 16) = w10m::real(o, 16) + delta.x;
        w10m::real(o, 24) = w10m::real(o, 24) + delta.z;
        if (nmPlayers > 0) {
            for (int i = 0; i < nmPlayers; ++i) {
                const unsigned entity_word = w10m::word(player, 23984u * static_cast<unsigned>(i) + 21872u);
                unsigned char* entity = w10m::from_word(entity_word);
                if (entity_word == 0) continue;
                if ((w10m::word(entity, 12) & 8u) != 0u) continue;
                Point3 foe = *reinterpret_cast<Point3*>(entity + 16);
                foe.z = foe.z + w10m::real(entity, 32) * 0.5f;
                const float c = icos__Fi(w10m::half(o, 56)) * w10m::bits(0x42100000u);
                const float x = w10m::real(o, 16) + c;
                const float s = isin__Fi(w10m::half(o, 56)) * w10m::bits(0x42100000u);
                Point3 probe;
                probe.x = x;
                probe.y = w10m::real(o, 20) + s;
                probe.z = w10m::real(o, 24);
                const float reach = w10m::real(o, 28) + w10m::real(entity, 28);
                const float dx = probe.x - foe.x;
                const float dy = probe.y - foe.y;
                const float dz = probe.z - foe.z;
                if (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) <= reach) {
                    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(entity, o, w10m::bits(0x41200000u), w10m::bits(0x42480000u), 100000, 0, 0,
                                                                                    0x04000140, nullptr, false);
                    hit = 1;
                    break;
                }
            }
        }
    }
    if (hit == 0) {
        if (!less(o, 256, 808)) {
            Point3 here = *reinterpret_cast<Point3*>(position);
            const float ground = worldFindHeight__FP12_worldHeaderG6Point3f(world, here, w10m::bits(0x40c00000u));
            if (!(w10m::real(o, 24) < ground)) return;
        }
    }
    if (count > 0) despawn_bursts(o, count, &matrix);
    else objectAddToDeleteList__FP10GameObject(o);
}

void attached_bursts(unsigned char* o) {  // state 19
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
    if (count > 0) {
        for (int index = 0; index < count; ++index) {
            void* model = model_of(o);
            Point3 origin;
            origin.x = 0.0f;
            origin.y = 0.0f;
            origin.z = 0.0f;
            Point3 where;
            float value;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, o + 288, origin, -16384, index, &where, &value,
                                                                                             nullptr, w10m::real(o, 224));
            if (w10m::word(o, 960) != 0) {
                Matrix34 attach, inverse;
                getSoulAttachMatrix__6MinionP8Matrix34(w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 960)), 12)), &attach);
                modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(
                    w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 28)), o + 288, 0, &inverse, 1.0f);
                matInvert__FP8Matrix34T0(&inverse, &inverse);
                matMul__FP8Matrix34N20(&attach, &inverse, &attach);
                matTransform__FP6Point3P8Matrix34T0(&where, &attach, &where);
            }
            for (int shot = 0; shot < 2; ++shot) {
                Point3 velocity = burst_velocity(o);
                emit(&where, &velocity);
            }
        }
    }
    w10m::word(o, 12) = w10m::word(o, 12) | 2u;
    objectAddToDeleteList__FP10GameObject(o);
}
}  // namespace

extern "C" void msg_run__4Soul(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, model_of(o));
    if (everyFrame__8CreatureP12_modelHeader(self, model_of(o)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const int advanced = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.0f);
    w10m::call_entry(o, 168);
    if (w10m::word(o, 960) == 0 && w10m::half(o, 572) != 17) {
        Point3 zero;
        zero.x = 0.0f;
        zero.y = 0.0f;
        zero.z = 0.0f;
        move2__8CreatureG6Point3(self, zero);
    }
    switch (static_cast<unsigned>(static_cast<int>(w10m::half(o, 572)))) {
    case 0u:
        if (tick(o) && demonNear__4Soul(self) != 0) set_state(o, 14);
        else if (less(o, 256, 808) || bFarFromHome__8Creature(self) != 0) set_state(o, 1);
        break;
    case 1u:
        if (tick(o) && demonNear__4Soul(self) != 0) {
            set_state(o, 14);
        } else if (steerAndRouteToPosition__8CreatureG6Point3(self, *reinterpret_cast<Point3*>(o + 272)) != 0 || less(o, 256, 808)) {
            set_state(o, 0);
        }
        break;
    case 3u:
        if (w10m::word(o, 576) == 0) set_state(o, 0);
        break;
    case 5u:
        if (bExitStatePersueIdle__8Creature(self) != 0) set_state(o, 3);
        break;
    case 8u:
    case 15u:
        if ((static_cast<unsigned short>(w10m::half(o, 294)) & 0x10u) == 0) set_state(o, 14);
        break;
    case 13u:
        if (less(o, 256, 808)) set_state(o, 1);
        break;
    case 14u:
        if (tick(o) && demonNear__4Soul(self) == 0) set_state(o, 0);
        break;
    case 10u:
    case 24u:
        shadow_bursts(o);
        break;
    case 17u:
        travelling_soul(o);
        break;
    case 19u:
        if ((advanced & 1) != 0) attached_bursts(o);
        break;
    default:
        break;
    }
}
