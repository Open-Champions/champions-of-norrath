#include "../w10-actor-q-common/q.hpp"
// Observed msg_run__6Wraith at 0x00203f08, 3400 bytes: the per-frame run routine.
// Notation: m is the pointer stored at +36 (m[12] is a model header); w(x) is the word at +x (signed
// compares are noted); F(n) is the float at +n; L is the signed byte at +960; E(r, a) is
// enemyInZone__8Creaturefs(object, r, a); Q is the word at +576 (the current enemy node, whose word at
// +12 is the enemy object); R is the result of creatureAdvanceAnimation (bits 0 and 1 used);
// setState(n) is the table entry at (word +72) + 184, called with the object adjusted by the entry's
// signed halfword and n; the entry at +168 is called on the adjusted object alone.
// Order: newDamageHandler(object, m[12]); a nonzero everyFrame returns; a zero
// objectActivationCheck(object, 600) returns; R = creatureAdvanceAnimation(object, 1.0); when the
// halfword state at +572 is not 10 the +168 entry runs; slideBack(object, 0.0). Then the 64-bit word at
// +160 gets bits 0x54 set when L >= 21 and cleared otherwise. L below 99: when the state is not 10,
// bit 0 of w(12) is cleared. L 99 or more: bit 0 of w(12) is set and, with Q nonzero, the halfword at
// +56 becomes iatan2__Fff(enemy y - y, enemy x - x). A state outside 0..15 returns; 2, 4, 7, 11 and 12
// do nothing more.
//   0: L (below 100) += 1. E(400, 32767) or E(500, 16000): setState(3); else w(256) < w(808):
//      setState(1).
//   1: L (below 100) += 1. E(400, 32767) or E(500, 16000): setState(0); else smartWander.
//   3: L moves one step toward 60. Q zero: setState(0). E(F(28) + 20, 6000): setState(13) when w(964) <
//      w(808), else setState(6). E(F(28) + 20, 32767): turnToFaceEnemy. With w(968) zero a point 30 units
//      ahead along the heading is probed with worldFindHeight(world, p, 15.0) (soft-double |h - z|
//      against 6.0): below 6.0, setState(15). Otherwise turnToFaceEnemy, then x and y advance by
//      F(176) * (icos, isin) / 60, z moves 1/6 toward the enemy's z, and objectUpdateInGrid.
//   5: E(F(28) + 20, 6000): as state 3's first test; otherwise bExitStatePersueIdle -> setState(3).
//   6: with Q: R bit 0 -> produceMeleeDamage(object, 36.0, h200, h202, h204, 8, none); turnToFaceEnemy;
//      z toward the enemy; objectUpdateInGrid. Then L (above 0) -= 2; halfword +294 bit 3 clear ->
//      setState(3).
//   13: with Q: R bit 0 -> a fresh damage description (see the source), produceMeleeDamage(..., 0x404,
//      description); on a nonzero result F(560) += F(564) / 15, then F(560) is capped at F(564);
//      turnToFaceEnemy; z toward the enemy; objectUpdateInGrid. Then the same tail as state 6.
//   15: R bit 0 -> a Skeleton is built 30 units ahead (ground height + 1.0) from a 1128-byte block, tinted
//      by halfwords, linked into the list head at +968, and "spel_he_heal.anm" from "fx.lmp" is played on
//      it. Then L (above 0) -= 2 and the same tail as state 6.
//   14: L steps 2 toward 70; w(256) < w(808) -> setState(3).
//   9: w(804) < w(808) -> setState(0).   8: halfword +294 bit 4 clear -> setState(3).
//   10: a file-local particle definition is constructed once; L at 100 or more -> objectAddToDeleteList;
//      else L += 1 and, three times, shadow points of the model give particles (see the source).
// Inferred: L is a fade level and state 10 the death fade. Unknown: the meaning of w(968), F(560/564)
// and of the numbers in the tinting. Nothing here ran on the EE.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int iatan2__Fff(float y, float x);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius);
extern void* world;
unsigned long long fptodp(float value);
int dpcmp(unsigned long long left, unsigned long long right);
unsigned long long dpsub(unsigned long long left, unsigned long long right);
unsigned long long dpmul(unsigned long long left, unsigned long long right);
float dptofp(unsigned long long value);
void objectUpdateInGrid__FP10GameObject(void* object);
void* blockAlloc__Fi(int size);
void __8SkeletonG6Point3sPPcPci(void* self, const float* point, short heading, char** names, char* name, int value);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float gain);
extern unsigned skills[];
extern float worldDifficulty;
int eRand__10GameObject(void* self);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y,
                                         float z, int i, int j, int k, void (*callback)(void*));
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, const float* point, short heading, int index, float* position, float* weight, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position, float* velocity, float scale);
void objectAddToDeleteList__FP10GameObject(void* object);
}

namespace {
using namespace w10q;

static char skeletonText[] = {'S', 'k', 'e', 'l', 'e', 't', 'o', 'n', '\0'};
char* skelname = skeletonText;
unsigned char wraithParticle[80];
int particleReady;

__attribute__((always_inline)) inline int sword(void* o, unsigned off) { return static_cast<int>(word(o, off)); }
__attribute__((always_inline)) inline bool in_zone(void* self, float range, short angle) {
    return enemyInZone__8Creaturefs(self, range, angle) != 0;
}
__attribute__((always_inline)) inline void table_call_state(void* self, int n) {
    unsigned char* e = link(self, 72) + 184;
    typedef void (*Call)(void*, int);
    reinterpret_cast<Call>(from_word(word(e, 4)))(bytes(self) + shalf(e, 0), n);
}
__attribute__((always_inline)) inline void table_call_plain(void* self) {
    unsigned char* e = link(self, 72) + 168;
    typedef void (*Call)(void*);
    reinterpret_cast<Call>(from_word(word(e, 4)))(bytes(self) + shalf(e, 0));
}
// Each fade step reads L again from memory.
__attribute__((always_inline)) inline void fade_down_by_two(void* self) {
    if (sbyte(self, 960) > 0) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) - 2);
}
__attribute__((always_inline)) inline void follow_enemy_z(void* self) {
    unsigned char* enemy = link(link(self, 576), 12);
    const float mine = real(self, 24);
    const float theirs = real(enemy, 24);
    const float step = __builtin_bit_cast(float, 0x3e2aaaaau);
    if (theirs < mine) real(self, 24) = mine - step;
    else if (mine < theirs) real(self, 24) = mine + step;
}
// The tail shared by states 6, 13 and 15.
__attribute__((always_inline)) inline void finish_attack(void* self) {
    fade_down_by_two(self);
    if ((half(self, 294) & 0x8u) != 0u) return;
    table_call_state(self, 3);
}

int skill_roll(unsigned index) {
    unsigned char* skill = from_word(skills[index]);
    unsigned char* entry = link(skill, 96) + 96;
    typedef int (*Call)(void*, int, int);
    return reinterpret_cast<Call>(from_word(word(entry, 4)))(skill + shalf(entry, 0), 0, static_cast<int>(worldDifficulty));
}

int attack_with_description(void* self) {
    unsigned char description[80];
    unsigned char* d = description;
    for (int i = 3; i >= 0; --i) *reinterpret_cast<SHalf*>(d + 36 + 8 * i) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<Half*>(d + 16 + 2 * i) = 0;
        *reinterpret_cast<Half*>(d + 24 + 2 * i) = 0;
    }
    const int rolled = skill_roll(14);
    const short code = 22;
    for (int i = 0; i < 4; ++i) {
        unsigned char* slot = d + 36 + 8 * i;
        const short existing = *reinterpret_cast<SHalf*>(slot);
        if (existing == -1 || existing == code) {
            *reinterpret_cast<SHalf*>(slot) = code;
            *reinterpret_cast<SHalf*>(slot + 6) = 0;
            *reinterpret_cast<SHalf*>(slot + 4) = static_cast<short>(rolled);
            *reinterpret_cast<SHalf*>(slot + 2) = static_cast<short>(rolled);
            break;
        }
    }
    return produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, shalf(self, 200), shalf(self, 202), shalf(self, 204), 0x404, d);
}

void death_particles(void* self) {
    if (particleReady == 0) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(wraithParticle, 10, 10, 30, 10, 10, 10, 30, 0, __builtin_bit_cast(float, 0x4129eb85u), 50.75f, -10.0f,
                                            2, 0, 2, 0);
        particleReady = 1;
    }
    if (sbyte(self, 960) >= 100) {
        objectAddToDeleteList__FP10GameObject(self);
        return;
    }
    ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) + 1);
    unsigned char* resource = link(self, 36);
    unsigned char* position = bytes(self) + 16;
    unsigned char* animation = bytes(self) + 288;
    float accumulated = 0.0f;
    const float zero = 0.0f;
    const int count = modelGetNmShadowPoints__FP12_modelHeader(link(resource, 12));
    float out_position[3];
    float scratch[3];
    float weight;
    float start[3];
    float velocity[3];
    float unit[3];
    float copy[3];
    for (int n = 0; n < 3; ++n) {
        const int pick = static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self))) % count;
        float origin[3] = {*reinterpret_cast<Float*>(position), *reinterpret_cast<Float*>(position + 4), *reinterpret_cast<Float*>(position + 8)};
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            link(link(self, 36), 12), animation, origin, shalf(self, 56), pick, out_position, &weight, nullptr, 1.0f);
        const float amount = weight;
        accumulated = accumulated + amount * amount * amount;
        if (zero < accumulated) {
            do {
                float scale = amount * __builtin_bit_cast(float, 0x3727c5acu);
                accumulated = accumulated - __builtin_bit_cast(float, 0x5196b020u);
                scale = scale * __builtin_bit_cast(float, 0x420d6666u);
                const float unit_step = __builtin_bit_cast(float, 0x37800000u);
                const float a = static_cast<float>(eRand__10GameObject(self));
                const float b = static_cast<float>(eRand__10GameObject(self));
                const float c = static_cast<float>(eRand__10GameObject(self));
                float x = (a * unit_step - 0.5f) * scale;
                float y = (b * unit_step - 0.5f) * scale;
                float z = (c * unit_step - 0.5f) * scale;
                start[0] = x + out_position[0];
                start[1] = y + out_position[1];
                start[2] = z + out_position[2];
                do {
                    const float p = static_cast<float>(eRand__10GameObject(self)) * unit_step;
                    unit[0] = p + p - 1.0f;
                    const float q = static_cast<float>(eRand__10GameObject(self)) * unit_step;
                    unit[1] = q + q - 1.0f;
                    const float r = static_cast<float>(eRand__10GameObject(self)) * unit_step;
                    unit[2] = r + r - 1.0f;
                } while (1.0f < unit[0] * unit[0] + unit[1] * unit[1] + unit[2] * unit[2]);
                copy[0] = unit[0];
                copy[1] = unit[1];
                copy[2] = unit[2];
                velocity[0] = copy[0] * 5.0f;
                velocity[1] = copy[1] * 5.0f;
                velocity[2] = copy[2] * 5.0f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(wraithParticle, start, velocity, zero);
            } while (zero < accumulated);
        }
    }
    (void)scratch;
}
}  // namespace

extern "C" void msg_run__6Wraith(void* self) {
    newDamageHandler__8CreatureP12_modelHeader(self, link(link(self, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, link(link(self, 36), 12)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const unsigned R = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    if (shalf(self, 572) != 10) table_call_plain(self);
    slideBack__8Creaturef(self, 0.0f);
    const unsigned char fade = ubyte(self, 960);
    Double& flags64 = *reinterpret_cast<Double*>(bytes(self) + 160);
    if (static_cast<signed char>(fade) >= 21) flags64 = flags64 | 0x54ull;
    else flags64 = flags64 & ~0x54ull;
    if (static_cast<signed char>(fade) < 99) {
        if (shalf(self, 572) != 10) word(self, 12) = word(self, 12) & ~1u;
    } else {
        word(self, 12) = word(self, 12) | 1u;
        if (word(self, 576) != 0u) {
            unsigned char* enemy = link(link(self, 576), 12);
            half(self, 56) = static_cast<unsigned short>(iatan2__Fff(real(enemy, 20) - real(self, 20), real(enemy, 16) - real(self, 16)));
        }
    }
    const int state = static_cast<short>(half(self, 572));
    if (static_cast<unsigned>(state) >= 16u) return;
    switch (state) {
    case 0:
        if (sbyte(self, 960) < 100) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) + 1);
        if (in_zone(self, 400.0f, 32767)) { table_call_state(self, 3); return; }
        if (in_zone(self, 500.0f, 16000)) { table_call_state(self, 3); return; }
        if (!(sword(self, 256) < sword(self, 808))) return;
        table_call_state(self, 1);
        return;
    case 1:
        if (sbyte(self, 960) < 100) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) + 1);
        if (in_zone(self, 400.0f, 32767)) { table_call_state(self, 0); return; }
        if (in_zone(self, 500.0f, 16000)) { table_call_state(self, 0); return; }
        smartWander__8Creature(self);
        return;
    case 3: {
        if (sbyte(self, 960) > 60) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) - 1);
        if (sbyte(self, 960) < 60) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) + 1);
        if (word(self, 576) == 0u) { table_call_state(self, 0); return; }
        if (in_zone(self, real(self, 28) + 20.0f, 6000)) {
            if (sword(self, 964) < sword(self, 808)) table_call_state(self, 13);
            else table_call_state(self, 6);
            return;
        }
        if (in_zone(self, real(self, 28) + 20.0f, 32767)) { turnToFaceEnemy__8Creature(self); return; }
        if (word(self, 968) == 0u) {
            const float heading_cos = icos__Fi(shalf(self, 56));
            const float heading_sin = isin__Fi(shalf(self, 56));
            const float reach = 30.0f;
            float point[3];
            point[1] = real(self, 20) + heading_sin * reach;
            point[0] = real(self, 16) + heading_cos * reach;
            point[2] = real(self, 24);
            const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, point, 15.0f);
            unsigned long long difference = fptodp(height - real(self, 24));
            if (dpcmp(difference, 0ull) < 0) difference = dpsub(0ull, difference);
            if (dpcmp(difference, 0x4018000000000000ull) < 0) { table_call_state(self, 15); return; }
        }
        turnToFaceEnemy__8Creature(self);
        const float speed = real(self, 176);
        float direction[3];
        direction[0] = icos__Fi(shalf(self, 56)) * speed;
        const float sine = isin__Fi(shalf(self, 56));
        direction[1] = sine * speed;
        direction[2] = dptofp(dpmul(fptodp(speed), 0x3fd0000000000000ull));
        const float inverse = __builtin_bit_cast(float, 0x3c888888u);
        const float dy = direction[1] * inverse;
        const float dx = direction[0] * inverse;
        const float y = real(self, 20) + dy;
        const float x = real(self, 16) + dx;
        real(self, 20) = y;
        real(self, 16) = x;
        follow_enemy_z(self);
        objectUpdateInGrid__FP10GameObject(self);
        return;
    }
    case 5:
        if (in_zone(self, real(self, 28) + 20.0f, 6000)) {
            if (sword(self, 964) < sword(self, 808)) table_call_state(self, 13);
            else table_call_state(self, 6);
            return;
        }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        table_call_state(self, 3);
        return;
    case 6:
        if (word(self, 576) != 0u) {
            if ((R & 1u) != 0u) produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 36.0f, shalf(self, 200), shalf(self, 202), shalf(self, 204), 8, nullptr);
            turnToFaceEnemy__8Creature(self);
            follow_enemy_z(self);
            objectUpdateInGrid__FP10GameObject(self);
        }
        finish_attack(self);
        return;
    case 13:
        if (word(self, 576) != 0u) {
            if ((R & 1u) != 0u) {
                const int result = attack_with_description(self);
                const float cap = real(self, 564);
                if (result != 0) real(self, 560) = real(self, 560) + cap * __builtin_bit_cast(float, 0x3d888888u);
                if (cap < real(self, 560)) real(self, 560) = cap;
            }
            turnToFaceEnemy__8Creature(self);
            follow_enemy_z(self);
            objectUpdateInGrid__FP10GameObject(self);
        }
        finish_attack(self);
        return;
    case 15:
        if ((R & 1u) != 0u) {
            const float heading_cos = icos__Fi(shalf(self, 56));
            const float heading_sin = isin__Fi(shalf(self, 56));
            const float reach = 30.0f;
            float point[3];
            point[1] = real(self, 20) + heading_sin * reach;
            point[0] = real(self, 16) + heading_cos * reach;
            point[2] = real(self, 24);
            point[2] = worldFindHeight__FP12_worldHeaderG6Point3f(world, point, 15.0f) + 1.0f;
            void* block = blockAlloc__Fi(1128);
            if (block != nullptr) {
                __8SkeletonG6Point3sPPcPci(block, point, shalf(self, 56), nullptr, skelname, 0);
                Double& skeleton_flags = *reinterpret_cast<Double*>(bytes(block) + 160);
                const Double merged = skeleton_flags | 0x01000000ull;
                half(block, 150) = 128;
                half(block, 152) = 255;
                half(block, 154) = 150;
                skeleton_flags = merged;
                half(block, 142) = 128;
                half(block, 144) = 255;
                half(block, 146) = 50;
                half(block, 140) = 0;
                half(block, 148) = 0;
                listOnewayLink__FP8ListHeadT0Pv(bytes(self) + 968, block, block);
                static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
                static char sound[] = {'s', 'p', 'e', 'l', '_', 'h', 'e', '_', 'h', 'e', 'a', 'l', '.', 'a', 'n', 'm', '\0'};
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, sound), block, 1.0f);
            }
        }
        finish_attack(self);
        return;
    case 14:
        if (sbyte(self, 960) >= 71) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) - 2);
        if (sbyte(self, 960) < 70) ubyte(self, 960) = static_cast<unsigned char>(ubyte(self, 960) + 2);
        if (!(sword(self, 256) < sword(self, 808))) return;
        table_call_state(self, 3);
        return;
    case 9:
        if (!(sword(self, 804) < sword(self, 808))) return;
        table_call_state(self, 0);
        return;
    case 8:
        if ((half(self, 294) & 0x10u) != 0u) return;
        table_call_state(self, 3);
        return;
    case 10:
        death_particles(self);
        return;
    default:
        return;
    }
}
