// Observed hitSomething__11SpellChargeP10GameObject at 0x00291738, 1596 bytes. S is the spell, X the object it hit
// (null when nothing was hit). The word at S+12 is the owner object (O), the word at S+8 the target object (T) and the
// signed byte at S+0 a value G. Nothing happens when the word at S+72 is non-zero. Otherwise, in order:
//  1. lumpFindResource__FPcT0("fx.lmp", "spel_bw_bull_hit.anm") is played on O with soundPlaySequence__FP15AnimationHeaderP10GameObjectf
//     (volume 1.0) and gameSetCameraShake__Ffb(10.0, false) is called.
//  2. X null: eight times, a point (O.x - 6 + float(eRand__5Spell(S) % 12), O.y - 6 + float(eRand % 12),
//     (O.z + 36 - 36) + float(eRand % 72)) goes to addEffect__FiG6Point3(2, point) (remainders unsigned, the z sum
//     in the order written). Then step 6.
//  3. X not null: A = slot byte 88 of the table at word (T + 96) called on T with (0, G); B = the same with (2, G).
//     When the flag word at X+12 has bit 0x40, slot byte 16 of X's table (word at +72) is called on X with (O, 0x40000,
//     0.0); otherwise a = float(A) and gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(X, O, a,
//     a + a * (float(eRand__5Spell(S)) * 2^-16) * k, 100000, 0, 0, 0x8c040, null, false) runs (k is the float at
//     gp - 21240, bits 0x3e4ccccc).
//  4. R = float(12 * B). When R is zero go to step 6. Otherwise a 128-entry box query around O's position
//     (O.x - R .. O.x + R, likewise y and z) fills a list with objectFindInBox__FG6Point3T0PP10GameObjecti, and
//     C = the slot byte 88 call on T with (1, G). Each listed object L, until the first null: skipped when it is O, X or
//     has flag bit 0x8; when it has flag bit 0x40, slot byte 16 is called as above; otherwise the damage call is made
//     on L with C in place of A (the float at gp - 21236 is the same 0x3e4ccccc).
//  5. A ring of particles: for angle 0, 500, ... while angle <= 65535: a point (O.x + icos(angle) * R, O.y + isin(angle)
//     * R, O.z + 5.0) and a velocity from a rejection-sampled unit-ball direction D (x, y, z drawn in that order by
//     eRand__5Spell, each float(roll) * 2^-16 doubled minus 1, redrawn while 1.0 < x*x + y*y + z*z) are made: the
//     velocity is D * 10.0 with its third component replaced by float(eRand) * 2^-16 * 5.0 + 10.0. Two
//     P_AddParticle__FP11ParticleDefP6Point3T1f calls follow with the particle record at 0x008d6780 (chargeHit,
//     file-local) and the float 0.0: the first at that point, the second at (O.x + icos(angle) * R * 0.5, O.y +
//     isin(angle) * R * 0.5, same z) with the same velocity.
//  6. The word at S+72 becomes 1.
// Inferred: this is the impact handler of a charge spell, damaging the hit object and everything else in a radius that
// scales with B (a skill value). Unknown: what A, B and C mean (the slot returns three different numbers for the
// modes 0, 2 and 1), which class slot 16 is, and the item description argument (null here). Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned char spellcharge_hit_something_particle_8d6780[];
int eRand__5Spell(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void gameSetCameraShake__Ffb(float amount, bool flag);
void addEffect__FiG6Point3(int effect, Point3 position);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* target, void* attacker, float low, float high, int a, int b, int c, int d,
                                                                      void* description, bool flag);

namespace {
// Slot byte 16 of the object's table (word at +72), called on the adjusted object with the owner word, 0x40000 and the
// float 0.0 (the float argument is not part of the host contract's call record).
void call_slot16(unsigned char* o, unsigned owner_word) {
    using namespace w12c;
    unsigned char* const e = table_entry(o, 16);
    unsigned char* const adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W12_SPELL_C_HOST)
    w12_spell_c_host_call(fn, adjusted, 2, owner_word, 0x40000ul);
#else
    reinterpret_cast<void (*)(void*, unsigned long, unsigned long, float)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, owner_word, 0x40000ul, 0.0f);
#endif
}

void damage(unsigned char* spell, void* victim, unsigned char* owner, float amount) {
    using namespace w12c;
    const float roll = static_cast<float>(eRand__5Spell(spell)) * bits(0x37800000u);
    const float spread = amount * roll * bits(0x3e4cccccu);
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(victim, owner, amount, amount + spread, 100000, 0, 0, 0x8c040, nullptr, false);
}
}  // namespace

void hitSomething__11SpellChargeP10GameObject(void* self, void* hit) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    unsigned char* const x = static_cast<unsigned char*>(hit);
    if (word(s, 72) != 0u) return;
    void* const sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_bw_bull_hit.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sequence, from_word(word(s, 12)), 1.0f);
    gameSetCameraShake__Ffb(10.0f, false);
    if (x == nullptr) {
        for (int i = 0; i < 8; ++i) {
            Point3 p;
            p.x = real(from_word(word(s, 12)), 16) - 6.0f + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__5Spell(s)) % 12u));
            p.y = real(from_word(word(s, 12)), 20) - 6.0f + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__5Spell(s)) % 12u));
            const float lift = 36.0f;
            p.z = real(from_word(word(s, 12)), 24) + lift - lift + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__5Spell(s)) % 72u));
            addEffect__FiG6Point3(2, p);
        }
        word(s, 72) = 1u;
        return;
    }
    const long long g = static_cast<signed char>(s[0]);
    const int a = static_cast<int>(call_entry_from(from_word(word(s, 8)), 96, 88, 0, static_cast<unsigned long>(g)));
    const int b = static_cast<int>(call_entry_from(from_word(word(s, 8)), 96, 88, 2, static_cast<unsigned long>(g)));
    const int radius_units = b * 12;
    if ((word(x, 12) & 0x40u) != 0u) {
        call_slot16(x, word(s, 12));
    } else {
        damage(s, x, from_word(word(s, 12)), static_cast<float>(a));
    }
    if (radius_units != 0) {
        const float r = static_cast<float>(radius_units);
        unsigned char* const owner = from_word(word(s, 12));
        Point3 low;
        Point3 high;
        low.x = real(owner, 16) - r;
        low.y = real(owner, 20) - r;
        low.z = real(owner, 24) - r;
        high.x = real(owner, 16) + r;
        high.y = real(owner, 20) + r;
        high.z = real(owner, 24) + r;
        void* list[128];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
        const int c = static_cast<int>(call_entry_from(from_word(word(s, 8)), 96, 88, 1, static_cast<unsigned long>(g)));
        for (int i = 0; list[i] != nullptr; ++i) {
            unsigned char* const other = static_cast<unsigned char*>(list[i]);
            if (other == from_word(word(s, 12))) continue;
            if (other == x) continue;
            const unsigned flags = word(other, 12);
            if ((flags & 0x8u) != 0u) continue;
            if ((flags & 0x40u) != 0u) call_slot16(other, word(s, 12));
            else damage(s, other, from_word(word(s, 12)), static_cast<float>(c));
        }
        const float radius = r;
        for (int angle = 0; angle <= 0xffff; angle += 500) {
            Point3 p;
            p.x = real(from_word(word(s, 12)), 16) + icos__Fi(angle) * radius;
            p.y = real(from_word(word(s, 12)), 20) + isin__Fi(angle) * radius;
            p.z = real(from_word(word(s, 12)), 24) + 5.0f;
            Point3 d;
            do {
                float t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
                d.x = t + t - 1.0f;
                t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
                d.y = t + t - 1.0f;
                t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
                d.z = t + t - 1.0f;
            } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
            Point3 v;
            v.x = d.x * 10.0f;
            v.y = d.y * 10.0f;
            v.z = d.z * 10.0f;
            v.z = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u) * 5.0f + 10.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(spellcharge_hit_something_particle_8d6780, &p, &v, 0.0f);
            p.x = real(from_word(word(s, 12)), 16) + icos__Fi(angle) * radius * 0.5f;
            p.y = real(from_word(word(s, 12)), 20) + isin__Fi(angle) * radius * 0.5f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(spellcharge_hit_something_particle_8d6780, &p, &v, 0.0f);
        }
    }
    word(s, 72) = 1u;
}
}
