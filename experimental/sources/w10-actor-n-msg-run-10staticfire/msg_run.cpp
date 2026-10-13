// Observed msg_run__10StaticFire at 0x0028e088, 2768 bytes. o is the object, L the light member at
// +128 (its destructor is called by _$_10StaticFire), t the word at +124 (a countdown), r the float
// at +100 and e = eRand(this). Steps in order:
//  1. The byte at +109 becomes 1 when strcmp(levelName + 81, "faydark3") is 0 and cutMode is
//     non-zero, otherwise 0.
//  2. When t is not -1: t == 0 calls lightDeactivate(L); otherwise t is decremented and, if it is
//     now 0, lightDeactivate(L); if it is below 120, r loses 1/120 (not below 0); else r gains 1/60
//     and is set to 1.0 when it exceeds 1.0 or the byte at +109 is non-zero.
//  3. When the signed byte at +108 is 0: the float at +76 gains 1.0 and, if its old value was
//     above 60.0, newWind(this, eRand(this)) runs and the word at +76 is cleared.
//  4. When t is non-zero and eRand(this) % 14 == 0: float +88 = float(+80) * 0.45 * 16 and float +92
//     = 1 / float(+88), then addSmokelet(this). updateSmokelets(this) always runs.
//  5. Only when the signed byte at +110 and t are non-zero and gamePositionVisible(point +16, 941.0)
//     holds:
//       - float +96: when it is 80.0 a roll % 10 == 0 adds 28.0; otherwise it loses 2.0 (not below 80.0);
//       - L floats 28 and 36 (object +156, +164) = (float(+96) * 0.34 + 54.0) * r; L floats 32 and 40
//         (object +160, +168) = 1 / (that * that);
//       - N = (int)(float(+84) * 4.0 * r) fire particles are added, each from a random unit-ball
//         direction scaled by float(+28), by r and by 0.693004 for the position offset, and a second
//         unit-ball direction scaled by 5.0 (z made non-negative) for the velocity;
//       - one roll in 33: (int)(r + r) spark particles (re-evaluated each pass) at a position offset
//         like the fire ones plus (0, 0, 12.0), with a unit-ball direction whose z is raised to
//         0.8, x and y scaled by 0.25, renormalized by a reciprocal square root and scaled by
//         ((roll * 2^-16) * 0.5 + 0.5) * 16;
//       - when (frameNm & 7) == 0: objectFindInBox over the cube of half-size float(+28) * r around
//         the point (64 entries), a stack item description is prepared (bytes 32 and 33 = -1, eight
//         halfwords at 16..31 = 0, four entries of 8 bytes at 36 with halfword -1; the first entry
//         whose halfword is -1 or 21 gets 21, halfwords 1, 1 and 0 at +2, +4 and +6), and every found
//         object other than this gets gameAttackResolver(found, this, 3.0, 3.0, 100000, 0, 0,
//         0x0100c040, description, false).
//  6. When t is 0 and the halfwords at +122 and +120 are equal, objectAddToDeleteList(this).
// The unit-ball draws are rejection sampled: three rolls (each (float)(int)roll * 2^-16, doubled,
// minus 1) until x*x + y*y + z*z <= 1.
// fireParticle (0x008d63c0) and sparkParticle (0x008d6460) are file-local records of the original;
// here they are unresolved data imports. Unknown: what the fields mean (fire size, fuel, light
// timing); no names are claimed beyond what the symbols say. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern char levelName[];
extern char cutMode;
extern int frameNm;
extern unsigned char norrath_w10_actor_n_view_8d63c0[];   // fireParticle in the original unit
extern unsigned char norrath_w10_actor_n_view_8d6460[];   // sparkParticle in the original unit
int strcmp(const char* a, const char* b);
unsigned eRand__10GameObject(void* self);
void lightDeactivate__FP5Light(void* light);
void newWind__10StaticFirei(void* self, int angle);
void addSmokelet__10StaticFire(void* self);
void updateSmokelets__10StaticFire(void* self);
int gamePositionVisible__FP6Point3f(Point3* position, float radius);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float extra);
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void objectAddToDeleteList__FP10GameObject(void* object);

namespace {
__attribute__((always_inline)) inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}

inline float roll_component(void* self) {
    const float v = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * w10n::bits(0x37800000u);
    return v + v - 1.0f;
}

inline Point3 unit_ball(void* self) {
    Point3 p;
    do {
        p.x = roll_component(self);
        p.y = roll_component(self);
        p.z = roll_component(self);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
    return p;
}
}  // namespace

void msg_run__10StaticFire(void* self) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const light = o + 128;

    o[109] = (strcmp(levelName + 81, "faydark3") == 0 && cutMode != 0) ? 1 : 0;

    if (sword(o, 124) != -1) {
        if (word(o, 124) == 0u) {
            lightDeactivate__FP5Light(light);
        } else {
            word(o, 124) = word(o, 124) - 1u;
            if (word(o, 124) == 0u) {
                lightDeactivate__FP5Light(light);
            } else if (sword(o, 124) < 120) {
                const float next = real(o, 100) - bits(0x3c088888u);
                real(o, 100) = next;
                if (next < 0.0f) real(o, 100) = 0.0f;
            } else {
                const float next = real(o, 100) + bits(0x3c888888u);
                real(o, 100) = next;
                if (1.0f < next || o[109] != 0) real(o, 100) = 1.0f;
            }
        }
    }

    if (static_cast<signed char>(o[108]) == 0) {
        const float old = real(o, 76);
        real(o, 76) = old + 1.0f;
        if (60.0f < old) {
            newWind__10StaticFirei(self, static_cast<int>(eRand__10GameObject(self)));
            word(o, 76) = 0u;
        }
    }

    if (word(o, 124) != 0u && eRand__10GameObject(self) % 14u == 0u) {
        const float size = real(o, 80) * bits(0x3ee66665u) * 16.0f;
        real(o, 88) = size;
        real(o, 92) = 1.0f / size;
        addSmokelet__10StaticFire(self);
    }
    updateSmokelets__10StaticFire(self);

    if (static_cast<signed char>(o[110]) != 0 && word(o, 124) != 0u && gamePositionVisible__FP6Point3f(reinterpret_cast<Point3*>(o + 16), bits(0x446b4000u)) != 0) {
        if (real(o, 96) == 80.0f) {
            if (eRand__10GameObject(self) % 10u == 0u) real(o, 96) = real(o, 96) + 28.0f;
        } else {
            const float next = real(o, 96) - 2.0f;
            real(o, 96) = next;
            if (next < 80.0f) real(o, 96) = 80.0f;
        }
        const float glow = (real(o, 96) * bits(0x3eae147au) + 54.0f) * real(o, 100);
        real(light, 28) = glow;
        real(light, 36) = glow;
        const float falloff = 1.0f / (glow * glow);
        real(light, 40) = falloff;
        real(light, 32) = falloff;

        int fire = static_cast<int>(real(o, 84) * 4.0f * real(o, 100));
        if (fire > 0) {
            do {
                const Point3 direction = unit_ball(self);
                const float spread = real(o, 28);
                const float strength = real(o, 100);
                Point3 position;
                position.x = real(o, 16) + (direction.x * spread * strength) * bits(0x3f3168b5u);
                position.y = real(o, 20) + (direction.y * spread * strength) * bits(0x3f3168b5u);
                position.z = real(o, 24) + (direction.z * spread * strength) * bits(0x3f3168b5u);
                const Point3 shake = unit_ball(self);
                Point3 velocity;
                velocity.x = shake.x * 5.0f;
                velocity.y = shake.y * 5.0f;
                velocity.z = shake.z * 5.0f;
                if (velocity.z < 0.0f) velocity.z = -velocity.z;
                P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w10_actor_n_view_8d63c0, &position, &velocity, 0.0f);
            } while (--fire != 0);
        }

        if (eRand__10GameObject(self) % 33u == 0u) {
            int count = static_cast<int>(real(o, 100) + real(o, 100));
            if (count > 0) {
                int made = 0;
                do {
                    const Point3 direction = unit_ball(self);
                    const float spread = real(o, 28);
                    const float strength = real(o, 100);
                    Point3 position;
                    position.x = real(o, 16) + (direction.x * spread * strength) * bits(0x3f3168b5u);
                    position.y = real(o, 20) + (direction.y * spread * strength) * bits(0x3f3168b5u);
                    position.z = real(o, 24) + (direction.z * spread * strength) * bits(0x3f3168b5u);
                    const Point3 lift = {0.0f, 0.0f, 12.0f};
                    Point3 start;
                    start.x = position.x + lift.x;
                    start.y = position.y;
                    start.z = position.z + lift.z;
                    Point3 velocity = unit_ball(self);
                    if (velocity.z < bits(0x3f4cccccu)) velocity.z = bits(0x3f4cccccu);
#if defined(__mips__)
                    // Keeps z * z a run-time multiply (as in the image) instead of a folded constant.
                    __asm__("" : "+f"(velocity.z));
#endif
                    ++made;
                    velocity.y = velocity.y * 0.25f;
                    velocity.x = velocity.x * 0.25f;
                    const float norm = reciprocal_sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
                    velocity.x = velocity.x * norm;
                    velocity.y = velocity.y * norm;
                    velocity.z = velocity.z * norm;
                    const float roll = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
                    const float speed = ((roll * bits(0x37800000u)) * 0.5f + 0.5f) * 16.0f;
                    velocity.x = velocity.x * speed;
                    velocity.y = velocity.y * speed;
                    velocity.z = velocity.z * speed;
                    P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w10_actor_n_view_8d6460, &start, &velocity, 0.0f);
                    count = static_cast<int>(real(o, 100) + real(o, 100));
                } while (made < count);
            }
        }

        if ((frameNm & 7) == 0) {
            const float reach = real(o, 28) * real(o, 100);
            Point3 low, high;
            low.x = real(o, 16) - reach; low.y = real(o, 20) - reach; low.z = real(o, 24) - reach;
            high.x = real(o, 16) + reach; high.y = real(o, 20) + reach; high.z = real(o, 24) + reach;
            void* found[64];
            objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
            unsigned char description[72];
            for (int i = 3; i >= 0; --i) half(description, 36 + 8 * i) = -1;
            description[33] = 0xff;
            description[32] = 0xff;
            for (int i = 0; i < 4; ++i) {
                half(description, 16 + 2 * i) = 0;
                half(description, 24 + 2 * i) = 0;
            }
            int slot = 0;
            for (int i = 0; i < 4; ++i) {
                const int mark = half(description, 36 + 8 * i);
                if (mark == -1 || mark == 21) {
                    half(description, 36 + 8 * i) = 21;
                    half(description, 42 + 8 * i) = 0;
                    half(description, 38 + 8 * i) = 1;
                    half(description, 40 + 8 * i) = 1;
                    break;
                }
                ++slot;
            }
            if (found[0] != nullptr) {
                for (void** entry = found; *entry != nullptr; ++entry) {
                    if (*entry != self) {
                        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(*entry, self, 3.0f, 3.0f, 100000, 0, 0, 0x0100c040, description, false);
                    }
                }
            }
        }
    }

    if (word(o, 124) == 0u && half(o, 122) == half(o, 120)) objectAddToDeleteList__FP10GameObject(self);
}
}
