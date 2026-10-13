#include "../w10-actor-q-common/q.hpp"
// Observed msg_run__14WoodElfSoldier at 0x002477b0, 3360 bytes: the per-frame run routine.
// Notation: m is the pointer stored at +36 (m[12] is a model header); w(x) is the word at +x (signed
// compares are noted); F(n) is the float at +n; E(r, a) is enemyInZone__8Creaturefs(object, r, a); the
// state is the signed halfword at +572; V is the word at +964; Q is the word at +576 (the current
// enemy node, whose word at +12 is the enemy object); R is the result of creatureAdvanceAnimation
// (only bits 0, 1 and 4 are used). setState(n) is the table entry at (word +72) + 184, called with the
// object adjusted by the entry's signed halfword and n; the entry at +168 is called on the adjusted
// object alone; the entry at +176 is called with (adjusted object, 0, pointer).
// Order: newDamageHandler(object, m[12]); a nonzero everyFrame(object, m[12]) returns; then
// objectActivationCheck(object, 600) (result unused) and a zero w(236) returns; R =
// creatureAdvanceAnimation(object, 1.0); slideBack(object, 0.2 (0x3e4ccccc)); the +168 entry;
// moveArrow__14WoodElfSoldier(object); a state outside 0..15 returns, and 4, 11 and 12 do nothing more.
//   0: when V is 2 and commanderValid is zero, joinCommander. If F(856) > 0: setState(7). Else if E(400,
//      32767) or E(500, 16000): setState(3); else if bFarFromHome: setState(1); else when w(256) < w(808):
//      setState(14) if V is 1 and bNextToEdge is nonzero, else setState(1).
//   1: if F(856) > 0: setState(7); else if E(400, 32767) or E(500, 16000): setState(0); else when
//      steerAndRouteToPosition(object, copy of the Point3 at +272) is nonzero or w(256) < w(808):
//      setState(0).
//   2: if E(400, 32767) or E(500, 16000): setState(0); else when smartWander is zero: setState(0).
//   7: when w(808) < w(256): flee2(object, object); else setState(0).
//   3: Q zero: setState(0). F(856) > 0: setState(7). V 1 or 3: (V 3: E(60, 3000) -> setState(6)); E(400,
//      6000) -> setState(15), else steerAndRouteToEnemy. V 2: with commanderValid, aim at a point 50 units
//      from the commander toward the enemy and setState(13) when steerAndRouteToPosition succeeds; without
//      it E(70, 2000) -> setState(6). Otherwise (V 0): E(60, 3000) -> setState(6). Then, V 2 without a
//      commander or V 0: E(70 or 60, 32767) -> turnToFaceEnemy, else steerAndRouteToEnemy.
//   5: by V as in state 3's first checks (V 3: E(60, 3000) -> 6; V 1: E(400, 6000) -> 15; V 2: E(70, 2000) -> 6;
//      V 0: E(60, 3000) -> 6); then bExitStatePersueIdle nonzero -> setState(3).
//   13: Q zero: setState(0); else turnToFaceEnemy, and w(256) < w(808) -> setState(3).
//   6: Q nonzero: R bit 0 -> produceMeleeDamage(object, 60.0, h200, h202, h204, 0x40008, none); halfword
//      +294 bit 3 -> turnToFaceEnemy. Then halfword +294 bit 3 clear -> w(256) -= 1; w(256) > 0 returns,
//      else setState(3).
//   15: with Q nonzero: a target Point3 copied from the enemy at +16; R bit 0 -> clear bit 0x400 of w(244),
//      store modelGetActiveChunks as a doubleword at +552, fire a missile (see the source); R bit 1 -> set
//      bit 0x400 and refresh the chunks; halfword +294 bit 3 -> the +176 entry (adjusted, 0, target).
//      Then halfword +294 bit 3 clear -> w(256) -= 1; w(256) > 0 returns, else setState(3).
//   14: R bit 0 -> clear bit 0x400, chunks, play a random arrow sound, w(988) = 1, +972 = (x, y, z + 58),
//      halfword +984 = halfword +56; R bit 1 -> set bit 0x400, chunks; halfword +294 bit 3 ->
//      TurnToFaceEdge__14WoodElfSoldier; bit 3 clear -> w(256) -= 1; w(256) > 0 returns, else setState(0).
//   9: w(804) < w(808) -> setState(0).   8: halfword +294 bit 4 clear -> setState(3).
//   10: halfword +294 bit 7 -> objectAddToSlowRunList; R bit 4 with bit 1 of w(12) clear -> floorAdd at
//      the point 20 behind the object; R bit 1 with bit 1 of w(12) clear -> w(12) |= 1.
// Inferred: Q is the target enemy node, F(856) a fear or flee timer. Unknown: the meaning of V and of
// the numbers 972 to 988. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void moveArrow__14WoodElfSoldier(void* self);
int commanderValid__14WoodElfSoldier(void* self);
void joinCommander__14WoodElfSoldier(void* self);
int bNextToEdge__14WoodElfSoldier(void* self);
void TurnToFaceEdge__14WoodElfSoldier(void* self);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int bFarFromHome__8Creature(void* self);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, const float* point);
int smartWander__8Creature(void* self);
void flee2__8CreatureP10GameObject(void* self, void* other);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int requested);
int creatureFindLeadYaw__8CreatureP10GameObjectf(void* self, void* other, float range);
int eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int sprintf(char* destination, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* animation, void* object, float gain);
void* blockAlloc__Fi(int size);
void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* model, void* texture, const float* position, const float* velocity, int angle,
    void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description);
void objectAddToSlowRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const float* point, short a, float b, int c, int d);
extern int syncRandState;
extern unsigned char globalItem[] __asm__("_4item$globalItem");
}

namespace {
using namespace w10q;

__attribute__((always_inline)) inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }
__attribute__((always_inline)) inline float inverse_root(float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(1.0f), "f"(value));
    return out;
#else
    return 1.0f / __builtin_sqrtf(value);
#endif
}
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
__attribute__((always_inline)) inline void table_call_point(void* self, const float* point) {
    unsigned char* e = link(self, 72) + 176;
    typedef void (*Call)(void*, int, const float*);
    reinterpret_cast<Call>(from_word(word(e, 4)))(bytes(self) + shalf(e, 0), 0, point);
}
__attribute__((always_inline)) inline void refresh_chunks(void* self, unsigned flags) {
    unsigned char* resource = link(self, 36);
    *reinterpret_cast<Double*>(bytes(self) + 552) = modelGetActiveChunks__FP12_modelHeaderi(link(resource, 12), static_cast<int>(flags));
}
__attribute__((always_inline)) inline void play_arrow_sound(void* self, char* buffer) {
    sprintf(buffer, "swsh_arrow%d.anm", static_cast<int>(static_cast<unsigned>(eRand__10GameObject(self)) & 3u) + 1);
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump, buffer), self, 1.0f);
}
__attribute__((always_inline)) inline void melee_countdown(void* self) {
    if ((half(self, 294) & 0x8u) == 0u) word(self, 256) = word(self, 256) - 1u;
}

// The target z is raised in place; the +176 entry sees the raised point.
void fire_missile(void* self, float* target) {
    unsigned char* enemy = link(link(self, 576), 12);
    word(self, 244) = word(self, 244) & ~0x400u;
    refresh_chunks(self, word(self, 244));
    float position[3];
    position[0] = real(self, 16);
    position[1] = real(self, 20);
    position[2] = real(self, 24) + 58.0f;
    target[2] = target[2] + real(enemy, 32) * 0.5f;
    int yaw = creatureFindLeadYaw__8CreatureP10GameObjectf(self, enemy, 400.0f);
    const unsigned current = half(self, 56);
    const int difference = static_cast<short>(yaw - static_cast<int>(current));
    if (difference > 1000) yaw = static_cast<short>(current + 1000u);
    else if (difference < -1000) yaw = static_cast<short>(current - 1000u);
    const unsigned jitter = static_cast<unsigned>(eRand__10GameObject(self)) % 1800u;
    float dx = target[0] - position[0];
    float dy = target[1] - position[1];
    float dz = target[2] - position[2];
    const float inverse = inverse_root(dx * dx + dy * dy + dz * dz);
    dz = dz * inverse;
    yaw = static_cast<short>(static_cast<unsigned>(yaw) + jitter - 900u);
    float velocity[3];
    velocity[0] = icos__Fi(yaw);
    velocity[1] = isin__Fi(yaw);
    velocity[2] = dz;
    syncRandState = syncRandState * 0x19660d + 0x3c6ef35f;
    const unsigned lift = static_cast<unsigned>(syncRandState) >> 16;
    velocity[0] = velocity[0] * 400.0f;
    velocity[1] = velocity[1] * 400.0f;
    velocity[2] = velocity[2] * 400.0f + static_cast<float>(static_cast<int>(lift % 60u) - 30);
    char name[64];
    play_arrow_sound(self, name);
    void* block = blockAlloc__Fi(452);
    if (block == nullptr) return;
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        block, 0, from_word(word(globalItem, 8)), from_word(word(globalItem, 12)), position, velocity, yaw - 16384, self,
        nullptr, nullptr, shalf(self, 200), shalf(self, 202) / 2, 0, 0, shalf(self, 204), 4, nullptr);
}
}  // namespace

extern "C" void msg_run__14WoodElfSoldier(void* self) {
    newDamageHandler__8CreatureP12_modelHeader(self, link(link(self, 36), 12));
    if (everyFrame__8CreatureP12_modelHeader(self, link(link(self, 36), 12)) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    if (word(self, 236) == 0u) return;
    const unsigned R = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, bits(0x3e4ccccc));
    table_call_plain(self);
    moveArrow__14WoodElfSoldier(self);
    const int state = shalf(self, 572);
    if (static_cast<unsigned>(state) >= 16u) return;
    switch (state) {
    case 0:
        if (word(self, 964) == 2u && commanderValid__14WoodElfSoldier(self) == 0) joinCommander__14WoodElfSoldier(self);
        if (0.0f < real(self, 856)) { table_call_state(self, 7); return; }
        if (in_zone(self, 400.0f, 32767)) { table_call_state(self, 3); return; }
        if (in_zone(self, 500.0f, 16000)) { table_call_state(self, 3); return; }
        if (bFarFromHome__8Creature(self) != 0) { table_call_state(self, 1); return; }
        if (!(sword(self, 256) < sword(self, 808))) return;
        if (word(self, 964) == 1u && bNextToEdge__14WoodElfSoldier(self) != 0) { table_call_state(self, 14); return; }
        table_call_state(self, 1);
        return;
    case 1: {
        if (0.0f < real(self, 856)) { table_call_state(self, 7); return; }
        if (in_zone(self, 400.0f, 32767)) { table_call_state(self, 0); return; }
        if (in_zone(self, 500.0f, 16000)) { table_call_state(self, 0); return; }
        float destination[3] = {real(self, 272), real(self, 276), real(self, 280)};
        if (steerAndRouteToPosition__8CreatureG6Point3(self, destination) != 0) { table_call_state(self, 0); return; }
        if (!(sword(self, 256) < sword(self, 808))) return;
        table_call_state(self, 0);
        return;
    }
    case 2:
        if (in_zone(self, 400.0f, 32767)) { table_call_state(self, 0); return; }
        if (in_zone(self, 500.0f, 16000)) { table_call_state(self, 0); return; }
        if (smartWander__8Creature(self) != 0) return;
        table_call_state(self, 0);
        return;
    case 7:
        if (sword(self, 808) < sword(self, 256)) { flee2__8CreatureP10GameObject(self, self); return; }
        table_call_state(self, 0);
        return;
    case 3: {
        if (word(self, 576) == 0u) { table_call_state(self, 0); return; }
        if (0.0f < real(self, 856)) { table_call_state(self, 7); return; }
        const unsigned variant = word(self, 964);
        if (variant == 1u || variant == 3u) {
            if (variant == 3u && in_zone(self, 60.0f, 3000)) { table_call_state(self, 6); return; }
            if (in_zone(self, 400.0f, 6000)) { table_call_state(self, 15); return; }
            steerAndRouteToEnemy__8Creature(self);
            return;
        }
        float range;
        if (variant == 2u) {
            if (commanderValid__14WoodElfSoldier(self) != 0) {
                unsigned char* commander = link(self, 960);
                unsigned char* leader = link(commander, 12);
                unsigned char* enemy = link(link(self, 576), 12);
                float origin[3] = {real(leader, 16), real(leader, 20), real(leader, 24)};
                float dx = real(enemy, 16) - real(leader, 16);
                float dy = real(enemy, 20) - real(leader, 20);
                float dz = real(enemy, 24) - real(leader, 24);
                const float inverse = inverse_root(dx * dx + dy * dy + dz * dz);
                dx = dx * inverse;
                dy = dy * inverse;
                dz = dz * inverse;
                dx = dx * 50.0f;
                dy = dy * 50.0f;
                dz = dz * 50.0f;
                float target[3] = {origin[0] + dx, origin[1] + dy, origin[2] + dz};
                if (steerAndRouteToPosition__8CreatureG6Point3(self, target) != 0) table_call_state(self, 13);
                return;
            }
            range = 70.0f;
            if (in_zone(self, range, 2000)) { table_call_state(self, 6); return; }
        } else {
            range = 60.0f;
            if (in_zone(self, range, 3000)) { table_call_state(self, 6); return; }
        }
        if (in_zone(self, range, 32767)) turnToFaceEnemy__8Creature(self);
        else steerAndRouteToEnemy__8Creature(self);
        return;
    }
    case 5:
        if (word(self, 964) == 3u && in_zone(self, 60.0f, 3000)) { table_call_state(self, 6); return; }
        if (word(self, 964) == 1u && in_zone(self, 400.0f, 6000)) { table_call_state(self, 15); return; }
        if (word(self, 964) == 2u && in_zone(self, 70.0f, 2000)) { table_call_state(self, 6); return; }
        if (word(self, 964) == 0u && in_zone(self, 60.0f, 3000)) { table_call_state(self, 6); return; }
        if (bExitStatePersueIdle__8Creature(self) == 0) return;
        table_call_state(self, 3);
        return;
    case 13:
        if (word(self, 576) == 0u) { table_call_state(self, 0); return; }
        turnToFaceEnemy__8Creature(self);
        if (!(sword(self, 256) < sword(self, 808))) return;
        table_call_state(self, 3);
        return;
    case 6:
        if (word(self, 576) != 0u) {
            if ((R & 1u) != 0u) {
                produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 60.0f, shalf(self, 200), shalf(self, 202), shalf(self, 204), 0x40008, nullptr);
            }
            if ((half(self, 294) & 0x8u) != 0u) turnToFaceEnemy__8Creature(self);
        }
        melee_countdown(self);
        if (sword(self, 256) > 0) return;
        table_call_state(self, 3);
        return;
    case 15: {
        if (word(self, 576) != 0u) {
            unsigned char* enemy = link(link(self, 576), 12);
            float target[3] = {real(enemy, 16), real(enemy, 20), real(enemy, 24)};
            if ((R & 1u) != 0u) fire_missile(self, target);
            if ((R & 2u) != 0u) {
                word(self, 244) = word(self, 244) | 0x400u;
                refresh_chunks(self, word(self, 244));
            }
            if ((half(self, 294) & 0x8u) != 0u) table_call_point(self, target);
        }
        melee_countdown(self);
        if (sword(self, 256) > 0) return;
        table_call_state(self, 3);
        return;
    }
    case 14: {
        if ((R & 1u) != 0u) {
            word(self, 244) = word(self, 244) & ~0x400u;
            refresh_chunks(self, word(self, 244));
            char name[64];
            play_arrow_sound(self, name);
            float spot[3] = {real(self, 16), real(self, 20), real(self, 24) + 58.0f};
            word(self, 988) = 1u;
            real(self, 972) = spot[0];
            real(self, 976) = spot[1];
            real(self, 980) = spot[2];
            half(self, 984) = half(self, 56);
        }
        if ((R & 2u) != 0u) {
            word(self, 244) = word(self, 244) | 0x400u;
            refresh_chunks(self, word(self, 244));
        }
        if ((half(self, 294) & 0x8u) != 0u) TurnToFaceEdge__14WoodElfSoldier(self);
        melee_countdown(self);
        if (sword(self, 256) > 0) return;
        table_call_state(self, 0);
        return;
    }
    case 9:
        if (!(sword(self, 804) < sword(self, 808))) return;
        table_call_state(self, 0);
        return;
    case 8:
        if ((half(self, 294) & 0x10u) != 0u) return;
        table_call_state(self, 3);
        return;
    case 10: {
        if ((half(self, 294) & 0x80u) != 0u) objectAddToSlowRunList__FP10GameObject(self);
        if ((R & 0x10u) != 0u && (word(self, 12) & 0x2u) == 0u) {
            float point[3];
            point[0] = real(self, 16) - icos__Fi(shalf(self, 56)) * 20.0f;
            point[1] = real(self, 20) - isin__Fi(shalf(self, 56)) * 20.0f;
            point[2] = real(self, 24);
            const short pick = static_cast<short>(eRand__10GameObject(self));
            const unsigned size = static_cast<unsigned>(eRand__10GameObject(self)) % 10u + 20u;
            floorAdd__FG6Point3sfii(point, pick, static_cast<float>(static_cast<int>(size)), 0, 1);
        }
        if ((R & 0x2u) != 0u && (word(self, 12) & 0x2u) == 0u) word(self, 12) = word(self, 12) | 1u;
        return;
    }
    default:
        return;
    }
}
