// Observed cast__19SkillSummonSkeletoniP8CreatureG6Point3s at 0x002a86b0, 1972 bytes in .text.
// Arguments: skill, rank, creature, point (the by-value Point3, called P below) and the trailing short (called angle).
// Everything below is read from the function body; "table byte N" means the entry at byte N of the table whose address is
// the word at offset 72 of the creature (creature table) or offset 96 of the skill (skill table); an entry is a signed
// halfword that adjusts the receiver and a function word at +4. Nothing here ran on the EE.
//  1. four halfwords of the creature are written: -128 at byte offsets 148 and 152, -110 at 150 and 30 at 154.
//  2. M = creature table byte 128 called with the adjusted creature; N = modelGetNmShadowPoints__FP12_modelHeader(M).
//  3. when N is non-zero, two particle loops run:
//     a. 32 times: M = creature table byte 128 again; eRandState = eRandState * 0x19660d + 0x3c6ef35f and the index is
//        (eRandState >> 16) % N (signed); modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f
//        is called with (M, creature + 288, the Point3 at creature + 16, the signed halfword at creature + 56, the index, &Q,
//        null, null, 1.0) so Q is filled in; then two points U1 and U2 are drawn from the unit ball by rejection (see
//        below); Q += U1 * 0.23699799 (bits 0x3e72af9a); V = U2 * 0.06800499 (bits 0x3d8b4634); and
//        P_AddParticle__FP11ParticleDefP6Point3T1f is called with (summonskeletonParticle1, &Q, &V, 0.0);
//     b. 200 times: the same model lookup and shadow position are done (the random state is advanced once for the index);
//        then V = (P - Q) * -2.0004380 (bits 0xc000072d) and P_AddParticle is called with (summonskeletonParticle2, &Q, &V, 0.0).
//     A point is drawn from the unit ball by taking, for each of x, y and z in turn, the state advanced once more as above,
//     v = (state >> 16) converted to float times 2^-16, doubled and reduced by 1.0, and repeating the three draws while
//     1.0 < (x*x + y*y) + z*z. The random state is stored back once per loop-a iteration after both draws (as observed).
//  4. S = P with icos__Fi(angle) * 36.0 added to the first float and isin__Fi(angle) * 36.0 added to the second, and then its
//     third float replaced by worldFindHeight__FP12_worldHeaderG6Point3f(world, S, 12.0).
//  5. syncRandState = syncRandState * 0x19660d + 0x3c6ef35f and rnd = (state >> 16) * 2^-16. A = skill table byte 88 called
//     with (adjusted skill, 0, rank). When bit 0x40 of the word at creature+12 is set and the word at creature+960 is
//     below four, params = skelightParams + 8 * that word, otherwise null. B = skill table byte 88 called with
//     (adjusted skill, 1, rank). flag = rnd < (float)B * 0.01 (bits 0x3c23d70a).
//  6. 1112 bytes are allocated with blockAlloc__Fi; when the block K is not null __8SkelightG6Point3sPPcci is called with
//     (K, S, angle, params, flag, A).
//  7. when K is not null and rank is positive: the 64-bit value at K+160 gets bit 0x400000 set, the halfword at K+792 is set
//     to -1, and when bit 0x40 of the creature word at +12 is set the byte at K+85 is set to the low byte of the creature
//     word at +960.
//  8. the resource "spel_de_sumskel.anm" is looked up in "fx.lmp" and played at P with
//     soundPlaySequence__FP15AnimationHeaderG6Point3f(result, P, 1.0).
//  9. when K is null the function returns null. Otherwise 28 bytes are allocated; when that block O is null it returns null;
//     otherwise O is filled in place and returned: byte 0 = low byte of rank, word +4 = 0, word +8 = skill, word +12 =
//     creature, syncRandState advanced again with word +16 = the new state >> 16, word +20 = the table _vt$5Spell address
//     and then the table _vt$19SpellSummonSkeleton address, word +24 = 0, and listOnewayLink__FP8ListHeadT0Pv(O + 24, K, K).
// Inferred: this casts a summoning with a particle burst around the creature's model shadow points, places a Skelight
// object at the point (found on the ground by worldFindHeight) and ties a spell object to it. The loop counts, constants and
// field offsets are as observed; the names of the roles are not established here.
// Unknown: the meaning of the many constants and fields, the identity of the table byte 128 call, and the layouts of skill,
// creature, Skelight, the particle definitions and the model.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* model, void* animation, Point3 position, short angle, int index, Point3* out, float* a, void* b, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world_header, Point3 point, float scale);
void skelight_ctor(void* self, Point3 point, short angle, char** params, char flag, int value) asm("__8SkelightG6Point3sPPcci");
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* sound, Point3 position, float volume);
void listOnewayLink__FP8ListHeadT0Pv(void* list, void* node, void* data);
extern unsigned eRandState;
extern unsigned syncRandState;
extern void* world;
extern unsigned char summonskeletonParticle1[];
extern unsigned char summonskeletonParticle2[];
extern unsigned char skelightParams[];
extern char vt_spell_base[] asm("_vt$5Spell");
extern char vt_spell_summon_skeleton[] asm("_vt$19SpellSummonSkeleton");
void* cast_summon_skeleton(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__19SkillSummonSkeletoniP8CreatureG6Point3s");
}
static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_de_sumskel.anm";
static inline __attribute__((always_inline)) unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static inline __attribute__((always_inline)) unsigned advance(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }
// Draws a point from the unit ball by rejection, advancing the state three times per attempt.
static inline __attribute__((always_inline)) void draw_in_ball(unsigned& state, Point3& out) {
    const float unit = 0x1p-16f;
    float x, y, z;
    do {
        state = advance(state);
        x = static_cast<float>(state >> 16) * unit;
        state = advance(state);
        y = static_cast<float>(state >> 16) * unit;
        state = advance(state);
        z = static_cast<float>(state >> 16) * unit;
        x = x + x;
        y = y + y;
        z = z + z;
        x = x - 1.0f;
        y = y - 1.0f;
        z = z - 1.0f;
    } while (1.0f < (x * x + y * y) + z * z);
    out.x = x;
    out.y = y;
    out.z = z;
}
extern "C" void* cast_summon_skeleton(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    short* colours = reinterpret_cast<short*>(c + 148);
    colours[1] = -110;
    colours[2] = -128;
    colours[3] = 30;
    colours[0] = -128;
    const int points = modelGetNmShadowPoints__FP12_modelHeader(w13scb::call_object_entry0(c, 72, 128));
    Point3 q;
    if (points != 0) {
        for (int i = 0; i < 32; ++i) {
            void* model = w13scb::call_object_entry0(c, 72, 128);
            unsigned state = advance(eRandState);
            eRandState = state;
            const int index = static_cast<int>(state >> 16) % points;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, c + 288, *reinterpret_cast<Point3*>(c + 16), *reinterpret_cast<short*>(c + 56), index, &q, nullptr, nullptr, 1.0f);
            state = eRandState;
            Point3 first, second;
            draw_in_ball(state, first);
            q.x = q.x + first.x * 0x1.e55f34p-3f;
            q.y = q.y + first.y * 0x1.e55f34p-3f;
            q.z = q.z + first.z * 0x1.e55f34p-3f;
            draw_in_ball(state, second);
            eRandState = state;
            Point3 velocity;
            velocity.x = second.x * 0x1.168c68p-4f;
            velocity.y = second.y * 0x1.168c68p-4f;
            velocity.z = second.z * 0x1.168c68p-4f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(summonskeletonParticle1, &q, &velocity, 0.0f);
        }
        for (int counter = 199; counter >= 0; --counter) {
            void* model = w13scb::call_object_entry0(c, 72, 128);
            unsigned state = advance(eRandState);
            eRandState = state;
            const int index = static_cast<int>(state >> 16) % points;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, c + 288, *reinterpret_cast<Point3*>(c + 16), *reinterpret_cast<short*>(c + 56), index, &q, nullptr, nullptr, 1.0f);
            Point3 velocity;
            velocity.x = (point.x - q.x) * -0x1.000e5ap+1f;
            velocity.y = (point.y - q.y) * -0x1.000e5ap+1f;
            velocity.z = (point.z - q.z) * -0x1.000e5ap+1f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(summonskeletonParticle2, &q, &velocity, 0.0f);
        }
    }
    Point3 spot = point;
    spot.x = spot.x + icos__Fi(angle) * 36.0f;
    spot.y = spot.y + isin__Fi(angle) * 36.0f;
    spot.z = worldFindHeight__FP12_worldHeaderG6Point3f(world, spot, 12.0f);
    unsigned sync = advance(syncRandState);
    syncRandState = sync;
    const float roll = static_cast<float>(sync >> 16) * 0x1p-16f;
    const int value = w13scb::call_skill_entry(static_cast<unsigned char*>(skill), 88, 0, rank);
    char** params = nullptr;
    if (*reinterpret_cast<unsigned*>(c + 12) & 0x40) {
        const unsigned player_index = *reinterpret_cast<unsigned*>(c + 960);
        if (player_index < 4) params = reinterpret_cast<char**>(skelightParams + 8 * player_index);
    }
    const int ramp = w13scb::call_skill_entry(static_cast<unsigned char*>(skill), 88, 1, rank);
    const char flag = roll < static_cast<float>(ramp) * 0x1.47ae14p-7f ? 1 : 0;
    unsigned char* light = static_cast<unsigned char*>(blockAlloc__Fi(1112));
    if (light) skelight_ctor(light, spot, angle, params, flag, value);
    if (light && rank > 0) {
        *reinterpret_cast<unsigned long long*>(light + 160) |= 0x400000ull;
        *reinterpret_cast<short*>(light + 792) = -1;
        if (*reinterpret_cast<unsigned*>(c + 12) & 0x40) light[85] = c[960];
    }
    soundPlaySequence__FP15AnimationHeaderG6Point3f(lumpFindResource__FPcT0(lump_name, sound_name), point, 1.0f);
    if (!light) return nullptr;
    unsigned char* object = static_cast<unsigned char*>(blockAlloc__Fi(28));
    if (!object) return nullptr;
    object[0] = static_cast<unsigned char>(rank);
    *reinterpret_cast<unsigned*>(object + 20) = word_of(vt_spell_base);
    *reinterpret_cast<unsigned*>(object + 12) = word_of(creature);
    *reinterpret_cast<unsigned*>(object + 4) = 0;
    *reinterpret_cast<unsigned*>(object + 8) = word_of(skill);
    syncRandState = advance(syncRandState);
    *reinterpret_cast<unsigned*>(object + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(object + 20) = word_of(vt_spell_summon_skeleton);
    *reinterpret_cast<unsigned*>(object + 24) = 0;
    listOnewayLink__FP8ListHeadT0Pv(object + 24, light, light);
    return object;
}
