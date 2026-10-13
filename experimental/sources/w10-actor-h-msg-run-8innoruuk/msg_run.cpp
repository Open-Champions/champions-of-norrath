// Observed msg_run__8Innoruuk at 0x001f9150, 11684 bytes.
// Per-frame behaviour of the Innoruuk boss. Written from the image; the structure below follows its
// order. o is the object, "R" the resource block whose pointer is the word at o+36, "SET(n)" the call
// through the object's table: the halfword at table+184 is added to o and the function word at
// table+188 is called with that pointer and n (it is the same slot Innoruuk's setState is in).
// "rand" is eRand__10GameObject(o) reduced with unsigned division. "flags" is the result of
// creatureAdvanceAnimation__8Creaturef(o, 1.0).
//  Prologue, in order: fxRunWeapon for the weapon state at o+1080 (description o+1144, models R+172
//  and R+12, animation o+288), modelGetShadowPos copying the point it finds into o+1064, the light
//  at o+1016 gets its position updated and its size (float o+1076, lowered by 1.8 each frame and
//  kept at 0.0 or above) written to its offsets 28 and 36 with 1.0 / (size * size) at 40 and 32;
//  newDamageHandler and everyFrame (a nonzero result returns); objectActivationCheck(o, 600);
//  creatureAdvanceAnimation; slideBack(o, 0.2); the table call at table+168; then the halfword at
//  o+572 selects the behaviour (values 0..21 as below; every other value, and 2, 4, 7, 9, 11, 12 and
//  13, do nothing more).
// Inferred: the names come from the symbols the code calls. Unknown: what the states mean, the words
// at o+256, +576, +752, +960, +964, +968, +976, +988, +992, +996, +1000, +1004, +1008, +1216 and the
// meaning of the flags bits (bit 0 and 1 trigger attacks; bit 3 and 4 of the halfword o+294 are
// animation-finished style flags only by use).
// Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* weapon_state, const void* position, short yaw, void* model_again, void* animation,
    float a, int mesh, int zero, float b, void* matrix);
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(void* model, void* matrix, int index, void* point, float* value);
void lightUpdatePosition__FP5Light(void* light);
int eRand__10GameObject(void* object);
int objectRemoveFromRunList__FP10GameObject(void* object);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
void objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float rate);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int enemyInZone2__8Creatureffssfi(void* self, float a, float b, short angle, short c, float d, int e);
int alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short angle, short c);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void move2__8CreatureG6Point3(void* self, const void* point);
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short angle, short c, int d, int e,
                                                                  int f, int g, void* description);
void gameSetCameraShake__Ffb(float amount, char flag);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void light_effect_construct(void* self, const void* position, float f0, float f1, float f2, float f3, float f4, float f5, float f6,
                            float f7, float f8, float f9, float f10, float f11, void* object, int flag)
    __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float extra);
void turnToFaceEnemy__8Creature(void* self);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void soundStopSound__Fi(int id);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* position, float first, float second, int a, int b, int c, int d,
                                               void* texture, short e, int f);
void hate_beam_construct(void* self, int a, const void* from, const void* to, void* owner, void* skill)
    __asm__("__8HateBeamiG6Point3T2P8CreatureP5Skill");
void hate_projectile_construct(void* self, const void* position, short yaw, void* owner, void* skill, int a, void* header)
    __asm__("__14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader");
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, void* out);
int creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* target, float range);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* position, short yaw, void* animation, int a, void* out, int b, float c);
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
    void* model, const void* position, short yaw, void* animation, int a, void* matrix, float c);
void phprop_construct(void* self, const void* position, void* matrix, char* a, char* b, char* c, long number, int flag,
                      void* description, float scale) __asm__("__6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf");
void* lumpFindResource__FPcT0(char* lump, char* name);
void* listOnewayLink__FP8ListHeadT0Pv(void* first_head, void* second_head, void* value);
void listDestroy__FP8ListHead(void* head);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
    void* model, void* animation, const void* position, short yaw, void* state, float a, float b, float c, unsigned color);
void hate_soul_construct(void* self, const void* position, const void* velocity, int a, int b, int c)
    __asm__("__8HateSoulG6Point3T1iii");
void engineSetScreenColor__Fiiii(int a, int b, int c, int d);
void deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff(void* self, void* object, void* model, void* animation,
                                                                                     const void* position, short yaw, float a, float b);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void Pop__6Camera(void* camera);
void P_RevolveTrail__FiG6Point3N31i(int trail, const void* a, const void* b, const void* c, const void* d, int e);
void P_DeleteTrail__Fi(int trail);
long long litodp(int a);
long long dpmul(long long a, long long b);
int dpcmp(long long a, long long b);
void MC_SaveCheckPoint__Fiii(int a, int b, int c);
void gameNetKick__Fi(int a);
void netDisconnect__Fv();
void scriptSetGameState__FPci(char* name, int value);
void soundFadeStream__Fi(int stream);
extern signed char netNetworkMode;
extern int frameNm;
extern void* gameCam;
extern void* haloTexture1;
extern unsigned skills[];
extern unsigned innoruukResourceMap[];
extern unsigned char innoruukParticle2[];
extern unsigned char innoruukParticle3[];
extern unsigned char summonhammerParticle[];
extern unsigned char shockwaveInnoruukParticle[];
extern unsigned char shockwaveInnoruukParticle2[];
extern char levelName[];
extern void* haloTexture2;
}

namespace {
const float kDecay = 0x1.cccccc0p+0f;

struct Frame {
    void* self;
    unsigned char* o;
    int flags;
    unsigned& w(unsigned off) { return w10h::word_at(self, off); }
    float& f(unsigned off) { return *reinterpret_cast<float*>(o + off); }
    short& h(unsigned off) { return *reinterpret_cast<short*>(o + off); }
    unsigned short& uh(unsigned off) { return *reinterpret_cast<unsigned short*>(o + off); }
    unsigned char* block() { return w10h::from_word(w(36)); }
    void* header(unsigned off) { return w10h::from_word(*reinterpret_cast<unsigned*>(block() + off)); }
    unsigned char* target() { return w10h::from_word(*reinterpret_cast<unsigned*>(w10h::from_word(w(576)) + 12)); }
};

unsigned rnd(Frame& fr) { return static_cast<unsigned>(eRand__10GameObject(fr.self)); }

void set_state(Frame& fr, int n) {
    typedef void (*Slot)(void*, int);
    w10h::fn_from_word<Slot>(w10h::entry_function(fr.self, 184))(w10h::adjusted(fr.self, 184), n);
}

void* model_header(Frame& fr) {
    typedef void* (*Slot)(void*);
    return w10h::fn_from_word<Slot>(w10h::entry_function(fr.self, 128))(w10h::adjusted(fr.self, 128));
}

void shock_fields(Frame& fr) {
    fr.h(1160) = 115;
    fr.h(1162) = 40;
    fr.h(1166) = 100;
    fr.h(1168) = 64;
    fr.h(1174) = -64;
    fr.f(1076) = 180.0f;
    fr.h(1164) = 0;
    fr.h(1170) = -64;
    fr.h(1172) = -64;
}

void small_fields(Frame& fr) {
    fr.h(140) = 115;
    fr.h(142) = 40;
    fr.h(146) = 100;
    fr.h(148) = 64;
    fr.h(154) = -64;
    fr.h(150) = -64;
    fr.h(152) = -64;
    fr.h(144) = 0;
}

void damage(Frame& fr, float add, short second, int mask) {
    float value = *reinterpret_cast<float*>(fr.target() + 28) + fr.f(28);
    value = value + add;
    produceGeneralDamage__8CreatureffssiiiiPQ24item11description(fr.self, 0.0f, value, fr.h(56), second, fr.h(200), fr.h(202), fr.h(204),
                                                                  mask, nullptr);
}

void screen_flash() { engineSetScreenColor__Fiiii(220, 15, 0, 255); }

void move_nowhere(Frame& fr) {
    float zero[3] = {0.0f, 0.0f, 0.0f};
    move2__8CreatureG6Point3(fr.self, zero);
}

void light_effect(void* position, float tail_a, float tail_b) {
    void* effect = blockAlloc__Fi(188);
    if (effect != nullptr) {
        light_effect_construct(effect, position, 200.0f, 30.0f, 0.0f, -32.0f, 200.0f, 30.0f, 0.0f, -32.0f, 72.0f, 0.0f, tail_a, tail_b, nullptr, 0);
    }
}

void ring(Frame& fr, void* definition, float* position) {
    int angle = 0;
    for (int n = 0; n < 60; ++n) {
        float velocity[3];
        velocity[0] = icos__Fi(angle) * 100.0f;
        velocity[1] = isin__Fi(angle) * 100.0f;
        float z = static_cast<float>(static_cast<int>(rnd(fr)));
        z = z * 0x1.0p-16f;
        velocity[2] = z * 20.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position, velocity, 0.0f);
        angle = static_cast<short>(angle + 1092);
    }
}

void ring_position(Frame& fr, int yaw, float radius, float* out) {
    const float* pos = reinterpret_cast<const float*>(fr.o + 16);
    float x = icos__Fi(yaw) * radius;
    float y = isin__Fi(yaw) * radius;
    float z = 0.0f;
    out[0] = x + pos[0];
    out[1] = y + pos[1];
    out[2] = z + pos[2];
}

void unit_draw(Frame& fr, float* v) {
    float sum;
    do {
        float x = static_cast<float>(static_cast<int>(rnd(fr))) * 0x1.0p-16f;
        x = x + x;
        v[0] = x - 1.0f;
        float y = static_cast<float>(static_cast<int>(rnd(fr))) * 0x1.0p-16f;
        y = y + y;
        v[1] = y - 1.0f;
        float z = static_cast<float>(static_cast<int>(rnd(fr))) * 0x1.0p-16f;
        z = z + z;
        v[2] = z - 1.0f;
        sum = v[0] * v[0] + v[1] * v[1];
        sum = sum + v[2] * v[2];
    } while (1.0f < sum);
}

bool animation_done(Frame& fr) { return (fr.uh(294) & 8) != 0; }

void finish_or_state3(Frame& fr) {
    if (animation_done(fr)) return;
    set_state(fr, 3);
}

// Chains of zone checks shared by state 3 (both tried in the same order).
void state0(Frame& fr) {
    if (enemyInZone__8Creaturefs(fr.self, 600.0f, 32767) != 0) {
        set_state(fr, 3);
        return;
    }
    if (static_cast<int>(fr.w(256)) < static_cast<int>(fr.w(808))) set_state(fr, 1);
}

void state1(Frame& fr) {
    if (enemyInZone__8Creaturefs(fr.self, 600.0f, 32767) != 0) set_state(fr, 3);
    if (static_cast<int>(fr.w(256)) < static_cast<int>(fr.w(808))) {
        set_state(fr, 0);
        return;
    }
    smartWander__8Creature(fr.self);
}

void state3(Frame& fr) {
    if (fr.w(960) == 0) {
        if (fr.w(576) == 0) {
            set_state(fr, 20);
            return;
        }
        if (static_cast<int>(fr.w(996)) < static_cast<int>(fr.w(808))) {
            set_state(fr, 20);
            return;
        }
        if (enemyInZone2__8Creatureffssfi(fr.self, 0.0f, fr.f(28) + 100.0f, fr.h(56), 20000, 0x1.999998p-2f, 0) != 0) {
            set_state(fr, rnd(fr) % 100u < 40u ? 20 : 15);
            return;
        }
        if (alternateEnemyInZone__8Creatureffss(fr.self, 0.0f, fr.f(28) + 100.0f, fr.h(56), 20000) != 0) {
            set_state(fr, rnd(fr) % 100u < 40u ? 20 : 15);
            return;
        }
        steerAndRouteToEnemy__8Creature(fr.self);
        return;
    }
    if (fr.w(576) == 0) {
        set_state(fr, 0);
        return;
    }
    void* self = fr.self;
    if (enemyInZone2__8Creatureffssfi(self, 0.0f, fr.f(28) + 75.0f, fr.h(56), 12000, 0x1.999998p-2f, 0) != 0) {
        set_state(fr, rnd(fr) % 100u < 50u ? 18 : 6);
        return;
    }
    if (enemyInZone2__8Creatureffssfi(self, 75.0f, fr.f(28) + 125.0f, fr.h(56), 2000, 0x1.999998p-2f, 0) != 0) {
        set_state(fr, rnd(fr) % 100u < 70u ? 21 : 6);
        return;
    }
    if (enemyInZone2__8Creatureffssfi(self, 0.0f, fr.f(28) + 100.0f, fr.h(56), 20000, 0x1.999998p-2f, 0) != 0) {
        set_state(fr, 6);
        return;
    }
    if (enemyInZone2__8Creatureffssfi(self, 0.0f, fr.f(28) + 135.0f, fr.h(56), 7000, 0x1.999998p-2f, 0) != 0) {
        if (static_cast<int>(fr.w(992)) < static_cast<int>(fr.w(808))) set_state(fr, 16);
        else steerAndRouteToEnemy__8Creature(self);
        return;
    }
    if (enemyInZone2__8Creatureffssfi(self, 0.0f, fr.f(28) + 100.0f, static_cast<short>(fr.uh(56) - 32767), 4000, 0x1.999998p-2f, 0) != 0) {
        set_state(fr, 17);
        return;
    }
    if (alternateEnemyInZone__8Creatureffss(self, 0.0f, fr.f(28) + 75.0f, fr.h(56), 12000) != 0) {
        set_state(fr, rnd(fr) % 100u < 50u ? 18 : 6);
        return;
    }
    if (alternateEnemyInZone__8Creatureffss(self, 75.0f, fr.f(28) + 125.0f, fr.h(56), 2000) != 0) {
        set_state(fr, rnd(fr) % 100u < 70u ? 21 : 6);
        return;
    }
    if (alternateEnemyInZone__8Creatureffss(self, 0.0f, fr.f(28) + 100.0f, fr.h(56), 20000) != 0) {
        set_state(fr, 6);
        return;
    }
    if (alternateEnemyInZone__8Creatureffss(self, 0.0f, fr.f(28) + 135.0f, fr.h(56), 7000) != 0) {
        if (static_cast<int>(fr.w(992)) < static_cast<int>(fr.w(808))) set_state(fr, 16);
        else steerAndRouteToEnemy__8Creature(self);
        return;
    }
    if (alternateEnemyInZone__8Creatureffss(self, 0.0f, fr.f(28) + 100.0f, static_cast<short>(fr.uh(56) - 32767), 4000) != 0) {
        set_state(fr, 17);
        return;
    }
    steerAndRouteToEnemy__8Creature(self);
}

void state5(Frame& fr) {
    if (bExitStatePersueIdle__8Creature(fr.self) != 0) set_state(fr, 3);
}

void state6(Frame& fr) {
    move_nowhere(fr);
    if (fr.w(576) != 0) {
        if ((fr.flags & 1) != 0) {
            shock_fields(fr);
            if (rnd(fr) % 100u < 70u) damage(fr, 100.0f, 20000, 1);
            else damage(fr, 100.0f, 20000, 0x40001);
        }
        if (!animation_done(fr)) {
            set_state(fr, 3);
            return;
        }
        unsigned char* target = fr.target();
        float point[3];
        w10h::copy_bytes(point, target + 16, 12);
        typedef void (*Slot)(void*, int, void*, void*);
        w10h::fn_from_word<Slot>(w10h::entry_function(fr.self, 176))(w10h::adjusted(fr.self, 176), 0, point, target);
    }
    finish_or_state3(fr);
}

void state21(Frame& fr) {
    move_nowhere(fr);
    unsigned flags_word;
    if (fr.w(576) == 0) {
        flags_word = fr.uh(294);
    } else {
        if ((fr.flags & 1) != 0) {
            gameSetCameraShake__Ffb(22.0f, 0);
            float position[3];
            ring_position(fr, fr.h(56), 100.0f, position);
            light_effect(position, 0x1.999998p-1f, 0x1.333332p-2f);
            ring(fr, innoruukParticle2, position);
            damage(fr, 125.0f, 3000, 0x80001);
        }
        if ((fr.flags & 2) != 0) fr.w(976) = 1;
        flags_word = fr.uh(294);
        if ((flags_word & 8) != 0 && fr.w(976) == 0) {
            turnToFaceEnemy__8Creature(fr.self);
            flags_word = fr.uh(294);
        } else {
            shock_fields(fr);
        }
    }
    if ((flags_word & 8) != 0) return;
    fr.w(976) = 0;
    set_state(fr, 3);
}

void state17(Frame& fr) {
    move_nowhere(fr);
    if ((fr.flags & 1) != 0) {
        gameSetCameraShake__Ffb(22.0f, 0);
        float position[3];
        ring_position(fr, fr.h(56), 100.0f, position);
        light_effect(position, 0x1.999998p-1f, 0x1.333332p-2f);
        ring(fr, innoruukParticle2, position);
        if (fr.w(576) != 0) damage(fr, 100.0f, 5000, 0x80001);
    }
    if ((fr.flags & 2) != 0) fr.w(976) = 1;
    if (fr.w(976) != 0) shock_fields(fr);
    finish_or_state3(fr);
}

void state18(Frame& fr) {
    move_nowhere(fr);
    if ((fr.flags & 1) != 0) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(fr.header(212), fr.self, 1.0f);
        fr.w(968) = 1;
        float position[3];
        ring_position(fr, fr.h(56) - 5000, 50.0f, position);
        light_effect(position, 0x1.999998p-1f, 0x1.333332p-2f);
        ring(fr, innoruukParticle3, position);
        if (rnd(fr) % 100u < 50u) {
            gameSetCameraShake__Ffb(14.0f, 0);
            if (fr.w(576) != 0) damage(fr, 75.0f, 12000, 0x40001);
        } else {
            gameSetCameraShake__Ffb(22.0f, 0);
            if (fr.w(576) != 0) damage(fr, 75.0f, 12000, 0x80001);
        }
    }
    if (fr.w(968) != 0) {
        void* model = model_header(fr);
        float point[3];
        w10h::copy_bytes(point, fr.o + 16, 12);
        fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
            model, fr.o + 288, point, fr.h(56), fr.o + 824, 4.0f, fr.f(224), 0.0f, 0xff000fdcu);
    }
    if (animation_done(fr)) return;
    unsigned node = fr.w(576);
    fr.w(968) = 0;
    if (node == 0) {
        set_state(fr, 3);
        return;
    }
    {
        float value = *reinterpret_cast<float*>(fr.target() + 28) + fr.f(28);
        value = value + 125.0f;
        if (enemyInZone2__8Creatureffssfi(fr.self, 75.0f, value, fr.h(56), 2000, 0x1.999998p-2f, 0) != 0) {
            set_state(fr, rnd(fr) % 100u < 80u ? 21 : 3);
            return;
        }
    }
    if (alternateEnemyInZone__8Creatureffss(fr.self, 75.0f, fr.f(28) + 125.0f, fr.h(56), 2000) != 0) {
        set_state(fr, rnd(fr) % 100u < 80u ? 21 : 3);
        return;
    }
    set_state(fr, 3);
}

void state14(Frame& fr) {
    if (static_cast<int>(fr.w(256)) < static_cast<int>(fr.w(808))) set_state(fr, 3);
}

void state15(Frame& fr) {
    if (fr.w(576) != 0) {
        float first[3];
        float second[3];
        unsigned char* position = fr.o + 16;
        {
            void* model = model_header(fr);
            float copy[3];
            w10h::copy_bytes(copy, position, 12);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, fr.h(56), fr.o + 288, 0, first, 0,
                                                                                               fr.f(224));
        }
        {
            void* model = model_header(fr);
            float copy[3];
            w10h::copy_bytes(copy, position, 12);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, fr.h(56), fr.o + 288, 1, second, 0,
                                                                                               fr.f(224));
        }
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(first, 0.0f, 11000.0f, 220, 45, 0, 150, haloTexture1, 0, 0);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(second, 0.0f, 11000.0f, 220, 45, 0, 150, haloTexture1, 0, 0);
        if ((fr.flags & 1) != 0) {
            void* beam = blockAlloc__Fi(176);
            if (beam != nullptr) {
                hate_beam_construct(beam, 1, first, second, fr.self, w10h::from_word(skills[27]));
            }
            fr.w(1216) = static_cast<unsigned>(soundPlaySequence__FP15AnimationHeaderP10GameObjectf(fr.header(188), fr.self, 1.0f));
        }
        if (animation_done(fr)) {
            turnToFaceEnemy__8Creature(fr.self);
            if (animation_done(fr)) return;
        }
    } else if (animation_done(fr)) {
        return;
    }
    soundStopSound__Fi(static_cast<int>(fr.w(1216)));
    fr.w(1216) = static_cast<unsigned>(-1);
    set_state(fr, 3);
}

void state16(Frame& fr) {
    if (fr.w(576) != 0) {
        const float* pos = reinterpret_cast<const float*>(fr.o + 16);
        int yaw = fr.h(56);
        float cosine = icos__Fi(yaw);
        float sine = isin__Fi(yaw);
        float lead[3];
        float scaled_x = cosine * 300.0f;
        float scaled_y = sine * 300.0f;
        lead[0] = pos[0] + scaled_x;
        lead[1] = pos[1] + scaled_y;
        lead[2] = pos[2];
        findEnemyLeadPosition__8CreaturefP6Point3(fr.self, 350.0f, lead);
        if ((fr.flags & 1) != 0) {
            float attachment[3];
            {
                void* model = model_header(fr);
                float copy[3];
                w10h::copy_bytes(copy, fr.o + 16, 12);
                modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, fr.h(56), fr.o + 288, 1,
                                                                                                   attachment, 0, fr.f(224));
            }
            float saved[3];
            w10h::copy_bytes(saved, fr.o + 16, 12);
            w10h::copy_bytes(fr.o + 16, attachment, 12);
            int lead_yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(fr.self, fr.target(), 350.0f);
            w10h::copy_bytes(fr.o + 16, saved, 12);
            small_fields(fr);
            fr.w(992) = fr.w(808) + 120u;
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(fr.header(196), fr.self, 1.0f);
            for (int i = 0; i < 3; ++i) {
                void* projectile = blockAlloc__Fi(400);
                if (projectile != nullptr) {
                    float spread = static_cast<float>(i) * 0.5f;
                    spread = spread * 11000.0f;
                    float centre = static_cast<float>(lead_yaw) - 5500.0f;
                    int yaw_sum = static_cast<int>(centre) + static_cast<int>(spread);
                    hate_projectile_construct(projectile, attachment, static_cast<short>(yaw_sum), fr.self, w10h::from_word(skills[28]), 1,
                                              fr.header(204));
                }
            }
        }
    } else {
        if (animation_done(fr)) return;
        set_state(fr, 3);
        return;
    }
    if (animation_done(fr)) {
        turnToFaceEnemy__8Creature(fr.self);
        if (animation_done(fr)) return;
    }
    set_state(fr, 3);
}

void sphere_draw(Frame& fr, float* v) {
    // x = cos(r1) * 36 * sin(r2'), y = sin(r3) * 36 * sin(r4'), z = cos(r5') * 36 where r' = (r >> 1) & 0xffff.
    int r1 = static_cast<int>(rnd(fr));
    float x = icos__Fi(r1);
    unsigned r2 = rnd(fr);
    x = x * 36.0f;
    float sx = isin__Fi(static_cast<int>((r2 >> 1) & 0xffff));
    x = x * sx;
    v[0] = x;
    int r3 = static_cast<int>(rnd(fr));
    float y = isin__Fi(r3);
    unsigned r4 = rnd(fr);
    y = y * 36.0f;
    float sy = isin__Fi(static_cast<int>((r4 >> 1) & 0xffff));
    y = y * sy;
    v[1] = y;
    unsigned r5 = rnd(fr);
    float cz = icos__Fi(static_cast<int>((r5 >> 1) & 0xffff));
    v[2] = cz * 36.0f;
}

void state20(Frame& fr) {
    float out0[3];
    float out1[3];
    unsigned char* position = fr.o + 16;
    {
        void* model = model_header(fr);
        float copy[3];
        w10h::copy_bytes(copy, position, 12);
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, fr.h(56), fr.o + 288, 0, out0, 0, fr.f(224));
    }
    {
        void* model = model_header(fr);
        float copy[3];
        w10h::copy_bytes(copy, position, 12);
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, copy, fr.h(56), fr.o + 288, 1, out1, 0, fr.f(224));
    }
    fr.h(1166) = 100;
    fr.h(1160) = 115;
    fr.h(1162) = 40;
    fr.h(1168) = 64;
    fr.h(1174) = -64;
    fr.f(1076) = 180.0f;
    fr.h(1164) = 0;
    fr.h(1170) = -64;
    fr.h(1172) = -64;
    for (int pass = 0; pass < 2; ++pass) {
        const float* base = pass == 0 ? out0 : out1;
        for (int n = 0; n < 40; ++n) {
            float a[3];
            sphere_draw(fr, a);
            float placed[3] = {a[0] + base[0], a[1] + base[1], a[2] + base[2]};
            float toward[3] = {base[0] - placed[0], base[1] - placed[1], base[2] - placed[2]};
            float velocity[3] = {toward[0], toward[1], toward[2]};
            P_AddParticle__FP11ParticleDefP6Point3T1f(summonhammerParticle, placed, velocity, 0.0f);
        }
    }
    if (fr.w(988) != 0) {
        unsigned char* held = w10h::from_word(*reinterpret_cast<unsigned*>(w10h::from_word(fr.w(988)) + 12));
        const int count = modelGetNmShadowPoints__FP12_modelHeader(w10h::from_word(*reinterpret_cast<unsigned*>(held + 76)));
        int index = 0;
        if (count > 0) {
            do {
                unsigned char* object = w10h::from_word(*reinterpret_cast<unsigned*>(w10h::from_word(fr.w(988)) + 12));
                float point[3];
                float value;
                modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(w10h::from_word(*reinterpret_cast<unsigned*>(object + 76)), object + 272, index,
                                                                         point, &value);
                ++index;
                float swing = isin__Fi(static_cast<int>(fr.w(808) << 10));
                int phase = static_cast<int>(static_cast<unsigned>(fr.uh(808)) << 22) >> 16;
                swing = swing * 250.0f;
                swing = swing + 10000.0f;
                swing = swing - 2013.0f;
                P_FaceSprite__FG6Point3ffiiiiP8_texturesi(point, -0x1.9b645ap+3f, swing + 10000.0f, 115, 40, 0, 100, haloTexture2,
                                                          static_cast<short>(phase), 3);
            } while (index < count);
        }
    }
    if ((fr.flags & 1) != 0) {
        unsigned node = fr.w(988);
        if (node != 0) {
            unsigned char* held = w10h::from_word(*reinterpret_cast<unsigned*>(w10h::from_word(node) + 12));
            if (held != nullptr) {
                typedef void (*Slot)(void*, int);
                unsigned char* table = w10h::from_word(w10h::word_at(held, 72));
                Slot destroy = w10h::fn_from_word<Slot>(*reinterpret_cast<unsigned*>(table + 12));
                destroy(held + *reinterpret_cast<short*>(table + 8), 3);
            }
            listDestroy__FP8ListHead(fr.o + 988);
        }
        fr.w(960) = 1;
    }
    if (animation_done(fr)) return;
    set_state(fr, 3);
}

void state8(Frame& fr) {
    if (fr.w(964) != 0 && (fr.flags & 1) != 0) {
        fr.w(964) = 0;
        fr.w(960) = 0;
        float matrix[16];
        float copy[3];
        w10h::copy_bytes(copy, fr.o + 16, 12);
        modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(w10h::from_word(innoruukResourceMap[3]), copy, fr.h(56),
                                                                                              fr.o + 288, 0, matrix, fr.f(224));
        void* prop = blockAlloc__Fi(584);
        if (prop != nullptr) {
            float zero[3] = {0.0f, 0.0f, 0.0f};
            phprop_construct(prop, zero, matrix, const_cast<char*>("innoruuk.lmp"), const_cast<char*>("hammer.vif"), const_cast<char*>("hammer.tex"), 1,
                             0xc001, nullptr, fr.f(224));
            listOnewayLink__FP8ListHeadT0Pv(fr.o + 988, prop, prop);
        }
    }
    if ((fr.uh(294) & 0x10) != 0) return;
    set_state(fr, 0);
}

void sound_for(Frame& fr, const char* lump, const char* name) {
    void* header = lumpFindResource__FPcT0(const_cast<char*>(lump), const_cast<char*>(name));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, fr.self, 1.0f);
}

void spawn_soul(Frame& fr) {
    const unsigned pick = rnd(fr) % 600u;
    const int yaw = fr.h(56) + static_cast<int>(pick) - 300;
    float cosine = icos__Fi(yaw);
    float sine = isin__Fi(yaw);
    float velocity[3] = {cosine * 540.0f, sine * 540.0f, 50.0f};
    float velocity_copy[3];
    w10h::copy_bytes(velocity_copy, velocity, 12);
    float placed[3] = {0.0f, 0.0f, 160.0f};
    const float* pos = reinterpret_cast<const float*>(fr.o + 16);
    placed[0] = placed[0] + pos[0];
    placed[1] = placed[1] + pos[1];
    placed[2] = placed[2] + pos[2];
    void* soul = blockAlloc__Fi(172);
    if (soul != nullptr) hate_soul_construct(soul, placed, velocity_copy, yaw, 0, 1);
    sound_for(fr, "fx.lmp", "spel_he_dismiss_bnce.anm");
}

int elapsed(Frame& fr) { return static_cast<int>(fr.w(808) - fr.w(1000)); }

void state10(Frame& fr) {
    unsigned char* o = fr.o;
    typedef unsigned long long __attribute__((may_alias)) AliasedDouble;
    *reinterpret_cast<AliasedDouble*>(o + 160) = *reinterpret_cast<AliasedDouble*>(o + 160) | 0x10000000ull;
    if ((fr.flags & 1) != 0 && fr.w(960) != 0) {
        fr.w(960) = 0;
        float matrix[16];
        float copy[3];
        w10h::copy_bytes(copy, o + 16, 12);
        modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(w10h::from_word(innoruukResourceMap[3]), copy, fr.h(56),
                                                                                              o + 288, 0, matrix, fr.f(224));
        void* prop = blockAlloc__Fi(584);
        if (prop != nullptr) {
            float zero[3] = {0.0f, 0.0f, 0.0f};
            phprop_construct(prop, zero, matrix, const_cast<char*>("innoruuk.lmp"), const_cast<char*>("hammer.vif"), const_cast<char*>("hammer.tex"), 1,
                             16385, nullptr, fr.f(224));
        }
    }
    if (fr.w(960) != 0) shock_fields(fr);
    int d = elapsed(fr);
    if (d < 3060) {
        float amount = static_cast<float>(d);
        amount = amount * 0x1.56ac00p-12f;
        amount = amount * 5.0f;
        gameSetCameraShake__Ffb(amount, d < 2700);
    }
    d = elapsed(fr);
    if (d < 2880) {
        if (frameNm % 45 == 0) {
            if (rnd(fr) % 6u == 0) {
                const unsigned k = (rnd(fr) % 3u) & 0xffffu;
                if (k == 1) sound_for(fr, "innoruuk.lmp", "cast1.anm");
                else if (k == 0) sound_for(fr, "innoruuk.lmp", "drophamr.anm");
                else if (k == 2) sound_for(fr, "innoruuk.lmp", "lightning.anm");
            } else {
                const unsigned pick = (rnd(fr) & 1u) & 0xffffu;
                if (pick == 0) sound_for(fr, "fx.lmp", "spel_ew_heatwpn_hit.anm");
                else if (pick == 1) sound_for(fr, "fx.lmp", "spel_ew_lightwpn_hit.anm");
            }
        }
    }
    d = elapsed(fr);
    if (static_cast<unsigned>(d - 361) < 1979u) {
        void* model = model_header(fr);
        float point[3];
        w10h::copy_bytes(point, o + 16, 12);
        fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(model, o + 288, point, fr.h(56), o + 824, 8.0f,
                                                                                                fr.f(224), 0.0f, 0xff000fdcu);
        d = elapsed(fr);
    }
    if (static_cast<unsigned>(d - 901) < 539u) {
        float t = static_cast<float>(d - 900);
        t = t * 0x1.e573acp-10f;
        fr.h(144) = 0;
        float a = t * -64.0f;
        float b = t * 115.0f;
        float c = t * 40.0f;
        float e = t * 100.0f;
        float g = t * 64.0f;
        const int va = static_cast<int>(a);
        const int vb = static_cast<int>(b);
        const int vc = static_cast<int>(c);
        const int ve = static_cast<int>(e);
        const int vg = static_cast<int>(g);
        fr.h(154) = static_cast<short>(va);
        fr.h(140) = static_cast<short>(vb);
        fr.h(142) = static_cast<short>(vc);
        fr.h(146) = static_cast<short>(ve);
        fr.h(148) = static_cast<short>(vg);
        fr.h(150) = static_cast<short>(va);
        fr.h(152) = static_cast<short>(va);
    }
    if (static_cast<unsigned>(d - 1441) < 1259u) {
        small_fields(fr);
        if (static_cast<int>(fr.w(808)) % 60 == 0) {
            if (d < 1980) screen_flash();
            d = elapsed(fr);
        }
        if (static_cast<int>(fr.w(808)) % 77 == 0) {
            if (elapsed(fr) < 1980) screen_flash();
            d = elapsed(fr);
        }
        d = elapsed(fr);
    }
    if (static_cast<unsigned>(d - 1981) < 359u) {
        if (static_cast<int>(fr.w(808)) % 47 == 0) spawn_soul(fr);
        if (static_cast<int>(fr.w(808)) % 37 == 0) spawn_soul(fr);
        d = elapsed(fr);
    }
    if (static_cast<unsigned>(d - 2341) < 359u) {
        if (static_cast<int>(fr.w(808)) % 17 == 0) spawn_soul(fr);
        if (static_cast<int>(fr.w(808)) % 37 == 0) spawn_soul(fr);
        d = elapsed(fr);
    }
    if (static_cast<unsigned>(d - 2701) < 179u) {
        if (static_cast<int>(fr.w(808)) % 47 == 0) screen_flash();
        if (static_cast<int>(fr.w(808)) % 37 == 0) screen_flash();
        float ramp = static_cast<float>(elapsed(fr) - 2700);
        float scaled = ramp * 0x1.6c16c0p-8f;
        float fade = 180.0f / ramp;
        fr.h(144) = 0;
        scaled = scaled * 255.0f;
        float low = scaled + -64.0f;
        float e = fade * 100.0f;
        float b = fade * 115.0f;
        scaled = scaled + 64.0f;
        float c = fade * 40.0f;
        const int vlow = static_cast<int>(low);
        const int vb = static_cast<int>(b);
        const int vc = static_cast<int>(c);
        const int ve = static_cast<int>(e);
        const int vhigh = static_cast<int>(scaled);
        fr.h(154) = static_cast<short>(vlow);
        fr.h(140) = static_cast<short>(vb);
        fr.h(150) = static_cast<short>(vlow);
        fr.h(142) = static_cast<short>(vc);
        fr.h(146) = static_cast<short>(ve);
        fr.h(148) = static_cast<short>(vhigh);
        fr.h(152) = static_cast<short>(vlow);
        d = elapsed(fr);
    }
    if (static_cast<unsigned>(d - 2881) < 179u) {
        fr.h(154) = 255;
        fr.h(148) = 255;
        fr.h(150) = 255;
        fr.h(152) = 255;
        for (int n = 0; n < 10; ++n) {
            float v[3];
            unit_draw(fr, v);
            v[0] = v[0] * 40.0f;
            v[1] = v[1] * 40.0f;
            float lift = fr.f(32);
            v[2] = v[2] * 40.0f;
            lift = lift * 0.5f;
            float position[3];
            w10h::copy_bytes(position, o + 16, 12);
            position[2] = position[2] + lift;
            P_AddParticle__FP11ParticleDefP6Point3T1f(shockwaveInnoruukParticle2, position, v, 0.0f);
        }
        if (static_cast<int>(fr.w(808)) % 17 == 0) screen_flash();
        if (static_cast<int>(fr.w(808)) % 37 == 0) screen_flash();
        if (frameNm % 120 == 0) {
            if (((rnd(fr) & 1u) & 0xffffu) != 0) sound_for(fr, "fx.lmp", "spel_ew_heatwpn_hit.anm");
        }
        d = elapsed(fr);
    }
    if (d >= 3061) {
        if (d == 3061) {
            void* model = model_header(fr);
            float point[3];
            w10h::copy_bytes(point, o + 16, 12);
            deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff(fr.self, fr.self, model, o + 288, point, fr.h(56),
                                                                                           fr.f(224), fr.f(228));
            fr.w(12) = fr.w(12) | 3u;
            *reinterpret_cast<AliasedDouble*>(o + 160) = *reinterpret_cast<AliasedDouble*>(o + 160) | (0x8000ull << 26);
            screen_flash();
            gameSetCameraShake__Ffb(20.0f, 0);
            fr.w(1008) = static_cast<unsigned>(P_StartTrail__Fiifiiiii(7, 300, 0x1.333332p-2f, 7, 225, 20, 0, 64));
        }
        if (fr.w(1000) == fr.w(808) - 3062u) {
            screen_flash();
            gameSetCameraShake__Ffb(20.0f, 0);
            sound_for(fr, "fx.lmp", "spel_fire_hit.anm");
            sound_for(fr, "innoruuk.lmp", "hateaura.anm");
            Pop__6Camera(gameCam);
        }
        d = elapsed(fr);
        if (static_cast<unsigned>(d - 3061) < 64u) {
            const float* position = reinterpret_cast<const float*>(o + 16);
            float lift = fr.f(32) * 0.5f;
            const int ticks = static_cast<int>(fr.w(1004));
            float centre[3];
            centre[0] = position[0] + 0.0f;
            centre[1] = position[1];
            centre[2] = position[2] + lift;
            float axis[3] = {0.0f, 0.0f, 1.0f};
            float near_edge[3] = {static_cast<float>(ticks + 5), 0.0f, 0.0f};
            float far_edge[3] = {static_cast<float>(ticks + 53), 0.0f, 0.0f};
            fr.w(1004) = static_cast<unsigned>(ticks + 5);
            P_RevolveTrail__FiG6Point3N31i(static_cast<int>(fr.w(1008)), centre, axis, near_edge, far_edge, 60);
            for (int n = 0;; ++n) {
                long long as_double = litodp(n);
                long long limit = litodp(static_cast<int>(fr.w(1004)) + 48);
                limit = dpmul(limit, 0x40191eb851eb851eLL);
                limit = dpmul(limit, 0x3fb999cbe0000000LL);
                if (!(dpcmp(as_double, limit) < 0)) break;
                float outward[3];
                unit_draw(fr, outward);
                const int r = static_cast<int>(rnd(fr));
                float cosine = icos__Fi(r);
                float radius = static_cast<float>(static_cast<int>(fr.w(1004)) + 48);
                float placed[3];
                placed[0] = cosine * radius;
                float sine = isin__Fi(r);
                float radius_again = static_cast<float>(static_cast<int>(fr.w(1004)) + 48);
                float height = fr.f(32) * 0.5f;
                sine = sine * radius_again;
                placed[2] = height;
                placed[1] = sine;
                float spread[3];
                unit_draw(fr, spread);
                float spread_copy[3];
                w10h::copy_bytes(spread_copy, spread, 12);
                float push[3];
                push[0] = spread_copy[0] * 12.0f;
                push[1] = spread_copy[1] * 12.0f;
                push[2] = spread_copy[2] * 12.0f;
                placed[0] = placed[0] + push[0];
                placed[0] = placed[0] + position[0];
                placed[1] = placed[1] + push[1];
                placed[2] = placed[2] + push[2];
                placed[1] = placed[1] + position[1];
                placed[2] = placed[2] + position[2];
                P_AddParticle__FP11ParticleDefP6Point3T1f(shockwaveInnoruukParticle, placed, outward, 0.0f);
            }
            d = elapsed(fr);
        }
    }
    if (fr.w(1000) == fr.w(808) - 3125u) P_DeleteTrail__Fi(static_cast<int>(fr.w(1008)));
    if (fr.w(1000) == fr.w(808) - 4500u) {
        MC_SaveCheckPoint__Fiii(0, 0, 1);
        if (netNetworkMode != 0) {
            gameNetKick__Fi(0);
            netDisconnect__Fv();
        }
        objectRemoveFromRunList__FP10GameObject(fr.self);
        scriptSetGameState__FPci(const_cast<char*>("whichAct"), -2);
        w10h::copy_bytes(levelName, "actintro", 9);
        levelName[80] = static_cast<char>(-1);
        soundFadeStream__Fi(0);
    }
}
}  // namespace

extern "C" void msg_run__8Innoruuk(void* self) {
    Frame fr = {self, static_cast<unsigned char*>(self), 0};
    unsigned char* o = fr.o;
    {
        unsigned char point[12];
        w10h::copy_bytes(point, o + 16, 12);
        unsigned char* block = fr.block();
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1144, fr.header(172), o + 1080, point, fr.h(56), fr.header(12), o + 288, 0.5f, fr.h(572) == 6, 0, fr.f(224), nullptr);
        (void)block;
    }
    {
        float point[3];
        float value[4];
        modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(fr.header(172), o + 1080, 40, point, value);
        w10h::copy_bytes(o + 1064, point, 12);
    }
    lightUpdatePosition__FP5Light(o + 1016);
    {
        float size = fr.f(1076) - kDecay;
        fr.f(1076) = size;
        if (size < 0.0f) fr.f(1076) = 0.0f;
        size = fr.f(1076);
        float square = size * size;
        fr.f(1044) = size;
        fr.f(1052) = size;
        float inverse = 1.0f / square;
        fr.f(1056) = inverse;
        fr.f(1048) = inverse;
    }
    newDamageHandler__8CreatureP12_modelHeader(self, fr.header(12));
    if (everyFrame__8CreatureP12_modelHeader(self, fr.header(12)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    fr.flags = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0x1.999998p-3f);
    {
        typedef void (*Slot)(void*);
        w10h::fn_from_word<Slot>(w10h::entry_function(self, 168))(w10h::adjusted(self, 168));
    }
    const unsigned state = static_cast<unsigned>(static_cast<int>(fr.h(572)));
    if (state >= 22u) return;
    switch (state) {
        case 0: state0(fr); break;
        case 1: state1(fr); break;
        case 3: state3(fr); break;
        case 5: state5(fr); break;
        case 6: state6(fr); break;
        case 8: state8(fr); break;
        case 10: state10(fr); break;
        case 14: state14(fr); break;
        case 15: state15(fr); break;
        case 16: state16(fr); break;
        case 17: state17(fr); break;
        case 18: state18(fr); break;
        case 19: finish_or_state3(fr); break;
        case 20: state20(fr); break;
        case 21: state21(fr); break;
        default: break;
    }
}
