// Observed run__18SpellDiseaseShieldG6Point3s at 0x002958c0, 3004 bytes. O is the spell object, P a local copy of the
// Point3 argument (the short argument is not read), W = the object at word O+12, frameNm the global frame counter, G the
// float at O+428 (read again where the image reads it again), A the three floats at O+408. "unit vector" below always
// means a vector drawn inside the unit ball by rejection with eRand__5Spell(O) (each component float(roll) * 2^-16 doubled
// minus 1, drawn x, y, z in that order, redrawn while 1.0 < x*x + y*y + z*z). Steps in order:
//  1. Unless frameNm is below the signed word at O+420: entry 8 of the table whose address is the word at O+20 is called on
//     O (adjusted by the entry's signed halfword) with 3, and that ends it.
//  2. D = (W+16) - A, V = (D.x * 6.0, D.y * 6.0, 0.0). T = float(signed word at O+424). When G is below T, G gains 0.5.
//  3. Loop one runs (int)((G * 0.00083333) * (G * 3.14159)) times (re-evaluated after every pass), P_AddParticle__FP11ParticleDefP6Point3T1f
//     (record 0x008d69b0, position, velocity, 0.0) each with: u a unit vector, u.z *= 0.4, u *= G * 0.75; the velocity is u with
//     its z made non-negative and all three scaled by 0.4, plus V; the position is u + P with W's float at +32 times 0.25
//     subtracted from z.
//  4. Loop two runs (int)((G * 0.00041667) * (G * 3.14159)) times: a unit vector scaled by G * 0.33 and added to P is the
//     position; a second unit vector with z replaced by |z * 0.3|, scaled by 14.0 and added to V is the velocity; P_AddParticle
//     (record 0x008d6a00, position, velocity, 0.0).
//  5. Sixteen entries i = 0..15 of two arrays of Point3 (R at O+24+12i, Q at O+216+12i): a unit vector u scaled by 1.5; when
//     G * G * 0.8333 is below |R|^2, u gains R * -0.02; Q = (Q + u) * 0.8; R += Q; X = R + (W+16) with W's float at +32 times
//     0.75 added to z; P_FaceSprite__FG6Point3ffiiiiP8_texturesi(X, 30.0, 400.0, 0, 0, 0, 200, haloTexture5, 0, 0).
//  6. A becomes (W+16) (three floats). Every 30th frame (frameNm % 30 == 0), r = (int)G and the objects in the box P -/+ float(r)
//     are found with objectFindInBox__FG6Point3T0PP10GameObjecti (capacity 64). For each listed T whose W entry 160 (called on
//     W with T) is non-zero: a = the entry at table byte 104 of the table at the word at (the object at word O+8)+96, called on
//     that object with (0, the signed byte at O+0, W), as an int; a local item description (the shape the other handlers build:
//     halfwords at +16..+22 and +24..+30 zeroed, bytes at +32 and +33 = -1, four 8-byte slots at +36 starting -1) gets in
//     its first free slot {38, a, a, 0} (halfwords +0, +2, +4, +6); gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb
//     (T, W, float(a), (float(a) + float(a) * (float(r1) * 2^-16) * 0.2) * 0.5, 100000, 0, 0, 64, description, false), r1 being
//     eRand__5Spell(O). When at least one T was hit, lumpFindResource__FPcT0("fx.lmp", "spel_de_dishield_hit.anm") and
//     soundPlaySequence__FP15AnimationHeaderG6Point3f(that, (W+16), 1.0) run.
// The two particle records are file-local objects of the original unit, imported under address names. The image tests O for
// null before the entry 8 call; that test cannot be true here. Inferred: a disease-shield spell around its caster, with a
// periodic damaging pulse. Unknown: what the words at +0, +8, +420, +424 and the arrays mean. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* haloTexture5;
extern unsigned char diseaseshield_run_particle_8d69b0[];
extern unsigned char diseaseshield_run_particle_8d6a00[];
unsigned eRand__5Spell(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume);

namespace {
unsigned roll(unsigned char* o) { return eRand__5Spell(o); }
}  // namespace

void run__18SpellDiseaseShieldG6Point3s(void* self, Point3 where, short) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 p = where;
    if (!(frameNm < sword(o, 420))) {
        call_entry_from(o, 20, 8, 3, 0);
        return;
    }
    unsigned char* const w = from_word(word(o, 12));
    Point3 d;
    d.x = real(w, 16) - real(o, 408);
    d.y = real(w, 20) - real(o, 412);
    d.z = real(w, 24) - real(o, 416);
    Point3 e;
    e.x = d.x * 6.0f;
    e.y = d.y * 6.0f;
    e.z = d.z * 6.0f;
    Point3 v = e;
    v.z = 0.0f;
    const float t = static_cast<float>(sword(o, 424));
    float g = real(o, 428);
    if (g < t) {
        g = g + 0.5f;
        real(o, 428) = g;
    }
    // Loop one.
    if (static_cast<int>((g * bits(0x3a5a740du)) * (g * bits(0x40490fcfu))) > 0) {
        int pass = 0;
        do {
            Point3 u;
            unit_ball(o, u, roll);
            u.z = u.z * bits(0x3eccccccu);
            const float k = real(o, 428) * 0.75f;
            u.x = u.x * k;
            u.y = u.y * k;
            u.z = u.z * k;
            Point3 velocity = u;
            if (velocity.z < 0.0f) velocity.z = -velocity.z;
            velocity.x = velocity.x * bits(0x3eccccccu);
            velocity.y = velocity.y * bits(0x3eccccccu);
            velocity.z = velocity.z * bits(0x3eccccccu);
            u.x = u.x + p.x;
            u.y = u.y + p.y;
            u.z = u.z + p.z;
            u.z = u.z - real(from_word(word(o, 12)), 32) * 0.25f;
            velocity.x = velocity.x + v.x;
            velocity.y = velocity.y + v.y;
            velocity.z = velocity.z + v.z;
            ++pass;
            P_AddParticle__FP11ParticleDefP6Point3T1f(diseaseshield_run_particle_8d69b0, &u, &velocity, 0.0f);
            const float again = real(o, 428);
            if (!(pass < static_cast<int>((again * bits(0x3a5a740du)) * (again * bits(0x40490fcfu))))) break;
        } while (true);
    }
    // Loop two.
    if (static_cast<int>((real(o, 428) * bits(0x39da740du)) * (real(o, 428) * bits(0x40490fcfu))) > 0) {
        int pass = 0;
        do {
            Point3 u;
            unit_ball(o, u, roll);
            const float s = real(o, 428) * bits(0x3ea8f5c2u);
            u.z = u.z * s;
            u.x = u.x * s;
            u.y = u.y * s;
            u.x = u.x + p.x;
            u.y = u.y + p.y;
            u.z = u.z + p.z;
            Point3 velocity;
            unit_ball(o, velocity, roll);
            velocity.z = velocity.z * bits(0x3e999999u);
            if (velocity.z < 0.0f) velocity.z = -velocity.z;
            velocity.x = velocity.x * 14.0f;
            velocity.y = velocity.y * 14.0f;
            velocity.z = velocity.z * 14.0f;
            ++pass;
            velocity.x = velocity.x + v.x;
            velocity.y = velocity.y + v.y;
            velocity.z = velocity.z + v.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(diseaseshield_run_particle_8d6a00, &u, &velocity, 0.0f);
            const float again = real(o, 428);
            if (!(pass < static_cast<int>((again * bits(0x39da740du)) * (again * bits(0x40490fcfu))))) break;
        } while (true);
    }
    // Sixteen points.
    for (int i = 0; i < 16; ++i) {
        Point3 u;
        unit_ball(o, u, roll);
        unsigned char* const r = o + 24 + 12 * i;
        unsigned char* const q = o + 216 + 12 * i;
        const float gg = real(o, 428);
        const float threshold = gg * gg * bits(0x3f55554fu);
        u.x = u.x * 1.5f;
        u.y = u.y * 1.5f;
        u.z = u.z * 1.5f;
        if (threshold < real(r, 0) * real(r, 0) + real(r, 4) * real(r, 4) + real(r, 8) * real(r, 8)) {
            Point3 copy = *reinterpret_cast<Point3*>(r);
            Point3 f;
            f.x = copy.x * bits(0xbca3d70au);
            f.y = copy.y * bits(0xbca3d70au);
            f.z = copy.z * bits(0xbca3d70au);
            u.x = u.x + f.x;
            u.y = u.y + f.y;
            u.z = u.z + f.z;
        }
        real(q, 0) = real(q, 0) + u.x;
        real(q, 0) = real(q, 0) * bits(0x3f4cccccu);
        real(q, 4) = real(q, 4) + u.y;
        real(q, 4) = real(q, 4) * bits(0x3f4cccccu);
        real(q, 8) = real(q, 8) + u.z;
        real(q, 8) = real(q, 8) * bits(0x3f4cccccu);
        real(r, 0) = real(r, 0) + real(q, 0);
        real(r, 4) = real(r, 4) + real(q, 4);
        real(r, 8) = real(r, 8) + real(q, 8);
        unsigned char* const owner = from_word(word(o, 12));
        Point3 x;
        x.x = real(r, 0) + real(owner, 16);
        x.y = real(r, 4) + real(owner, 20);
        x.z = real(r, 8) + real(owner, 24);
        x.z = x.z + real(owner, 32) * 0.75f;
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(x, 30.0f, 400.0f, 0, 0, 0, 200, haloTexture5, 0, 0);
    }
    // Follow the caster and, every 30th frame, pulse.
    *reinterpret_cast<Point3*>(o + 408) = *reinterpret_cast<Point3*>(from_word(word(o, 12)) + 16);
    if (frameNm % 30 != 0) return;
    const float radius = static_cast<float>(static_cast<int>(real(o, 428)));
    Point3 low, high;
    low.x = p.x - radius;
    low.y = p.y - radius;
    low.z = p.z - radius;
    high.x = p.x + radius;
    high.y = p.y + radius;
    high.z = p.z + radius;
    void* found[64];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
    bool hit = false;
    if (found[0] != nullptr) {
        int index = 0;
        do {
            void* const target = found[index];
            if (call_entry(from_word(word(o, 12)), 160, to_word(static_cast<unsigned char*>(target))) != 0u) {
                const int amount = static_cast<int>(call_entry_from(from_word(word(o, 8)), 96, 104, 0,
                                                                    static_cast<unsigned long>(static_cast<long>(static_cast<signed char>(o[0]))), word(o, 12)));
                unsigned char description[80];
                for (int k = 0; k < 4; ++k) half(description, 60 - 8 * k) = -1;
                description[33] = 0xff;
                description[32] = 0xff;
                for (int k = 0; k < 4; ++k) {
                    half(description, 16 + 2 * k) = 0;
                    half(description, 24 + 2 * k) = 0;
                }
                for (int k = 0; k < 4; ++k) {
                    unsigned char* const slot = description + 36 + 8 * k;
                    if (half(slot, 0) == -1 || half(slot, 0) == 38) {
                        half(slot, 0) = 38;
                        half(slot, 6) = 0;
                        half(slot, 2) = static_cast<short>(amount);
                        half(slot, 4) = static_cast<short>(amount);
                        break;
                    }
                }
                float base = static_cast<float>(amount);
                const unsigned r1 = eRand__5Spell(o);
                float extra = base * (static_cast<float>(static_cast<int>(r1)) * bits(0x37800000u));
                extra = extra * bits(0x3e4cccccu);
                const float total = base + extra;
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(target, from_word(word(o, 12)), base, total * 0.5f, 100000, 0, 0, 64, description, false);
                hit = true;
            }
            ++index;
        } while (found[index] != nullptr);
    }
    if (hit) {
        void* const sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_de_dishield_hit.anm"));
        soundPlaySequence__FP15AnimationHeaderG6Point3f(sequence, *reinterpret_cast<Point3*>(from_word(word(o, 12)) + 16), 1.0f);
    }
}
}
