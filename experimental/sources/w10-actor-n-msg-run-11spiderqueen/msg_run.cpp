// Observed msg_run__11SpiderQueen at 0x0020b6f0, 5740 bytes. o is the object, R the block at the word
// object+36, "anim" the animation state at +288, s the halfword at +572 (the state), f the halfword at
// +294 (a flag set), W the member at +968 (a Web), B the record block [word +576 -> +12], t the word at
// +808, r = real28 the float at +28, b the result of creatureAdvanceAnimation (see below) and e = eRand.
// "setState(n)" is the table entry at (word +72)+184 called with n; "zone(d, k)" is
// enemyInZone(this, d, k). "turn" is turnToFaceEnemy.
// Common start: newDamageHandler(this, R[12]); everyFrame(this, R[12]) non-zero ends the call;
// objectActivationCheck(this, 1200); b = creatureAdvanceAnimation(this, 1.0) (kept in the image through a
// delay slot of the next call, as in msg_run__8SuperOrc); slideBack(this, 0.0); the table entry at +168
// is called with no arguments; Web::run(W, (x, y, z + 10), t); Web::draw(W, (x, y, z + 10)). Then s selects
// a case (s >= 21 and 2, 4, 7, 11, 12 do nothing before the common ending).
//  0: zone(600, 32767) or zone(600, 18000): setState(3); else when the word at +256 is below t:
//     setState(1).
//  1: zone(600, 32767) or zone(600, 18000): setState(3); else smartWander(this).
//  3: no word at +576: setState(0). Otherwise the first that holds: zone(600, 32767) and not zone(600,
//     16384): setState(20); zone(r + 30, 2000): setState(6); canGrab(this) and zone(450, 3000):
//     setState(13); canGrab and zone(225, 5000): setState(15); canGrab and zone(250, 5000): setState(16);
//     canGrab and zone(250, 6000): setState(17); zone(450, 3000): setState(18); else
//     steerAndRouteToEnemy(this).
//  5: the same list as 3 without the word test, ending with bExitStatePersueIdle(this) non-zero:
//     setState(3).
//  6: with a word at +576 and bit 0 of b: an item description is prepared and produceMeleeDamage(this,
//     r + 24, h200, h202, h204, 0x90004, description) runs (see "attack"). Then when bit 3 of f is
//     clear: setState(3).
//  8: when bit 4 of f is clear: setState(3).   9: when the signed word at +804 is below t: setState(0).
//  10: with bit 3 of b and bit 1 of the word at +12 clear: gameSetCameraShake(8.0, false),
//     floorAdd(point +16, (short)e, 100, 6, 1), soundFadeStream(0). Then, when bit 7 of f is set:
//     objectRemoveFromRunList(this).
//  13: with a word at +576, bit 5 of B's word at +12 and bit 0 of b: Web::start(W, (x, y, z + 10), B,
//     300, t). Then when W is not active: setState(3); else when the word at +980 is non-zero:
//     setState(14).
//  14: when W is not active: setState(3); otherwise, with a word at +576: when zone(r + 50, 2000):
//     breakWeb(W) and setState(15); then turn.
//  15: with no word at +576 only the ending below runs. Otherwise: bit 0 of b sets the word at +960
//     to 1; bit 2 of b clears it and, when bit 5 of B's word at +12 is set, clears B's word at +756.
//     With the word at +960 non-zero: the point is moved along the heading by (r + B's float +28 + 30)
//     ("approach"), and B's float +756 and halfword +748 follow it (see "approach" with factor 15).
//     Bit 1 of b: attack with flags 0x10004 (no zone test). Ending.
//  16, 17: with a word at +576 and bit 0 of b, when bit 5 of B's word at +12 is set and B's float +756
//     is 0, the word at +960 becomes 1. Then zone(250, 5000) with the word at +960 non-zero runs
//     "approach" (factor 6). Then bit 1 of b with zone(r + 30, 2000): attack with flags 0x10004. Then
//     when bit 3 of f is clear: setState(3), and with a word at +576 whose B has bit 5 of its word at
//     +12 set, B's word at +756 is cleared.
//  18: when bit 3 of f is clear: setState(3). With bit 0 of b: the point P = (x + 300 cos h, y + 300 sin
//     h, z) for the heading h (halfword +56) goes to findEnemyLeadPosition(this, 300, P); the spawn
//     point (x + 85 cos h, y + 85 sin h, z + 36) goes through modelGetCharAttachmentPos; then twice: a
//     heading toward P (clamped to +/-4000 around h, plus e % 2000 - 1000) gives an offset vector 300 *
//     (cos, sin) with the speed (e % 50 + 75) * distance / 300 in its z; a 168-byte block is allocated
//     and, if non-null, built as a PoisonProjectile with the spawn point, the vector, h - 16384 and
//     the float (e * 2^-16) * 0.5 + 0.5.
//  19: bit 0 of b: gameSetCameraShake(4.0, false); ending. 20: bit 0 of b flips the word at +960;
//     bit 1 of b: gameSetCameraShake(5.0, false); with the word at +960 and a word at +576: turn; ending.
// "Ending": when bit 3 of f is clear: setState(3). "Attack": the description (halfwords at 16..31 and
// bytes 32 and 33 as -1 or 0 as in the other StaticFire/Soul descriptions) gets, in its first free entry,
// 23, v and v + (int)(v * 0.2 * e * 2^-16) where v is the skill table entry skills[26] called through
// its table at +96 with (0, 1).
// Common finish: the first call builds the file-local particle record BloodDropParticle (once, guarded
// by a file-local flag) from the constants in the source; then for slot i = 2..7 (words at +1028 + 4i) a
// spray particle is added while the slot time exceeds t, or, once past and non-zero, one time in ten;
// while the slot time still exceeds t one time in twenty the point is dropped to the floor and a
// splash is added through floorAdd.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern int world;
extern unsigned skills[];
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
void objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float amount);
int slideBack__8Creaturef(void* self, float amount);
void run__3WebG6Point3i(void* web, Point3* position, int t);
void draw__3WebG6Point3(void* web, Point3* position);
void start__3WebG6Point3P8Creaturefi(void* web, Point3* position, void* creature, float f, int t);
int bActive__3Web(void* web);
void breakWeb__3Web(void* web);
int enemyInZone__8Creaturefs(void* self, float range, short a);
int canGrab__11SpiderQueen(void* self);
void smartWander__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float range, int a, int b, int c, int flags, void* description);
void gameSetCameraShake__Ffb(float amount, bool flag);
void floorAdd__FG6Point3sfii(Point3 position, short a, float size, int b, int c);
void soundFadeStream__Fi(int stream);
void objectRemoveFromRunList__FP10GameObject(void* object);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int index, Point3* out1, Point3* out2, float scale);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, Point3* position);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void* blockAlloc__Fi(int size);
void __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(
    void* self, void* map, int a, int b, int c, int d, Point3 position, Point3 vector, int e, float f, void* creature);
float worldFindPointHeight__FP12_worldHeaderG6Point3(void* world, Point3 position);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y, float z, int i, int j,
                                         int k, void (*callback)(void*));
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float extra);

using namespace w10n;

// Function-local statics of the image: the particle record (80 bytes, at 0x007d71c0) and the guard word.
static unsigned char BloodDropParticle[80];
static int bloodDropReady;

static inline float roll_component(void* self) {
    const float v = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u);
    return v + v - 1.0f;
}

static void set_state(unsigned char* o, int n) { call_entry(o, 184, static_cast<unsigned long>(n)); }

static void leave(unsigned char* o) {
    if ((static_cast<unsigned short>(half(o, 294)) & 8u) == 0u) set_state(o, 3);
}

static void prepare_description(unsigned char* d) {
    for (int i = 3; i >= 0; --i) half(d, 36 + 8 * i) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 4; ++i) {
        half(d, 16 + 2 * i) = 0;
        half(d, 24 + 2 * i) = 0;
    }
}

// Skill table entry 26 decides the damage value; the description gets it in its first free entry.
static void attack(void* self, unsigned char* o, unsigned flags) {
    unsigned char description[72];
    prepare_description(description);
    const int value = static_cast<int>(call_entry_from(from_word(skills[26]), 96, 96, 0, 1));
    float spread = static_cast<float>(value) * bits(0x3e4ccccc);
    const float fraction = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u);
    spread = spread * fraction;
    const int total = value + static_cast<int>(spread);
    for (int i = 0; i < 4; ++i) {
        const int mark = half(description, 36 + 8 * i);
        if (mark == -1 || mark == 23) {
            half(description, 36 + 8 * i) = 23;
            half(description, 42 + 8 * i) = 0;
            half(description, 38 + 8 * i) = static_cast<short>(value);
            half(description, 40 + 8 * i) = static_cast<short>(total);
            break;
        }
    }
    produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, real(o, 28) + 24.0f, half(o, 200), half(o, 202), half(o, 204), static_cast<int>(flags), description);
}

// Moves a point along the heading by (r + B's float +28 + 30), asks the model call to refine it, then
// stores the distance to B's point times the factor and the heading toward it in B.
static void approach(void* self, unsigned char* o, float factor) {
    Point3 point = *reinterpret_cast<Point3*>(o + 16);
    const float c = icos__Fi(half(o, 56));
    unsigned char* block = from_word(word(from_word(word(o, 576)), 12));
    point.x = point.x + c * ((real(o, 28) + real(block, 28)) + 30.0f);
    const float s = isin__Fi(half(o, 56));
    block = from_word(word(from_word(word(o, 576)), 12));
    point.y = point.y + s * ((real(o, 28) + real(block, 28)) + 30.0f);
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        from_word(word(from_word(word(o, 36)), 12)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 0, &point, nullptr, 1.0f);
    block = from_word(word(from_word(word(o, 576)), 12));
    if ((word(block, 12) & 0x20u) != 0u) {
        const float dx = real(block, 16) - point.x;
        const float dy = real(block, 20) - point.y;
        real(block, 756) = __builtin_sqrtf(dx * dx + dy * dy) * factor;
        const int heading = iatan2__Fff(real(block, 20) - point.y, real(block, 16) - point.x);
        half(block, 748) = static_cast<short>(heading - 32767);
    }
}

void msg_run__11SpiderQueen(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    newDamageHandler__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12)));
    if (everyFrame__8CreatureP12_modelHeader(self, from_word(word(from_word(word(o, 36)), 12))) != 0) return;
    objectActivationCheck__8Creaturei(self, 1200);
    unsigned char* const web = o + 968;
    const float lift = 10.0f;
    const int b = creatureAdvanceAnimation__8Creaturef(self, 1.0f);
    slideBack__8Creaturef(self, 0.0f);
    call_entry(o, 168);
    {
        Point3 p = {real(o, 16), real(o, 20), real(o, 24) + lift};
        run__3WebG6Point3i(web, &p, sword(o, 808));
    }
    {
        Point3 p = {real(o, 16), real(o, 20), real(o, 24) + lift};
        draw__3WebG6Point3(web, &p);
    }
    switch (static_cast<unsigned>(half(o, 572))) {
        case 0:
            if (enemyInZone__8Creaturefs(self, 600.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 600.0f, 18000) != 0) set_state(o, 3);
            else if (sword(o, 256) < sword(o, 808)) set_state(o, 1);
            break;
        case 1:
            if (enemyInZone__8Creaturefs(self, 600.0f, 32767) != 0 || enemyInZone__8Creaturefs(self, 600.0f, 18000) != 0) set_state(o, 3);
            else smartWander__8Creature(self);
            break;
        case 3:
        case 5: {
            const bool is3 = half(o, 572) == 3;
            if (is3 && word(o, 576) == 0u) { set_state(o, 0); break; }
            if (enemyInZone__8Creaturefs(self, 600.0f, 32767) != 0) {
                if (enemyInZone__8Creaturefs(self, 600.0f, 16384) == 0) { set_state(o, 20); break; }
            }
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 30.0f, 2000) != 0) { set_state(o, 6); break; }
            if (canGrab__11SpiderQueen(self) != 0 && enemyInZone__8Creaturefs(self, 450.0f, 3000) != 0) { set_state(o, 13); break; }
            if (canGrab__11SpiderQueen(self) != 0 && enemyInZone__8Creaturefs(self, 225.0f, 5000) != 0) { set_state(o, 15); break; }
            if (canGrab__11SpiderQueen(self) != 0 && enemyInZone__8Creaturefs(self, 250.0f, 5000) != 0) { set_state(o, 16); break; }
            if (canGrab__11SpiderQueen(self) != 0 && enemyInZone__8Creaturefs(self, 250.0f, 6000) != 0) { set_state(o, 17); break; }
            if (enemyInZone__8Creaturefs(self, 450.0f, 3000) != 0) { set_state(o, 18); break; }
            if (is3) steerAndRouteToEnemy__8Creature(self);
            else if (bExitStatePersueIdle__8Creature(self) != 0) set_state(o, 3);
            break;
        }
        case 6:
            if (word(o, 576) != 0u && (b & 1) != 0) attack(self, o, 0x90004u);
            leave(o);
            break;
        case 8:
            if ((static_cast<unsigned short>(half(o, 294)) & 0x10u) == 0u) set_state(o, 3);
            break;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(o, 0);
            break;
        case 10:
            if ((b & 8) != 0 && (word(o, 12) & 2u) == 0u) {
                gameSetCameraShake__Ffb(8.0f, false);
                const unsigned e = eRand__10GameObject(self);
                floorAdd__FG6Point3sfii(*reinterpret_cast<Point3*>(o + 16), static_cast<short>(e), 100.0f, 6, 1);
                soundFadeStream__Fi(0);
            }
            if ((static_cast<unsigned short>(half(o, 294)) & 0x80u) != 0u) objectRemoveFromRunList__FP10GameObject(self);
            break;
        case 13: {
            if (word(o, 576) != 0u) {
                unsigned char* const block = from_word(word(from_word(word(o, 576)), 12));
                if ((word(block, 12) & 0x20u) != 0u && (b & 1) != 0) {
                    Point3 p = {real(o, 16), real(o, 20), real(o, 24) + 10.0f};
                    start__3WebG6Point3P8Creaturefi(web, &p, block, 300.0f, sword(o, 808));
                }
            }
            if (bActive__3Web(web) == 0) { set_state(o, 3); break; }
            if (word(o, 980) != 0u) set_state(o, 14);
            break;
        }
        case 14:
            if (bActive__3Web(web) == 0) { set_state(o, 3); break; }
            if (word(o, 576) == 0u) break;
            if (enemyInZone__8Creaturefs(self, real(o, 28) + 50.0f, 2000) != 0) {
                breakWeb__3Web(web);
                set_state(o, 15);
            }
            turnToFaceEnemy__8Creature(self);
            break;
        case 15:
            if (word(o, 576) == 0u) { leave(o); break; }
            if ((b & 1) != 0) word(o, 960) = 1u;
            if ((b & 4) != 0) {
                unsigned char* const block = from_word(word(from_word(word(o, 576)), 12));
                word(o, 960) = 0u;
                if ((word(block, 12) & 0x20u) != 0u) word(block, 756) = 0u;
            }
            if (word(o, 960) != 0u) approach(self, o, 15.0f);
            if ((b & 2) != 0) attack(self, o, 0x10004u);
            leave(o);
            break;
        case 16:
        case 17: {
            if (word(o, 576) != 0u && (b & 1) != 0) {
                unsigned char* const block = from_word(word(from_word(word(o, 576)), 12));
                if ((word(block, 12) & 0x20u) != 0u && real(block, 756) == 0.0f) word(o, 960) = 1u;
            }
            if (enemyInZone__8Creaturefs(self, 250.0f, 5000) != 0 && word(o, 960) != 0u) approach(self, o, 6.0f);
            if ((b & 2) != 0 && enemyInZone__8Creaturefs(self, real(o, 28) + 30.0f, 2000) != 0) attack(self, o, 0x10004u);
            if ((static_cast<unsigned short>(half(o, 294)) & 8u) == 0u) {
                set_state(o, 3);
                if (word(o, 576) != 0u) {
                    unsigned char* const block = from_word(word(from_word(word(o, 576)), 12));
                    if ((word(block, 12) & 0x20u) != 0u) word(block, 756) = 0u;
                }
            }
            break;
        }
        case 18: {
            if ((static_cast<unsigned short>(half(o, 294)) & 8u) == 0u) set_state(o, 3);
            if ((b & 1) == 0) break;
            const float c = icos__Fi(half(o, 56));
            const float s = isin__Fi(half(o, 56));
            const float reach = 300.0f;
            Point3 target = {real(o, 16) + c * reach, real(o, 20) + s * reach, real(o, 24)};
            findEnemyLeadPosition__8CreaturefP6Point3(self, reach, &target);
            const float c2 = icos__Fi(half(o, 56));
            Point3 spawn;
            spawn.x = real(o, 16) + c2 * 85.0f;
            const float s2 = isin__Fi(half(o, 56));
            spawn.y = real(o, 20) + s2 * 85.0f;
            spawn.z = real(o, 24) + 36.0f;
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                from_word(word(from_word(word(o, 36)), 12)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, 0, &spawn, nullptr, 1.0f);
            for (int shot = 0; shot < 2; ++shot) {
                const int toward = iatan2__Fff(target.x - real(o, 16), target.y - real(o, 20));
                short aim = static_cast<short>(16384 - toward);
                const short current = half(o, 56);
                const short delta = static_cast<short>(aim - current);
                if (delta > 4000) aim = static_cast<short>(current + 4000);
                else if (delta < -4000) aim = static_cast<short>(current - 4000);
                aim = static_cast<short>(aim + static_cast<int>(eRand__10GameObject(self) % 2000u) - 1000);
                Point3 vector;
                vector.x = icos__Fi(aim) * reach;
                vector.y = isin__Fi(aim) * reach;
                vector.z = 0.0f;
                const float dy = real(o, 20) - target.y;
                const float dx = real(o, 16) - target.x;
                const float dz = real(o, 24) - target.z;
                const float ratio = __builtin_sqrtf((dx * dx + dy * dy) + dz * dz) * bits(0x3b5a740du);
                vector.z = (static_cast<float>(eRand__10GameObject(self) % 50u) + 75.0f) * ratio;
                void* const made = blockAlloc__Fi(168);
                if (made != nullptr) {
                    const float f = (static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u)) * 0.5f + 0.5f;
                    __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(made, from_word(word(o, 36)), 22, 23, 24, 26, spawn, vector,
                                                                                               half(o, 56) - 16384, f, self);
                }
            }
            break;
        }
        case 19:
            if ((b & 1) != 0) gameSetCameraShake__Ffb(4.0f, false);
            leave(o);
            break;
        case 20:
            if ((b & 1) != 0) word(o, 960) = word(o, 960) ^ 1u;
            if ((b & 2) != 0) gameSetCameraShake__Ffb(5.0f, false);
            if (word(o, 960) != 0u && word(o, 576) != 0u) turnToFaceEnemy__8Creature(self);
            leave(o);
            break;
        default:
            break;
    }

    if (bloodDropReady == 0) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(BloodDropParticle, 39, 108, 40, 53, 40, 87, 19, 0, 10.5f, bits(0x3f999999u), bits(0xc094ccccu), 3, 0, 6, nullptr);
        bloodDropReady = 1;
    }
    const Point3 zero = {0.0f, 0.0f, 0.0f};
    Point3 velocity = zero;
    for (int i = 2; i < 8; ++i) {
        unsigned* const slot = &word(o, 1028 + 4 * i);
        if (!(sword(o, 808) < static_cast<int>(*slot))) {
            if (*slot == 0u) continue;
            if (eRand__10GameObject(self) % 10u != 0u) continue;
        }
        Point3 start;
        Point3 spray;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            from_word(word(from_word(word(o, 36)), 12)), *reinterpret_cast<Point3*>(o + 16), half(o, 56), o + 288, i, &start, &spray, real(o, 224));
        float scale;
        spray.z = 0.0f;
        if (sword(o, 808) < static_cast<int>(*slot)) scale = static_cast<float>((static_cast<int>(*slot) - sword(o, 808)) / 20) * 36.0f;
        else scale = 16.0f;
        spray.x = spray.x * scale;
        spray.y = spray.y * scale;
        spray.z = spray.z * scale;
        Point3 ball;
        do {
            ball.x = roll_component(self);
            ball.y = roll_component(self);
            ball.z = roll_component(self);
        } while (1.0f < ball.x * ball.x + ball.y * ball.y + ball.z * ball.z);
        Point3 jitter = {ball.x + ball.x, ball.y + ball.y, ball.z + ball.z};
        spray.x = spray.x + jitter.x;
        spray.y = spray.y + jitter.y;
        spray.z = spray.z + jitter.z;
        velocity = spray;
        P_AddParticle__FP11ParticleDefP6Point3T1f(BloodDropParticle, &start, &velocity, 0.0f);
        if (!(sword(o, 808) < static_cast<int>(*slot))) continue;
        if (eRand__10GameObject(self) % 20u != 0u) continue;
        const float height = worldFindPointHeight__FP12_worldHeaderG6Point3(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)), start);
        Point3 splash = start;
        splash.z = height + 2.0f;
        const short angle = static_cast<short>(eRand__10GameObject(self));
        const unsigned second = eRand__10GameObject(self);
        const unsigned third = eRand__10GameObject(self);
        floorAdd__FG6Point3sfii(splash, angle, static_cast<float>(second & 7u) + 16.0f, static_cast<int>(third & 1u) + 4, 0);
    }
}
}
