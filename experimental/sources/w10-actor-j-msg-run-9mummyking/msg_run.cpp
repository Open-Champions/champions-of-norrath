// Observed msg_run__9MummyKing at 0x00239d68, 4040 bytes: the per-frame run routine of MummyKing. Offsets
// are bytes of the object; R is the block at the word +36 (R[n] is its word at byte n); the state is the
// halfword +572, t = word +808, "setState(n)" the table entry at (word +72)+184 called on the adjusted
// object with n, A the animation state at +288. Flag F is bit 26 (0x04000000) of the 64-bit word +160.
//  1. When F is set, the word +892 is filled once with findTagString(this, "ShowName", 0); otherwise it is
//     cleared.
//  2. With the word k at +960 non-zero, fxRunWeapon(description +1032, R[8k+4], state +964, copy of the point
//     +16, halfword +56, R[12], A, 0.5, state in {6, 13, 15, 16}, 0, 1.0, null) runs.
//  3. newDamageHandler(this, R[12]); a non-zero everyFrame(this, R[12]) ends the call; objectActivationCheck
//     (this, 600) (result unused); the result X of creatureAdvanceAnimation(this, 1.0) (only bit 0 is used);
//     slideBack(this, 0.2); the table entry at (word +72)+168 on the adjusted object.
//  4. States of 18 or more do nothing more. The cases (Z(r) = enemyInZone(this, r, 32767 or 4000 as listed),
//     B = (float +28) + 50, w(n) = the word +n, signed comparisons):
//   0: Z(400, 32767) -> setState(3).
//   1: Z(400, 32767) -> setState(3); else w256 < t -> setState(0); else smartWander.
//   3: w576 == 0 -> setState(0). Else when w1104 < t and bSpawnMummy -> setState(16). Then Z(B, 4000): when
//      w1152 < t setState(17), else setState(6) for an odd eRand and 13 for an even one. Else
//      alternateEnemyInZone(this, 0, B, halfword +56, 6000) gives the same choice; else bSpawnSnake ->
//      setState(15), bSpawnMummy -> setState(16), otherwise steerAndRouteToEnemy.
//   5: like 3 without the w576 test and without states 17: Z(B, 4000) or alternateEnemyInZone choose 6 or 13
//      by eRand parity, then bSpawnSnake -> 15, bSpawnMummy -> 16, bExitStatePersueIdle -> setState(3).
//   6: move2(this, zero); with w576 non-zero and X bit 0: a roll of eRand & 3 == 0 calls produceGeneralDamage
//      (this, 0, B, +56, 6000, h200, h202, h204, 0x80001, null), else with 0x40001; turnToFaceEnemy runs when
//      w576 is non-zero. Then setState(3) unless halfword +294 bit 3 is set.
//   8: unless halfword +294 bit 4 is set, setState(0).
//   9: w804 < t -> setState(3).   14: w256 < t -> setState(3).
//   10: with halfword +294 bit 7: soundFadeStream(0); objectRemoveFromRunList(this).
//   13: move2(this, zero); with w576 non-zero, halfwords 1048..1054 are set from byte +1160 (255,255,50 or
//       0,255,50 and 0), and with X bit 0 a damage description is rolled (see roll below) and
//       produceGeneralDamage(this, 0, B, +56, 6000, h200, h202, h204, 1, description) runs; turnToFaceEnemy
//       runs when X bit 0 was clear or after the damage call; then setState(3) unless +294 bit 3 is set.
//   15: halfwords 1048..1062 are set (see source); with X bit 0, up to one MummySnake per player is built at
//       a point offset from the position, linked into the slots +1136.., and R[132] is played.
//   16: spawns soldiers per player slot at +1108.. and one shaman at +1124 as the counter w1128 advances
//       (tier names chosen by the float +560 against the float +564 thirds), playing R[148], then
//       setState(3) once the counter passes the player count and the timing allows.
//   17: after w808 - w256 exceeds 48 it returns the king home (position +776) when further than the radius
//       float +788, else teleports it at random, and builds a DustDevil once; later at 120 it clears
//       the flags it set and calls setState(3).
// Roll for case 13: skill index 26 (byte +1160 non-zero, code 23) or 25 (code 38) called as (skill, 0,
// (int)worldDifficulty); the description entry gets the code, the rolled value, rolled + (int)(rolled * 0.2 *
// eRand * 2^-16), and 0.
// The roll's slot scan keeps the image's four-entry loop; entry 0 is always empty when it starts, so later
// entries are never reached. The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
extern float worldDifficulty;
extern unsigned skills[];
extern int nmPlayers;
extern unsigned char player[];
void* findTagString__10GameObjectPci(void* object, const char* tag, int index);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short a, void* model2, void* animation, float f, int g, int h,
    float i, void* matrix);
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short c, short d);
int bSpawnMummy__9MummyKing(void* self);
int bSpawnSnake__9MummyKing(void* self);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void move2__8CreatureG6Point3(void* self, Point3 delta);
void produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short c, short d, int e, int f, int g, int h, void* description);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void objectFindEmptySpot__FG6Point3iiPc(Point3* result, Point3 position, int a, int b, char* found);
void* blockAlloc__Fi(int size);
void spawn_mummy_construct(void* self, Point3 position, char* name, void* owner) __asm__("__10SpawnMummyG6Point3PcP10GameObject");
void mummy_snake_construct(void* self, Point3 position, short heading, void* owner, void* player, void* header)
    __asm__("__10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader");
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* item);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void soundFadeStream__Fi(int stream);
void objectRemoveFromRunList__FP10GameObject(void* object);
void objectUpdateInGrid__FP10GameObject(void* object);
void RandomTeleportSpell__FP10GameObjectP6Point3f(void* object, Point3* where, float range);
void dust_devil_construct(void* self, Point3 position) __asm__("__9DustDevilG6Point3");
}

namespace {
using namespace w10j;

struct Description {
    unsigned char raw[80];
};

unsigned roll_unit(unsigned char* o) { return eRand__10GameObject(o); }

// Fills a description like the Ant's roll: entries of four shorts at +36+8i, with -1 as empty code.
void roll_description(unsigned char* object, Description& description, unsigned skill_index, short code) {
    unsigned char* d = description.raw;
    for (int i = 3; i >= 0; --i) *reinterpret_cast<short*>(d + 36 + 8 * i) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<short*>(d + 16 + 2 * i) = 0;
        *reinterpret_cast<short*>(d + 24 + 2 * i) = 0;
    }
    unsigned char* skill = from_word(skills[skill_index]);
    unsigned char* e = from_word(word(skill, 96)) + 96;
    const int difficulty = static_cast<int>(worldDifficulty);
#if defined(W10_ACTOR_J_HOST)
    const int rolled = static_cast<int>(w10_actor_j_host_call(*reinterpret_cast<unsigned*>(e + 4), skill + *reinterpret_cast<short*>(e), 3, 0, difficulty));
#else
    const int rolled = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(
        skill + *reinterpret_cast<short*>(e), 0, difficulty);
#endif
    float amount = static_cast<float>(rolled) * bits(0x3e4ccccc);
    float unit = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * bits(0x37800000u);
    const int boosted = rolled + static_cast<int>(amount * unit);
    for (int i = 0; i < 4; ++i) {
        const short existing = *reinterpret_cast<short*>(d + 36 + 8 * i);
        if (existing == -1 || existing == code) {
            *reinterpret_cast<short*>(d + 36 + 8 * i) = code;
            *reinterpret_cast<short*>(d + 36 + 8 * i + 6) = 0;
            *reinterpret_cast<short*>(d + 36 + 8 * i + 2) = static_cast<short>(rolled);
            *reinterpret_cast<short*>(d + 36 + 8 * i + 4) = static_cast<short>(boosted);
            break;
        }
    }
}

void set_state(unsigned char* o, int state) { call_entry(o, 184, static_cast<unsigned long>(state)); }

// Names chosen by the current float +560 against the maximum float +564: above two thirds, above one
// third, otherwise.
const char* tier_name(unsigned char* o, const char* high, const char* middle, const char* low) {
    const float maximum = real(o, 564);
    const float current = real(o, 560);
    const float third = maximum * bits(0x3eaaaaaa);
    if (maximum - third < current) return high;
    if (maximum - (third + third) < current) return middle;
    return low;
}

// Finds an empty spot near the position; when none is found the point 50 along the heading is used.
void find_spot(unsigned char* o, Point3& out) {
    char found = 0;
    objectFindEmptySpot__FG6Point3iiPc(&out, *reinterpret_cast<Point3*>(o + 16), 200, 15, &found);
    if (found == 0) {
        const float x = real(o, 16) + icos__Fi(half(o, 56)) * 50.0f;
        const float y = real(o, 20) + isin__Fi(half(o, 56)) * 50.0f;
        out = Point3{x, y, real(o, 24)};
    }
}

void spawn_mummy(unsigned char* o, unsigned head_offset, const char* name) {
    Point3 spot;
    find_spot(o, spot);
    void* const mummy = blockAlloc__Fi(104);
    if (mummy != nullptr) spawn_mummy_construct(mummy, spot, const_cast<char*>(name), o);
    if (mummy != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + head_offset, mummy, mummy);
    unsigned char* const block = from_word(word(o, 36));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(block, 148)), o, 1.0f);
}

// 60 times the counter off the time word: the spawn timing of case 16.
bool timing_ok(unsigned char* o, int counter) { return sword(o, 256) < sword(o, 808) - 60 * counter; }
}  // namespace

extern "C" void msg_run__9MummyKing(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if ((dword(o, 160) & 0x04000000ull) != 0ull) {
        if (word(o, 892) == 0u) word(o, 892) = to_word(findTagString__10GameObjectPci(self, "ShowName", 0));
    } else {
        word(o, 892) = 0u;
    }
    unsigned char* block = from_word(word(o, 36));
    const unsigned weapon = word(o, 960);
    if (weapon != 0u) {
        unsigned char* const entry = block + weapon * 8u;
        const int state = half(o, 572);
        const int active = (state == 6 || state == 13 || state == 16 || state == 15) ? 1 : 0;
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1032, from_word(word(entry, 4)), o + 964, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(block, 12)), o + 288, 0.5f,
            active, 0, 1.0f, nullptr);
    }
    block = from_word(word(o, 36));
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(block, 12)));
    block = from_word(word(o, 36));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(block, 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 600);
    const unsigned advance = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, bits(0x3e4ccccc));
    call_entry(o, 168);
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) >= 18u) return;
    const float ahead = real(o, 28) + 50.0f;
    switch (state) {
        case 0:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) set_state(o, 3);
            return;
        case 1:
            if (enemyInZone__8Creaturefs(self, 400.0f, 32767) != 0) {
                set_state(o, 3);
            } else if (sword(o, 256) < sword(o, 808)) {
                set_state(o, 0);
            } else {
                smartWander__8Creature(self);
            }
            return;
        case 3:
            if (word(o, 576) == 0u) {
                set_state(o, 0);
                return;
            }
            if (sword(o, 1104) < sword(o, 808) && bSpawnMummy__9MummyKing(self) != 0) {
                set_state(o, 16);
                return;
            }
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 50.0f, 4000) != 0) {
                if (sword(o, 1152) < sword(o, 808)) {
                    set_state(o, 17);
                } else {
                    set_state(o, (static_cast<unsigned short>(eRand__10GameObject(self) & 1u) != 0) ? 6 : 13);
                }
                return;
            }
            if (alternateEnemyInZone__8Creatureffss(self, 0.0f, real(o, 28) + 50.0f, half(o, 56), 6000) != 0) {
                if (sword(o, 1152) < sword(o, 808)) {
                    set_state(o, 17);
                } else {
                    set_state(o, (static_cast<unsigned short>(eRand__10GameObject(self) & 1u) != 0) ? 6 : 13);
                }
                return;
            }
            if (bSpawnSnake__9MummyKing(self) != 0) {
                set_state(o, 15);
                return;
            }
            if (bSpawnMummy__9MummyKing(self) != 0) {
                set_state(o, 16);
                return;
            }
            steerAndRouteToEnemy__8Creature(self);
            return;
        case 5:
            if (sword(o, 1104) < sword(o, 808) && bSpawnMummy__9MummyKing(self) != 0) {
                set_state(o, 16);
                return;
            }
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 50.0f, 4000) != 0) {
                set_state(o, (static_cast<unsigned short>(eRand__10GameObject(self) & 1u) != 0) ? 6 : 13);
                return;
            }
            if (alternateEnemyInZone__8Creatureffss(self, 0.0f, real(o, 28) + 50.0f, half(o, 56), 6000) != 0) {
                set_state(o, (static_cast<unsigned short>(eRand__10GameObject(self) & 1u) != 0) ? 6 : 13);
                return;
            }
            if (bSpawnSnake__9MummyKing(self) != 0) {
                set_state(o, 15);
                return;
            }
            if (bSpawnMummy__9MummyKing(self) != 0) {
                set_state(o, 16);
                return;
            }
            if (bExitStatePersueIdle__8Creature(self) != 0) set_state(o, 3);
            return;
        case 6: {
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if (word(o, 576) != 0u) {
                if ((advance & 1u) != 0u) {
                    const unsigned e = eRand__10GameObject(self);
                    produceGeneralDamage__8CreatureffssiiiiPQ24item11description(self, 0.0f, real(o, 28) + 50.0f, half(o, 56), 6000, half(o, 200),
                                                                                  half(o, 202), half(o, 204), (e & 3u) == 0u ? 0x80001 : 0x40001, nullptr);
                }
                turnToFaceEnemy__8Creature(self);
            }
            if ((half(o, 294) & 8) == 0) set_state(o, 3);
            return;
        }
        case 8:
            if ((half(o, 294) & 0x10) == 0) set_state(o, 0);
            return;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(o, 3);
            return;
        case 10:
            if ((half(o, 294) & 0x80) != 0) {
                soundFadeStream__Fi(0);
                objectRemoveFromRunList__FP10GameObject(self);
            }
            return;
        case 13: {
            move2__8CreatureG6Point3(self, Point3{0.0f, 0.0f, 0.0f});
            if (word(o, 576) != 0u) {
                const unsigned char flag = o[1160];
                if (static_cast<signed char>(flag) != 0) {
                    half(o, 1050) = 255;
                    half(o, 1054) = 50;
                    half(o, 1048) = 0;
                } else {
                    half(o, 1050) = 255;
                    half(o, 1054) = 50;
                    half(o, 1048) = 255;
                }
                half(o, 1052) = 0;
                if ((advance & 1u) != 0u) {
                    Description description;
                    if ((static_cast<unsigned>(flag) << 24) != 0u) {
                        roll_description(o, description, 26, 23);
                    } else {
                        roll_description(o, description, 25, 38);
                    }
                    produceGeneralDamage__8CreatureffssiiiiPQ24item11description(self, 0.0f, real(o, 28) + 50.0f, half(o, 56), 6000, half(o, 200),
                                                                                  half(o, 202), half(o, 204), 1, description.raw);
                }
                turnToFaceEnemy__8Creature(self);
            }
            if ((half(o, 294) & 8) == 0) set_state(o, 3);
            return;
        }
        case 14:
            if (sword(o, 256) < sword(o, 808)) set_state(o, 3);
            return;
        case 15: {
            half(o, 1054) = 50;
            half(o, 1058) = 255;
            half(o, 1062) = 100;
            half(o, 1048) = 0;
            half(o, 1050) = 255;
            half(o, 1052) = 0;
            half(o, 1056) = 0;
            half(o, 1060) = 0;
            if ((advance & 1u) != 0u) {
                Point3 spot = *reinterpret_cast<Point3*>(o + 16);
                spot.x = spot.x + icos__Fi(-2000) * 24.0f;
                spot.y = spot.y + isin__Fi(-2000) * 24.0f;
                spot.z = spot.z + 48.0f;
                for (int i = 0; i < nmPlayers; ++i) {
                    unsigned char* const slot = o + 1136 + 4 * i;
                    unsigned char* const other = player + 21872 + 23984 * i;
                    if (word(slot, 0) != 0u) continue;
                    const unsigned target = word(other, 0);
                    if (target == 0u) continue;
                    if ((word(from_word(target), 12) & 8u) != 0u) continue;
                    void* const snake = blockAlloc__Fi(100);
                    if (snake != nullptr) {
                        mummy_snake_construct(snake, spot, half(o, 56), self, from_word(target), from_word(word(from_word(word(o, 36)), 140)));
                        listOnewayLink__FP8ListHeadT0Pv(slot, snake, snake);
                    }
                    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(from_word(word(o, 36)), 132)), self, 1.0f);
                }
            }
            if ((half(o, 294) & 8) == 0) set_state(o, 3);
            return;
        }
        case 16: {
            int i = sword(o, 1128);
            const int n = nmPlayers;
            if (i < n && timing_ok(o, i)) {
                if (word(o, 1108 + 4 * i) == 0u) {
                    spawn_mummy(o, 1108 + 4 * i, tier_name(o, "mummydesertsoldier", "mummytemplesoldier", "mummyelitesoldier"));
                    i = sword(o, 1128);
                }
                i = i + 1;
                sword(o, 1128) = i;
            }
            if (i == n && timing_ok(o, i)) {
                if (word(o, 1124) == 0u) {
                    spawn_mummy(o, 1124, tier_name(o, "mummydesertshaman", "mummytempleshaman", "mummyeliteshaman"));
                    i = sword(o, 1128);
                }
                i = i + 1;
                sword(o, 1128) = i;
            }
            if (n < i && timing_ok(o, i)) set_state(o, 3);
            return;
        }
        case 17: {
            const int elapsed = sword(o, 808) - sword(o, 256);
            if (word(o, 1156) == 0u) {
                if (bits(0x423fffff) < static_cast<float>(elapsed)) {
                    dword(o, 160) |= 0x20000000000ull | 0x800ull;
                    word(o, 12) |= 3u;
                    unsigned char* const here = o + 16;
                    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(from_word(word(from_word(word(o, 36)), 156)), self, 1.0f);
                    const float dx = real(o, 16) - real(o, 776);
                    const float dy = real(o, 20) - real(o, 780);
                    const float dz = real(o, 24) - real(o, 784);
                    const float radius = real(o, 788);
                    if (radius * radius < dx * dx + dy * dy + dz * dz) {
                        *reinterpret_cast<Point3*>(here) = *reinterpret_cast<Point3*>(o + 776);
                        objectUpdateInGrid__FP10GameObject(self);
                    } else {
                        const unsigned target = word(o, 576);
                        if (target != 0u) {
                            RandomTeleportSpell__FP10GameObjectP6Point3f(self, reinterpret_cast<Point3*>(from_word(word(from_word(target), 12)) + 16), 400.0f);
                        } else {
                            RandomTeleportSpell__FP10GameObjectP6Point3f(self, nullptr, 400.0f);
                        }
                    }
                    void* const devil = blockAlloc__Fi(88);
                    if (devil != nullptr) dust_devil_construct(devil, *reinterpret_cast<Point3*>(here));
                    word(o, 1156) = 1u;
                    word(o, 256) = word(o, 808);
                }
            } else if (120.0f < static_cast<float>(elapsed)) {
                word(o, 12) &= ~3u;
                dword(o, 160) &= ~(0x20000000000ull | 0x800ull);
                set_state(o, 3);
            } else if (bits(0x423fffff) < static_cast<float>(elapsed)) {
                word(o, 12) &= ~3u;
                dword(o, 160) &= ~(0x20000000000ull | 0x800ull);
            }
            return;
        }
        default:
            return;
    }
}
