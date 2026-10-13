// Observed msg_run__7Critter at 0x001f0648, 2300 bytes.
// Notation: o is the object, w(k) the int word at o+k, A the AnimationState at o+288, setState(n)
// the call of virtual slot byte 184 of the table at o+72 with the int n, and "lift" the float at
// +1080.
// Entry. If the signed halfword at +572 is 12: word +236 = 1 and moved = gameLineUpdatePosition(
// o+960). When moved is nonzero the signed byte +977 chooses how the halfword +56 changes: nonzero
// copies the halfword +1000 into it, zero sets it to lerpShortRot(+56, +1000, float +1016 * 0.25,
// 100.0, -1.0). Any other state: everyFrame__8Creature(o, word +1036) returning nonzero ends the
// function, and so does objectActivationCheck(o, 600) returning zero; moved is 0. Then
// creatureAdvanceAnimation(o, 1.0) runs and the (re-read) halfword +572 picks the case below
// (unsigned compare against 17; the states 2..6, 8, 9, 11 and anything above 16 do nothing).
//  12: nothing when moved is zero. Otherwise vx = float +180 * 60 and vy = float +184 * 60; x (+16)
//      += vx * (1/60 single) and y (+20) += vy * (1/60 single); then, in soft doubles, speed =
//      |(double) sqrt(vy*vy + vx*vx)|, and float +24 = (float)((double)z + (double)(float +972) *
//      speed * 1/60), followed by objectUpdateInGrid(o) and float +60 = float +24.
//  0:  move2__8Creature(o, a zero Point3). If word +1048 is nonzero and w(1032) < w(808): s = w(1048),
//      or w(1052) instead when that is nonzero and an eRand roll is even; animAddOneShot(A, s,
//      0.1, 0.1, 2056); w(1032) = w(808) + w(1028) + eRand % 60; done. Otherwise, unless bit 8 of the
//      halfword +294 is set or w(1056) is zero: setState(1) when w(256) < w(808) or
//      bFarFromHome(o).
//  1:  moved = bit 2 of w(1076) ? steerToPosition(o, Point3 at +272) : steerAndRouteToPosition(o,
//      Point3 at +272) (nonzero counts). When moved or w(256) < w(808): with w(1044) nonzero,
//      setState(0); otherwise resetRouting, the Point3 at +272 = getSmartRandomDestination(o, 200)
//      and w(256) = w(808) + eRand % 120 + 180. Then, if bit 4 of w(1076) is set and
//      worldFindHeight(world, Point3 at +776, float +28) + 11.0 < float +24, the Point3 at +776 is
//      copied to +272.
//  7:  with w(576) nonzero: flee2(o, word at (w(576) + 12)); then nothing more unless w(256) <
//      w(808), in which case listDestroy(o+576), w(576) = 0 and setState(0 if w(1044) else 1).
//      With w(576) zero: setState(0 if w(1044) else 1).
//  10: objectRemoveFromRunList(o), then virtual slot byte 192 is called (no second argument).
//  13: if the low four bits of (frameNm ^ signed byte +569) are zero and any player's object (the
//      pointer at player[i] + 21872, stride 23984, i < nmPlayers) is within sqrt(w(1084)^2) of o
//      (squared distance below (float)(w(1084) * w(1084))): setState(14). Then, if w(1048) is
//      nonzero and w(1032) < w(808): the one-shot below with flags 8. Otherwise, if w(236) is zero and
//      w(256) < w(808): setState(14).
//  14: w(236) zero: objectAddToDeleteList(o). Otherwise: bit 1 of w(1076) adds the single 2.0 minus
//      one ulp to lift; float +60 = float +24 + lift; x += float +180; y += float +184;
//      objectUpdateInGrid(o).
//  15: if bit 2 of w(12) is set: only when w(236) is zero it is cleared, and the player test is
//      skipped; otherwise the player test of state 13 runs but calls setState(16). Then, if
//      w(1048) is nonzero and w(1032) < w(808): the one-shot below with flags 8.
//  16: w(236) zero: bit 2 of w(12) is set, the Point3 at +776 is copied to +16, objectUpdateInGrid(o),
//      setState(15). Otherwise the same movement as state 14 from "bit 1 of w(1076)" onwards, with
//      the single 2.0 minus one ulp.
// One-shot (states 0, 13 and 15): s as in state 0; animAddOneShot(A, s, c, c, flags) with c = 0.1 and
// flags 8 (13, 15) or 2056 (0); then w(1032) = w(808) + w(1028) + eRand % 60.
// Inferred: this is a wandering/fleeing small animal's per-frame body; the singles written as hex
// are the originals' (the compiler truncates decimals). Unknown: field and state names, and what
// the words +236, +256, +1032, +1044, +1048, +1052, +1056 and +1076 mean. Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

struct Point3 {
    float x, y, z;
};

extern "C" {
int gameLineUpdatePosition__FP17gameLinePathState(void* state);
short lerpShortRot__Fssfff(short a, short b, float c, float d, float e);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int distance);
void creatureAdvanceAnimation__8Creaturef(void* self, float value);
void objectUpdateInGrid__FP10GameObject(void* object);
void move2__8CreatureG6Point3(void* self, const Point3* point);
int steerToPosition__8CreatureG6Point3(void* self, const Point3* point);
int steerAndRouteToPosition__8CreatureG6Point3(void* self, const Point3* point);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float in, float out, int flags);
int eRand__10GameObject(void* self);
int bFarFromHome__8Creature(void* self);
void resetRouting__8Creature(void* self);
Point3* getSmartRandomDestination__8Creaturei(Point3* result, void* self, int radius);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const Point3* point, float value);
void flee2__8CreatureP10GameObject(void* self, void* object);
void listDestroy__FP8ListHead(void* head);
void objectRemoveFromRunList__FP10GameObject(void* object);
void objectAddToDeleteList__FP10GameObject(void* object);
unsigned long long fptodp(float value);
unsigned long long dpadd(unsigned long long a, unsigned long long b);
unsigned long long dpsub(unsigned long long a, unsigned long long b);
unsigned long long dpmul(unsigned long long a, unsigned long long b);
int dpcmp(unsigned long long a, unsigned long long b);
float dptofp(unsigned long long value);
extern int frameNm;
extern int nmPlayers;
extern unsigned world;
extern unsigned char player[];
}

namespace {
template <typename T>
inline T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }

inline void set_state(void* self, int state) { w10_actor_d::call_slot(self, 184, state); }

inline bool player_in_range(unsigned char* o) {
    const int count = nmPlayers;
    for (int i = 0; i < count; ++i) {
        unsigned char* other = w10_actor_d::word_at(player + 21872 + 23984 * i, 0);
        if (other == nullptr) continue;
        const float dx = field<float>(o, 16) - field<float>(other, 16);
        const float dy = field<float>(o, 20) - field<float>(other, 20);
        const float dz = field<float>(o, 24) - field<float>(other, 24);
        const int radius = field<int>(o, 1084);
        const float limit = static_cast<float>(radius * radius);
        if (dx * dx + dy * dy + dz * dz < limit) return true;
    }
    return false;
}

inline void one_shot(unsigned char* o, int flags, float rate) {
    void* header = w10_actor_d::word_at(o, 1048);
    if (field<int>(o, 1052) != 0) {
        if ((eRand__10GameObject(o) & 1) == 0) header = w10_actor_d::word_at(o, 1052);
    }
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, header, rate, rate, flags);
    const int roll = eRand__10GameObject(o);
    field<int>(o, 1032) = field<int>(o, 808) + field<int>(o, 1028) + static_cast<int>(static_cast<unsigned>(roll) % 60u);
}

inline void drift(unsigned char* o, float step) {
    if ((field<unsigned>(o, 1076) & 1u) != 0) field<float>(o, 1080) = field<float>(o, 1080) + step;
    field<float>(o, 60) = field<float>(o, 24) + field<float>(o, 1080);
    field<float>(o, 16) = field<float>(o, 16) + field<float>(o, 180);
    field<float>(o, 20) = field<float>(o, 20) + field<float>(o, 184);
    objectUpdateInGrid__FP10GameObject(o);
}
}  // namespace

extern "C" void msg_run__7Critter(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    int moved = 0;
    if (field<short>(o, 572) == 12) {
        field<int>(o, 236) = 1;
        moved = gameLineUpdatePosition__FP17gameLinePathState(o + 960);
        if (moved != 0) {
            if (static_cast<signed char>(o[977]) != 0) {
                field<unsigned short>(o, 56) = field<unsigned short>(o, 1000);
            } else {
                field<short>(o, 56) = lerpShortRot__Fssfff(field<short>(o, 56), field<short>(o, 1000), field<float>(o, 1016) * 0.25f, 100.0f, -1.0f);
            }
        }
    } else {
        if (everyFrame__8CreatureP12_modelHeader(self, w10_actor_d::word_at(o, 1036)) != 0) return;
        if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    }
    creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    switch (static_cast<unsigned short>(field<short>(o, 572))) {
        case 12: {
            if (moved == 0) return;
            const float vx = field<float>(o, 180) * 60.0f;
            const float vy = field<float>(o, 184) * 60.0f;
            const float z = field<float>(o, 24);
            field<float>(o, 16) = field<float>(o, 16) + vx * 0x1.111110p-6f;
            field<float>(o, 20) = field<float>(o, 20) + vy * 0x1.111110p-6f;
            const float lengthSquared = vy * vy + vx * vx;
            const unsigned long long zWide = fptodp(z);
            const unsigned long long scaleWide = fptodp(field<float>(o, 972));
            unsigned long long speed = fptodp(__builtin_sqrtf(lengthSquared));
            if (dpcmp(speed, 0ull) < 0) speed = dpsub(0ull, speed);
            unsigned long long step = dpmul(scaleWide, speed);
            step = dpmul(step, 0x3f91111111111111ull);
            field<float>(o, 24) = dptofp(dpadd(zWide, step));
            objectUpdateInGrid__FP10GameObject(o);
            field<float>(o, 60) = field<float>(o, 24);
            return;
        }
        case 0: {
            const Point3 zero = {0.0f, 0.0f, 0.0f};
            move2__8CreatureG6Point3(self, &zero);
            if (field<int>(o, 1048) != 0 && field<int>(o, 1032) < field<int>(o, 808)) {
                one_shot(o, 2056, 0.1f);
                return;
            }
            if ((field<unsigned short>(o, 294) & 8u) != 0) return;
            if (field<int>(o, 1056) == 0) return;
            if (field<int>(o, 256) < field<int>(o, 808) || bFarFromHome__8Creature(self) != 0) set_state(self, 1);
            return;
        }
        case 1: {
            const Point3 target = field<Point3>(o, 272);
            bool reached;
            if ((field<unsigned>(o, 1076) & 2u) != 0) reached = steerToPosition__8CreatureG6Point3(self, &target) != 0;
            else reached = steerAndRouteToPosition__8CreatureG6Point3(self, &target) != 0;
            if (reached || field<int>(o, 256) < field<int>(o, 808)) {
                if (field<int>(o, 1044) != 0) {
                    set_state(self, 0);
                } else {
                    resetRouting__8Creature(self);
                    Point3 destination;
                    getSmartRandomDestination__8Creaturei(&destination, self, 200);
                    field<Point3>(o, 272) = destination;
                    const int roll = eRand__10GameObject(self);
                    field<int>(o, 256) = field<int>(o, 808) + static_cast<int>(static_cast<unsigned>(roll) % 120u) + 180;
                }
            }
            if ((field<unsigned>(o, 1076) & 4u) != 0) {
                const Point3 probe = field<Point3>(o, 776);
                const float ground = worldFindHeight__FP12_worldHeaderG6Point3f(w10_actor_d::from_word(world), &probe, field<float>(o, 28));
                if (ground + 11.0f < field<float>(o, 24)) field<Point3>(o, 272) = field<Point3>(o, 776);
            }
            return;
        }
        case 7: {
            if (field<int>(o, 576) != 0) {
                flee2__8CreatureP10GameObject(self, w10_actor_d::word_at(w10_actor_d::word_at(o, 576), 12));
                if (!(field<int>(o, 256) < field<int>(o, 808))) return;
                listDestroy__FP8ListHead(o + 576);
                field<int>(o, 576) = 0;
            }
            set_state(self, field<int>(o, 1044) != 0 ? 0 : 1);
            return;
        }
        case 10:
            objectRemoveFromRunList__FP10GameObject(self);
            w10_actor_d::call_slot(self, 192, 0);
            return;
        case 13:
            if (((frameNm ^ static_cast<signed char>(o[569])) & 0xf) == 0 && player_in_range(o)) set_state(self, 14);
            if (field<int>(o, 1048) != 0 && field<int>(o, 1032) < field<int>(o, 808)) {
                one_shot(o, 8, 0.1f);
                return;
            }
            if (field<int>(o, 236) != 0) return;
            if (field<int>(o, 256) < field<int>(o, 808)) set_state(self, 14);
            return;
        case 14:
            if (field<int>(o, 236) == 0) {
                objectAddToDeleteList__FP10GameObject(self);
                return;
            }
            drift(o, 0x1.fffffep+0f);
            return;
        case 15: {
            const unsigned flags = field<unsigned>(o, 12);
            if ((flags & 2u) != 0) {
                if (field<int>(o, 236) == 0) field<unsigned>(o, 12) = flags & ~2u;
            } else if (((frameNm ^ static_cast<signed char>(o[569])) & 0xf) == 0 && player_in_range(o)) {
                set_state(self, 16);
            }
            if (field<int>(o, 1048) == 0) return;
            if (!(field<int>(o, 1032) < field<int>(o, 808))) return;
            one_shot(o, 8, 0.1f);
            return;
        }
        case 16:
            if (field<int>(o, 236) == 0) {
                field<unsigned>(o, 12) = field<unsigned>(o, 12) | 2u;
                field<Point3>(o, 16) = field<Point3>(o, 776);
                objectUpdateInGrid__FP10GameObject(self);
                set_state(self, 15);
                return;
            }
            drift(o, 0x1.fffffep+0f);
            return;
        default:
            return;
    }
}
