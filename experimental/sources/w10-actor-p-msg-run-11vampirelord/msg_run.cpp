// Observed msg_run__11VampireLord at 0x00240be8, 8064 bytes. Offsets are used for fields; nothing
// about their names is established. Words: now (+808); W (+576, a node whose offset 12 holds the
// target object K, zero when there is none); flags +960 +964 +968 +972 +980 ... are named only by
// offset. The signed halfword at +572 is the state (a jump table of 23 entries).
//
// Every frame, in order:
//  1. When +960 is set: fxRunWeapon(description +1096, header at block+172, state +1032, point +16,
//     halfword +56, header block+12, animation state +288, 0.5, (state is 14..17), 0, 1.0, null).
//  2. newDamageHandler and everyFrame (Creature versions) with the header at block+12 (block is the
//     word at +36); a nonzero everyFrame result ends the call. objectActivationCheck(this, 600).
//  3. When +1020 is set: nothing more unless +1016 < now; then table byte offset 184 (the set-state
//     slot) is called with 21 and +1020 is cleared.
//  4. s4 = creatureAdvanceAnimation(this, 2.0 for state 3, else 1.5 when +980 is nonzero, else 1.0);
//     slideBack(this, 0.5). s4 carries animation event bits (1, 2, 4, 8, 0x80).
//  5. When cutMode is set and bit 0x80 of s4: sound for "fx.lmp"/"spel_fire_hit.anm" at the object, a
//     floorAdd at the point raised by 1 (random short tag, 48.0, 9, 0), flag bits 0xa set at +12 and
//     the object leaves the run list.
//  6. Table byte offset 168 is called. With +980 set, the shorts at +1120..1126 become 190, 0, 0, 0.
//  7. With +968 set: sixteen smoke particles at shadow-point positions of the model (model from table
//     byte offset 128) with random ball velocities scaled by 5.61.
//  8. The state dispatch (23-entry jump table; 2, 4, 9, 11, 12 and values above 22 do nothing). The
//     one-line summaries are inferred from the calls each state makes, not from known state names:
//     0 and 1: ranges 400/32767 and 500/18000 around an enemy send the object to state 3; state 0 goes to
//        state 1 once +256 is earlier than now; state 1 otherwise wanders.
//     3: without a target, state 0. Otherwise the last known point is kept at +996; with +960 set the
//        object refreshes a counter at +980 and chooses between states 20, 16, 17, turning or steering by
//        range (radius = float at +28 plus 55, 75 or 100); without +960 it chooses 7 or 18 by range
//        or 13 at +200, else steers.
//     5: leaves on the Creature's persue-idle test. 6, 8: melee and wait, ending in state 3.
//     7 and 10: record the point, spawn six bats with blockAlloc(1088) and the Critter constructor,
//        flee or end (state 10 deletes the object after a sound fade, state 7 converts back to a lord).
//     13 and 17: close in on the target with animation steps; 13 can also strike through a damage
//        description built from the skills object; 14, 15, 16: attack waits that choose the next step.
//     18, 19: charge and strike (19 builds the same description, aims the target and steers);
//        20: wait for a timer then pick 14, 15 or 16; 21: toggle +960 and the active model chunks;
//        22: the death sequence (delete the child list, explosion, sound, state 0 and a new timer).
// Setting a state is table byte offset 184 with the adjusted object and the new state.
// Two endings recur: ENDING_A(mask) returns when a bit of the halfword +294 is set and otherwise sets state
// 3; ENDING_B(mask) does the same but first sets +976 = 300 and +972 = 0.
// Inferred from the call names and the "bat" and "vampirefang" texts: a boss that summons bats and
// strikes in melee; unknown: what the states, flags and timers mean. All float constants were read as literals. Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

struct Point2 { float x, y; };

extern "C" {
extern signed char cutMode;
extern float worldDifficulty;
extern unsigned char skills[];
extern char bossMeterName[];
extern int bossMeterCurrent;
extern unsigned char smokeParticle[] __asm__("smokeParticle");
extern char* batParams[];
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short a, void* model2, void* animation, float f, int g, int h, float i, void* matrix);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void floorAdd__FG6Point3sfii(Point3 where, short tag, float extent, int a, int b);
void objectRemoveFromRunList__FP10GameObject(void* object);
unsigned eRand__10GameObject(void* object);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* out, float* outf, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float delay);
int enemyInZone__8Creaturefs(void* self, float radius, short angle);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void convertToBat__11VampireLord(void* self);
void convertToLord__11VampireLord(void* self);
void flee2__8CreatureP10GameObject(void* self, void* target);
void move2__8CreatureG6Point3(void* self, Point3 step);
int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* state, float time, Point3* position, Point2* uv, short* extra);
int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float range, int a, int b, int c, int flags, void* description);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void* blockAlloc__Fi(int size);
void critter_construct(void* self, char* name, Point3 position, short yaw, char** arguments) __asm__("__7CritterPcG6Point3sPPc");
void soundFadeStream__Fi(int stream);
void objectAddToDeleteList__FP10GameObject(void* object);
void objectUpdateInGrid__FP10GameObject(void* object);
void makeExplosion__FG6Point3f(Point3 where, float size);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
}

namespace {
using namespace w10p;

unsigned draw(unsigned char* o) { return eRand__10GameObject(o); }

unsigned char* block(unsigned char* o) { return from_word(word(o, 36)); }
unsigned char* node_object(unsigned node) { return from_word(word(from_word(node), 12)); }
unsigned short& flags294(unsigned char* o) { return *reinterpret_cast<unsigned short*>(o + 294); }

void set_state(unsigned char* o, int state) { call_entry(o, 184, static_cast<unsigned long>(state)); }
// Returns (does nothing) when a bit of the halfword +294 is set, otherwise sets state 3.
void ending_a(unsigned char* o, unsigned mask) {
    if ((flags294(o) & mask) != 0u) return;
    set_state(o, 3);
}
// The same, but +976 becomes 300 and +972 becomes 0 first.
void ending_b(unsigned char* o, unsigned mask) {
    if ((flags294(o) & mask) != 0u) return;
    word(o, 976) = 300u;
    word(o, 972) = 0u;
    set_state(o, 3);
}
void damage(unsigned char* o, int flags, void* description) {
    produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, bits(0x42100000u), half(o, 200), half(o, 202), half(o, 204), flags, description);
}
bool zone(unsigned char* o, unsigned extra, short angle) { return enemyInZone__8Creaturefs(o, real(o, 28) + bits(extra), angle) != 0; }

// Six bats around the object, shared by states 7 and 10.
void spawn_bats(unsigned char* o) {
    const Point3& p = *reinterpret_cast<const Point3*>(o + 16);
    Point3 u;
    for (int n = 5; n >= 0; --n) {
        unit_ball(o, u, draw);
        u.x = u.x * bits(0x41400000u);
        u.y = u.y * bits(0x41400000u);
        u.z = u.z * bits(0x41400000u);
        u.x = u.x + p.x;
        u.y = u.y + p.y;
        u.z = u.z + p.z;
        u.z = u.z + bits(0x41c00000u);
        void* const critter = blockAlloc__Fi(1088);
        if (critter != nullptr)
            critter_construct(critter, const_cast<char*>("bat"), u, static_cast<short>(static_cast<unsigned short>(half(o, 56)) + draw(o)), batParams);
    }
}

// The animation step shared by states 13 and 17 when the target exists. thr is the distance bound,
// near the time bound, scale1 and scale2 the two per-frame scales. Returns the animation event bits.
int approach_step(unsigned char* o, unsigned char* k, unsigned thr, unsigned nearBits, unsigned scale1, unsigned scale2) {
    const Point3& p = *reinterpret_cast<const Point3*>(o + 16);
    const Point3& kp = *reinterpret_cast<const Point3*>(k + 16);
    Point2& v = *reinterpret_cast<Point2*>(o + 180);
    const float dyd = p.y - kp.y;
    const float dxd = p.x - kp.x;
    const float dist = __builtin_sqrtf(dxd * dxd + dyd * dyd);
    int result;
    float step;
    if (dist < bits(thr)) {
        const float elapsed = real(o, 992);
        if (elapsed < bits(nearBits)) {
            step = bits(nearBits) - elapsed;
            result = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(o + 288, step, const_cast<Point3*>(&p), &v, reinterpret_cast<short*>(o + 188));
            v.x = 0.0f;
            v.y = 0.0f;
            half(o, 188) = 0;
            real(o, 992) = real(o, 992) + step;
            return result;
        }
    }
    {
        const unsigned char counter = o[800];
        step = 1.0f;
        if (static_cast<unsigned>(counter) - 1u < 60u) step = static_cast<float>(60 - static_cast<signed char>(counter)) * bits(scale1);
        result = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(o + 288, step, const_cast<Point3*>(&p), &v, reinterpret_cast<short*>(o + 188));
        real(o, 992) = real(o, 992) + step;
        const float factor = real(o, 224);
        v.x = v.x * factor;
        v.y = v.y * factor;
        const float c = icos__Fi(half(o, 56) - 16384);
        const float s = isin__Fi(half(o, 56) - 16384);
        const float nx = -v.x;
        const float y = real(o, 184);
        const float a = y * s;
        const float b = y * c;
        const float x2 = nx * c + a;
        const float y2 = nx * s - b;
        v.x = x2;
        real(o, 184) = y2;
        if (static_cast<unsigned>(o[800]) - 1u < 60u) {
            const float scale = static_cast<float>(60 - static_cast<signed char>(o[800])) * bits(scale2);
            const unsigned lowered = *reinterpret_cast<unsigned short*>(o + 188);
            v.y = v.y * scale;
            v.x = v.x * scale;
            const int turn = static_cast<int>(static_cast<float>(60 - static_cast<signed char>(o[800])) * bits(scale2));
            half(o, 188) = static_cast<short>(lowered * static_cast<unsigned>(turn));
        }
    }
    return result;
}

// Builds a strike description, asks the skills object for a damage figure, and applies the hit.
void weapon_block(unsigned char* o, unsigned scaleBits, unsigned healBits) {
    word(o, 972) = 0u;
    unsigned char desc[80];
    for (int n = 3; n >= 0; --n) half(desc, 36 + 8 * n) = -1;
    desc[33] = 0xff;
    desc[32] = 0xff;
    for (int n = 0; n < 4; ++n) {
        half(desc, 16 + 2 * n) = 0;
        half(desc, 24 + 2 * n) = 0;
    }
    unsigned char* const skill = from_word(word(skills, 56));
    const int base = static_cast<int>(call_entry_from(skill, 96, 96, 0, static_cast<unsigned long>(static_cast<int>(worldDifficulty))));
    const int doubled = base * 2;
    float scaled = static_cast<float>(doubled) * bits(scaleBits);
    scaled = scaled * (static_cast<float>(draw(o)) * bits(0x37800000u));
    const int variance = static_cast<int>(scaled);
    const int total = doubled + variance;
    int tried = 0;
    unsigned char* e = desc + 36;
    for (;;) {
        const short v = half(e, 0);
        if (v == -1 || v == 22) {
            half(e, 0) = 22;
            half(e, 6) = 0;
            half(e, 2) = static_cast<short>(doubled);
            half(e, 4) = static_cast<short>(total);
            break;
        }
        ++tried;
        if (!(tried < 4)) break;
        e += 8;
    }
    if (produceMeleeDamage__8CreaturefiiiiPQ24item11description(o, bits(0x42100000u), half(o, 200), half(o, 202), half(o, 204), 8, desc) != 0) {
        const float maximum = real(o, 564);
        const float healed = real(o, 560) + maximum * bits(healBits);
        real(o, 560) = healed;
        if (maximum < healed) real(o, 560) = maximum;
        __builtin_memcpy(bossMeterName, "VampireLord", 12);
        bossMeterCurrent = static_cast<int>(real(o, 560));
    }
}

// Tells the target how far it is from a point in front of the object and which way it faces.
// shifted: the starting point is first moved back by 12 units along the heading (state 13).
void aim_target(unsigned char* o, bool shifted) {
    const Point3& p = *reinterpret_cast<const Point3*>(o + 16);
    Point3 t = p;
    if (shifted) {
        const float c = icos__Fi(half(o, 56));
        t.x = t.x - c * bits(0x41400000u);
        const float s = isin__Fi(half(o, 56));
        t.y = t.y - s * bits(0x41400000u);
    }
    Point3 u;
    const float c = icos__Fi(half(o, 56));
    unsigned char* k = node_object(word(o, 576));
    const float reach1 = real(k, 28) + real(o, 28);
    float ux = t.x + reach1 * c;
    const float s = isin__Fi(half(o, 56));
    k = node_object(word(o, 576));
    const float reach2 = real(k, 28) + real(o, 28);
    u.x = ux;
    u.z = p.z;
    u.y = t.y + reach2 * s;
    const Point3& kp = *reinterpret_cast<const Point3*>(k + 16);
    const float dx = kp.x - u.x;
    const float dy = kp.y - u.y;
    real(k, 756) = __builtin_sqrtf(dx * dx + dy * dy) * bits(0x41700000u);
    const int heading = iatan2__Fff(real(k, 20) - u.y, kp.x - u.x);
    half(node_object(word(o, 576)), 748) = static_cast<short>(heading - 32767);
}

// States 14..17 chain: pick the next attack step with the draw (percentage) and the ranges.
void choose_next(unsigned char* o, int below, int above) {
    if (zone(o, 0x425c0000u, 28000)) {
        set_state(o, static_cast<int>(draw(o) % 100u) < 50 ? below : above);
        return;
    }
    if (zone(o, 0x42960000u, 7000)) { set_state(o, 16); return; }
    if (zone(o, 0x42c80000u, 3500)) { set_state(o, 17); return; }
    set_state(o, 3);
}

// Shared body of states 14, 15 and 16.
void attack_wait(unsigned char* o, int s4, int flags, int below, int above) {
    move2__8CreatureG6Point3(o, Point3{0.0f, 0.0f, 0.0f});
    if (word(o, 576) == 0u) { ending_a(o, 8); return; }
    if ((s4 & 1) != 0) damage(o, flags, nullptr);
    if ((flags294(o) & 8u) != 0u) turnToFaceEnemy__8Creature(o);
    if (word(o, 980) == 0u) { ending_a(o, 8); return; }
    if ((s4 & 2) == 0) { ending_a(o, 8); return; }
    choose_next(o, below, above);
}
}  // namespace

extern "C" void msg_run__11VampireLord(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const Point3& here = *reinterpret_cast<const Point3*>(o + 16);
    int s4;

    if (word(o, 960) != 0u) {
        const Point3 copy = here;
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1096, from_word(word(block(o), 172)), o + 1032, copy, half(o, 56), from_word(word(block(o), 12)), o + 288, 0.5f,
            static_cast<unsigned short>(half(o, 572) - 14) < 4u ? 1 : 0, 0, 1.0f, nullptr);
    }
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(block(o), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(block(o), 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    if (word(o, 1020) != 0u) {
        if (!(sword(o, 1016) < sword(o, 808))) return;
        call_entry(o, 184, 21);
        word(o, 1020) = 0u;
    }
    {
        float rate = 1.0f;
        if (half(o, 572) == 3) rate = 2.0f;
        else if (word(o, 980) != 0u) rate = 1.5f;
        s4 = creatureAdvanceAnimation__8Creaturef(self, rate);
    }
    slideBack__8Creaturef(self, 0.5f);
    if (cutMode != 0 && (s4 & 0x80) != 0) {
        void* const header = lumpFindResource__FPcT0("fx.lmp", "spel_fire_hit.anm");
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
        Point3 spot = {here.x, here.y, here.z + 1.0f};
        const short tag = static_cast<short>(draw(o));
        floorAdd__FG6Point3sfii(spot, tag, bits(0x42400000u), 9, 0);
        word(o, 12) = word(o, 12) | 0xau;
        objectRemoveFromRunList__FP10GameObject(self);
    }
    call_entry(o, 168);
    if (word(o, 980) != 0u) {
        half(o, 1122) = 0;
        half(o, 1120) = 190;
        half(o, 1124) = 0;
        half(o, 1126) = 0;
    }
    if (word(o, 968) != 0u) {
        void* const animation = o + 288;
        void* model = from_word(call_entry(o, 128));
        const int points = modelGetNmShadowPoints__FP12_modelHeader(model);
        Point3 spot, out, vel;
        float level;
        for (int n = 0; n < 16; ++n) {
            const int index = static_cast<int>(draw(o)) % points;
            model = from_word(call_entry(o, 128));
            spot = here;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, animation, spot, half(o, 56), index, &out, &level, nullptr,
                                                                                                 real(o, 224));
            unit_ball(o, vel, draw);
            vel.x = vel.x * bits(0x40b3851eu);
            vel.y = vel.y * bits(0x40b3851eu);
            vel.z = vel.z * bits(0x40b3851eu);
            P_AddParticle__FP11ParticleDefP6Point3T1f(smokeParticle, &out, &vel, 0.0f);
        }
    }

    const unsigned state = static_cast<unsigned>(static_cast<int>(half(o, 572)));
    if (state >= 23u) return;
    switch (state) {
        case 0:
            if (enemyInZone__8Creaturefs(self, bits(0x43c80000u), 32767) != 0) { set_state(o, 3); return; }
            if (enemyInZone__8Creaturefs(self, bits(0x43fa0000u), 18000) != 0) { set_state(o, 3); return; }
            if (!(sword(o, 256) < sword(o, 808))) return;
            set_state(o, 1);
            return;
        case 1:
            if (enemyInZone__8Creaturefs(self, bits(0x43c80000u), 32767) != 0) { set_state(o, 3); return; }
            if (enemyInZone__8Creaturefs(self, bits(0x43fa0000u), 18000) != 0) { set_state(o, 3); return; }
            smartWander__8Creature(self);
            return;
        case 3: {
            if (word(o, 576) == 0u) { set_state(o, 0); return; }
            *reinterpret_cast<Point3*>(o + 996) = here;
            if (word(o, 960) != 0u) {
                if (!(sword(o, 988) < sword(o, 808))) word(o, 980) = 0u;
                else word(o, 980) = (draw(o) & 3u) + 4u;
                if (zone(o, 0x425c0000u, 28000)) {
                    sword(o, 984) = sword(o, 808) + sword(o, 980) * 60;
                    set_state(o, 20);
                    return;
                }
                if (zone(o, 0x42960000u, 7000)) { set_state(o, 16); return; }
                if (zone(o, 0x42c80000u, 3500)) { set_state(o, 17); return; }
                if (zone(o, 0x425c0000u, 32767)) { turnToFaceEnemy__8Creature(self); return; }
                steerAndRouteToEnemy__8Creature(self);
                return;
            }
            if (zone(o, 0x425c0000u, 28000)) {
                if (sword(o, 808) < sword(o, 256)) set_state(o, 7);
                else set_state(o, 18);
                return;
            }
            if (zone(o, 0x43480000u, 3500)) { set_state(o, 13); return; }
            steerAndRouteToEnemy__8Creature(self);
            return;
        }
        case 5:
            if (bExitStatePersueIdle__8Creature(self) == 0) return;
            set_state(o, 3);
            return;
        case 6:
            if (word(o, 576) == 0u) { ending_a(o, 8); return; }
            if ((s4 & 1) != 0) damage(o, 8, nullptr);
            if ((flags294(o) & 8u) == 0u) { set_state(o, 3); return; }
            turnToFaceEnemy__8Creature(self);
            ending_a(o, 8);
            return;
        case 7:
            *reinterpret_cast<Point3*>(o + 996) = here;
            if ((s4 & 1) != 0) word(o, 968) = 1u;
            if ((s4 & 2) != 0) {
                word(o, 968) = 0u;
                convertToBat__11VampireLord(self);
                spawn_bats(o);
            }
            if (sword(o, 808) < sword(o, 256) && word(o, 576) != 0u) {
                flee2__8CreatureP10GameObject(self, node_object(word(o, 576)));
                return;
            }
            convertToLord__11VampireLord(self);
            set_state(o, 3);
            return;
        case 8:
            if ((flags294(o) & 0x10u) != 0u) return;
            set_state(o, 3);
            return;
        case 10:
            *reinterpret_cast<Point3*>(o + 996) = here;
            if ((s4 & 1) != 0) word(o, 968) = 1u;
            if ((s4 & 2) != 0) {
                word(o, 968) = 0u;
                convertToBat__11VampireLord(self);
                spawn_bats(o);
            }
            if (sword(o, 808) < sword(o, 256) && word(o, 576) != 0u) {
                flee2__8CreatureP10GameObject(self, node_object(word(o, 576)));
                return;
            }
            soundFadeStream__Fi(0);
            objectAddToDeleteList__FP10GameObject(self);
            return;
        case 13: {
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if (word(o, 576) == 0u) { ending_b(o, 8); return; }
            s4 = approach_step(o, node_object(word(o, 576)), 0x42400000u, 0x42480000u, 0x3c888888u, 0x3c888888u);
            slideBack__8Creaturef(self, 0.5f);
            if ((flags294(o) & 8u) != 0u) turnToFaceEnemy__8Creature(self);
            if ((s4 & 1) != 0) {
                word(o, 972) = 1u;
                damage(o, 8, nullptr);
            } else if ((s4 & 2) != 0) {
                weapon_block(o, 0x3e4ccccc, 0x3da3d70a);
            }
            if (word(o, 576) != 0u && word(o, 972) != 0u) aim_target(o, true);
            ending_b(o, 8);
            return;
        }
        case 14:
            attack_wait(o, s4, 8, 15, 16);
            return;
        case 15:
            attack_wait(o, s4, 8, 14, 16);
            return;
        case 16:
            attack_wait(o, s4, 0x40008, 14, 15);
            return;
        case 17: {
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if (word(o, 576) == 0u) {
                creatureAdvanceAnimation__8Creaturef(self, 1.0f);
                slideBack__8Creaturef(self, 0.5f);
                ending_a(o, 8);
                return;
            }
            s4 = approach_step(o, node_object(word(o, 576)), 0x41c00000u, 0x422c006eu, 0x3c888888u, 0x3c888888u);
            slideBack__8Creaturef(self, 0.5f);
            if ((s4 & 1) != 0) damage(o, 0x80008, nullptr);
            if ((flags294(o) & 8u) != 0u) {
                turnToFaceEnemy__8Creature(self);
                if ((flags294(o) & 8u) != 0u) return;
            }
            if (word(o, 980) == 0u) { set_state(o, 3); return; }
            choose_next(o, 14, 15);
            return;
        }
        case 18:
            if (word(o, 576) != 0u) {
                real(o, 996) = here.x;
                real(o, 1000) = here.y;
                real(o, 1004) = real(o, 1004) + bits(0x40c00000u);
                if ((s4 & 1) != 0) {
                    *reinterpret_cast<Point3*>(o + 996) = here;
                    word(o, 968) = 1u;
                    word(o, 12) = word(o, 12) | 5u;
                    real(o, 1004) = real(o, 1004) + bits(0x424815a8u);
                }
                if ((s4 & 2) != 0) {
                    word(o, 968) = 0u;
                    word(o, 12) = word(o, 12) | 2u;
                }
                real(o, 176) = bits(0x43160000u);
                steerAndRouteToEnemy__8Creature(self);
                word(o, 176) = 0u;
            }
            if (!(sword(o, 256) < sword(o, 808))) return;
            set_state(o, 19);
            return;
        case 19: {
            if (word(o, 576) == 0u) { ending_b(o, 0x20); return; }
            real(o, 996) = here.x;
            real(o, 1000) = here.y;
            real(o, 1004) = real(o, 1004) - bits(0x40fdba73u);
            if ((s4 & 4) != 0) {
                word(o, 968) = 1u;
                *reinterpret_cast<Point3*>(o + 996) = here;
                real(o, 1004) = real(o, 1004) + bits(0x435247aeu);
            }
            if ((s4 & 8) != 0) word(o, 12) = word(o, 12) & ~2u;
            if ((s4 & 1) != 0) {
                word(o, 968) = 0u;
                word(o, 972) = 1u;
                word(o, 12) = word(o, 12) & ~5u;
                damage(o, 8, nullptr);
            } else if ((s4 & 2) != 0) {
                weapon_block(o, 0x3e4ccccc, 0x3da3d70a);
            }
            if (word(o, 972) != 0u) {
                aim_target(o, false);
            } else {
                unsigned char* const k = node_object(word(o, 576));
                const Point3& kp = *reinterpret_cast<const Point3*>(k + 16);
                half(o, 56) = static_cast<short>(iatan2__Fff(kp.y - here.y, kp.x - here.x));
                real(o, 176) = bits(0x43160000u);
                steerAndRouteToEnemy__8Creature(self);
                word(o, 176) = 0u;
            }
            ending_b(o, 0x20);
            return;
        }
        case 20:
            if (word(o, 576) == 0u) { ending_a(o, 8); return; }
            if (word(o, 960) == 0u) return;
            half(o, 1118) = 30;
            half(o, 1112) = 190;
            half(o, 1114) = 0;
            half(o, 1116) = 0;
            turnToFaceEnemy__8Creature(self);
            if (!(sword(o, 984) < sword(o, 808))) return;
            if (zone(o, 0x425c0000u, 28000)) {
                set_state(o, static_cast<int>(draw(o) % 100u) < 50 ? 14 : 15);
                return;
            }
            if (!zone(o, 0x42960000u, 7000)) { set_state(o, 3); return; }
            set_state(o, static_cast<int>(draw(o) % 100u) < 65 ? 16 : 15);
            return;
        case 21:
            if ((s4 & 1) != 0) word(o, 968) = 1u;
            if (!(sword(o, 256) < sword(o, 808))) return;
            if (word(o, 960) != 0u) {
                word(o, 968) = 0u;
                word(o, 244) = 0u;
                word(o, 960) = 0u;
            } else {
                word(o, 968) = 0u;
                word(o, 244) = 3u;
                word(o, 960) = 1u;
            }
            dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(block(o), 12)), sword(o, 244));
            set_state(o, 3);
            return;
        case 22: {
            if ((s4 & 1) != 0) word(o, 968) = 1u;
            const float elapsed = static_cast<float>(sword(o, 808) - sword(o, 1024));
            unsigned list;
            if (bits(0x42efffe7u) < elapsed) {
                list = word(o, 1028);
            } else {
                if (cutMode != 0) return;
                list = word(o, 1028);
            }
            if (list == 0u) {
                word(o, 968) = 0u;
            } else {
                do {
                    void* const victim = node_object(list);
                    const unsigned next = word(from_word(list), 0);
                    objectAddToDeleteList__FP10GameObject(victim);
                    list = next;
                } while (list != 0u);
                word(o, 968) = 0u;
            }
            objectUpdateInGrid__FP10GameObject(self);
            o[570] = static_cast<unsigned char>(o[570] & 0xf7u);
            const Point3 copy = here;
            makeExplosion__FG6Point3f(copy, bits(0x40800000u));
            void* const header = lumpFindResource__FPcT0("fx.lmp", "spel_we_explarrow_hit.anm");
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
            half(o, 572) = 0;
            sword(o, 256) = sword(o, 808) + static_cast<int>(draw(o) % 90u) + 90;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, from_word(word(block(o), 52)), bits(0x3dcccccc), 3);
            return;
        }
        default:
            return;
    }
}
