// Observed msg_run__8SuperOrc at 0x0022a0a8, 3156 bytes. o is the object, R the block at the word
// object+36, "anim" the animation state at +288, s the halfword at +572 (the state) and f the
// halfword at +294 (a flag set). "setState(n)" is the table entry at (word +72)+184 called with n on
// the adjusted object. e = eRand(this).
// Common start:
//  newDamageHandler(this, R[12]); if everyFrame(this, R[12]) is non-zero the call ends. Then
//  objectActivationCheck(this, 600). Unless s is 13: b = creatureAdvanceAnimation(this, 1.0)
//  (b is kept; the image moves it into its register in the delay slot of the next call, so the
//  value is the advance call's result, not slideBack's); slideBack(this, 0.4); move2(this, (0, 0, 0));
//  when s is 13, b is 0. Then the table
//  entry at +168 is called with no arguments, and s (read again) selects a case (s >= 17, 2, 4,
//  7, 11 and 12 do nothing more).
//  0: when enemyInZone(this, 400, 32767) or enemyInZone(this, 500, 16000): setState(3).
//  1: when enemyInZone(this, 400, 32767) or enemyInZone(this, 500, 16000): setState(0); otherwise
//     smartWander(this).
//  3: with no word at +576: setState(0). Otherwise, with r = float(+28), the first zone test that
//     holds picks: enemyInZone(r + 50, 11000): setState(6) when e % 100 < 65 else setState(14);
//     enemyInZone(r + 90, 5500): setState(16) when e % 100 < 65 else setState(6);
//     enemyInZone(r + 70, 22000): setState(14) when e % 100 < 65 else setState(6);
//     enemyInZone(r + 220, 3000) with the frame counter (frameNm) not below the word at +968:
//     setState(13); otherwise steerAndRouteToEnemy(this).
//  5: with a word at +576: enemyInZone(r + 50, 11000): setState(6); then (word re-read)
//     enemyInZone(r + 70, 22000): setState(14). Otherwise, when bExitStatePersueIdle(this) is
//     non-zero: setState(3).
//  6, 14, 16: with no word at +576 only the final check below runs. Otherwise rounds for the bits
//     of b (6: bit 0; 16: bits 0 and 1; 14: bits 0, 1 and 2): the halfwords +1056..+1070 are set to
//     (110, 183, 255, 100, 110, 183, 255, 100) and produceMeleeDamage(this, range, h200, h202,
//     h204, flags, null) is called with flags 0x40008 when e % 100 < limit and 8 otherwise
//     (6: range 50, limit 50; 14: range 70, limit 40; 16: range 90, limit 40). Final check: when
//     bit 3 of f is clear: setState(3); otherwise turnToFaceEnemy(this) and again setState(3) when
//     bit 3 of f is then clear.
//  15: same final check, no rounds.
//  8: when bit 4 of f is clear: setState(3).
//  9: when the signed word at +804 is below the signed word at +808: setState(0).
//  10: when bit 7 of f is set: objectRemoveFromRunList(this).
//  13: with a word at +576 (a record whose word +12 points at a block holding a point at +16): the
//     frame counter + 180 goes to the word at +968; d is the x/y distance from the point at +16 to
//     that point. If d < 36: with t the float at +960, if t < 55: animAdvanceAnimationTime(anim,
//     55 - t, point +16, +180, halfword +188) gives b, the pair at +180 is zeroed, the halfword
//     at +188 is zeroed and t gains 55 - t. Otherwise (d >= 36, or t >= 55) with u the byte at
//     +800: q = 1.0 or, for 1 <= u <= 60, (60 - (signed u)) / 60; b = animAdvanceAnimationTime(anim,
//     q, ...); t gains q; the pair at +180 is scaled by the float at +224 and rotated by the angle
//     (halfword +56 minus 16384) as (-x*cos + y*sin, -x*sin - y*cos); for 1 <= u <= 60 it is then
//     scaled by (60 - u) / 60 and the halfword at +188 is multiplied by (int)((60 - u) / 60).
//     Then slideBack(this, 0.4), move2(this, 0). When b has bit 0 set and d < 50:
//     superorcSlamEffect(this, record word +12, 1), the halfwords +1056..+1070 are set as above, and
//     produceMeleeDamage(this, 50, 2*h200, 2*h202, h204, 0x100048 when the word at +964 is non-zero
//     else 0x80008, null). Then (this path) if bit 3 of f is set turnToFaceEnemy(this) and return if
//     bit 3 is still set. With no word at +576: creatureAdvanceAnimation(this, 1.0), slideBack(this,
//     0.4), move2(this, 0); return if bit 3 of f is set. Both paths end: when the word at +964 is
//     zero setState(3); otherwise setState(15) when e % 100 < 30, else setState(3).
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern int frameNm;
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
void objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
int slideBack__8Creaturef(void* self, float amount);
void move2__8CreatureG6Point3(void* self, Point3 offset);
int enemyInZone__8Creaturefs(void* self, float range, short a);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float range, int a, int b, int c, int flags, void* description);
int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* anim, float amount, void* position, void* pair, short* value);
void superorcSlamEffect__8SuperOrcP8Creaturei(void* self, void* target, int flag);
void objectRemoveFromRunList__FP10GameObject(void* object);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);

using namespace w10n;

static void set_colors(unsigned char* o) {
    half(o, 1066) = 183; half(o, 1058) = 183; half(o, 1064) = 110; half(o, 1068) = 255;
    half(o, 1070) = 100; half(o, 1056) = 110; half(o, 1060) = 255; half(o, 1062) = 100;
}

static void melee_round(void* self, unsigned char* o, float range, unsigned limit) {
    set_colors(o);
    if (eRand__10GameObject(self) % 100u < limit) {
        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, range, half(o, 200), half(o, 202), half(o, 204), 0x40008, nullptr);
    } else {
        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, range, half(o, 200), half(o, 202), half(o, 204), 8, nullptr);
    }
}

static void set_state(unsigned char* o, int n) { call_entry(o, 184, static_cast<unsigned long>(n)); }

// The shared ending of the attack states: face the enemy if needed, then leave the state.
static void leave(unsigned char* o) {
    if ((static_cast<unsigned short>(half(o, 294)) & 8u) == 0u) set_state(o, 3);
}

static void face_and_leave(void* self, unsigned char* o) {
    if ((static_cast<unsigned short>(half(o, 294)) & 8u) == 0u) { set_state(o, 3); return; }
    turnToFaceEnemy__8Creature(self);
    if ((static_cast<unsigned short>(half(o, 294)) & 8u) == 0u) set_state(o, 3);
}

static void decide(void* self, unsigned char* o) {
    if (word(o, 964) == 0u) { set_state(o, 3); return; }
    if (eRand__10GameObject(self) % 100u < 30u) set_state(o, 15);
    else set_state(o, 3);
}


void msg_run__8SuperOrc(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    void* const block_model = from_word(word(from_word(word(o, 36)), 12));
    newDamageHandler__8CreatureP12_modelHeader(self, block_model);
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    int b = 0;
    if (half(o, 572) != 13) {
        b = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
        slideBack__8Creaturef(self, bits(0x3ecccccc));
        const Point3 zero = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero);
    }
    call_entry(o, 168);
    switch (static_cast<unsigned>(half(o, 572))) {
        case 0:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 500.0f, 16000) != 0) set_state(o, 3);
            break;
        case 1:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 500.0f, 16000) != 0) set_state(o, 0);
            else smartWander__8Creature(self);
            break;
        case 3:
            if (word(o, 576) == 0u) { set_state(o, 0); break; }
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 50.0f, 11000) != 0) {
                set_state(o, eRand__10GameObject(self) % 100u < 65u ? 6 : 14);
            } else if (enemyInZone__8Creaturefs(self, real(o, 28) + 90.0f, 5500) != 0) {
                set_state(o, eRand__10GameObject(self) % 100u < 65u ? 16 : 6);
            } else if (enemyInZone__8Creaturefs(self, real(o, 28) + 70.0f, 22000) != 0) {
                set_state(o, eRand__10GameObject(self) % 100u < 65u ? 14 : 6);
            } else if (enemyInZone__8Creaturefs(self, real(o, 28) + 220.0f, 3000) != 0 && !(frameNm < sword(o, 968))) {
                set_state(o, 13);
            } else {
                steerAndRouteToEnemy__8Creature(self);
            }
            break;
        case 5:
            if (word(o, 576) != 0u) {
                if (enemyInZone__8Creaturefs(self, real(o, 28) + 50.0f, 11000) != 0) { set_state(o, 6); break; }
                if (word(o, 576) != 0u) {
                    if (enemyInZone__8Creaturefs(self, real(o, 28) + 70.0f, 22000) != 0) { set_state(o, 14); break; }
                }
            }
            if (bExitStatePersueIdle__8Creature(self) != 0) set_state(o, 3);
            break;
        case 6:
            if (word(o, 576) == 0u) { leave(o); break; }
            if ((b & 1) != 0) melee_round(self, o, 50.0f, 50u);
            face_and_leave(self, o);
            break;
        case 14:
            if (word(o, 576) == 0u) { leave(o); break; }
            if ((b & 1) != 0) melee_round(self, o, 70.0f, 40u);
            if ((b & 2) != 0) melee_round(self, o, 70.0f, 40u);
            if ((b & 4) != 0) melee_round(self, o, 70.0f, 40u);
            face_and_leave(self, o);
            break;
        case 16:
            if (word(o, 576) == 0u) { leave(o); break; }
            if ((b & 1) != 0) melee_round(self, o, 90.0f, 40u);
            if ((b & 2) != 0) melee_round(self, o, 90.0f, 40u);
            face_and_leave(self, o);
            break;
        case 15:
            if (word(o, 576) == 0u) leave(o);
            else face_and_leave(self, o);
            break;
        case 8:
            if ((static_cast<unsigned short>(half(o, 294)) & 0x10u) == 0u) set_state(o, 3);
            break;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(o, 0);
            break;
        case 10:
            if ((static_cast<unsigned short>(half(o, 294)) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
            break;
        case 13: {
            if (word(o, 576) == 0u) {
                creatureAdvanceAnimation__8Creaturef(self, 1.0f);
                slideBack__8Creaturef(self, bits(0x3ecccccc));
                const Point3 zero = {0.0f, 0.0f, 0.0f};
                move2__8CreatureG6Point3(self, zero);
                if ((static_cast<unsigned short>(half(o, 294)) & 8u) != 0u) return;
                decide(self, o);
                break;
            }
            unsigned char* const target = from_word(word(from_word(word(o, 576)), 12)) + 16;
            const float dy = real(o, 20) - real(target, 4);
            const float dx = real(o, 16) - real(target, 0);
            const float distance = __builtin_sqrtf(dx * dx + dy * dy);
            word(o, 968) = static_cast<unsigned>(frameNm + 180);
            float step = 0.0f;
            bool scaled = false;
            if (distance < 36.0f) {
                const float t = real(o, 960);
                if (t < 55.0f) {
                    step = 55.0f - t;
                    b = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(o + 288, step, o + 16, o + 180, reinterpret_cast<short*>(o + 188));
                    real(o, 180) = 0.0f;
                    real(o, 184) = 0.0f;
                    half(o, 188) = 0;
                    real(o, 960) = real(o, 960) + step;
                } else {
                    scaled = true;
                }
            } else {
                scaled = true;
            }
            if (scaled) {
                const unsigned char u = o[800];
                step = 1.0f;
                if (static_cast<unsigned>(u) - 1u < 60u) step = static_cast<float>(60 - static_cast<signed char>(u)) * bits(0x3c888888u);
                b = animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(o + 288, step, o + 16, o + 180, reinterpret_cast<short*>(o + 188));
                real(o, 960) = real(o, 960) + step;
                const float scale = real(o, 224);
                real(o, 180) = real(o, 180) * scale;
                real(o, 184) = real(o, 184) * scale;
                const int angle = half(o, 56) - 16384;
                const float c = icos__Fi(angle);
                const float s = isin__Fi(half(o, 56) - 16384);
                const float x = -real(o, 180);
                const float y = real(o, 184);
                const float rx = x * c + y * s;
                const float ry = x * s - y * c;
                real(o, 180) = rx;
                real(o, 184) = ry;
                const unsigned char v = o[800];
                if (static_cast<unsigned>(v) - 1u < 60u) {
                    const float g = static_cast<float>(60 - static_cast<signed char>(v)) * bits(0x3c888888u);
                    real(o, 184) = real(o, 184) * g;
                    real(o, 180) = real(o, 180) * g;
                    const unsigned short keep = static_cast<unsigned short>(half(o, 188));
                    half(o, 188) = static_cast<short>(keep * static_cast<int>(static_cast<float>(60 - static_cast<signed char>(o[800])) * bits(0x3c888888u)));
                }
            }
            slideBack__8Creaturef(self, bits(0x3ecccccc));
            const Point3 zero = {0.0f, 0.0f, 0.0f};
            move2__8CreatureG6Point3(self, zero);
            if ((b & 1) != 0 && distance < 50.0f) {
                const int h200 = half(o, 200);
                const int h202 = half(o, 202);
                unsigned char* const record = from_word(word(o, 576));
                superorcSlamEffect__8SuperOrcP8Creaturei(self, from_word(word(record, 12)), 1);
                set_colors(o);
                if (word(o, 964) != 0u) {
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 50.0f, h200 * 2, h202 * 2, half(o, 204), 0x100048, nullptr);
                } else {
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, 50.0f, h200 * 2, h202 * 2, half(o, 204), 0x80008, nullptr);
                }
            }
            if ((static_cast<unsigned short>(half(o, 294)) & 8u) != 0u) {
                turnToFaceEnemy__8Creature(self);
                if ((static_cast<unsigned short>(half(o, 294)) & 8u) != 0u) return;
            }
            decide(self, o);
            break;
        }
        default:
            break;
    }
}
}
