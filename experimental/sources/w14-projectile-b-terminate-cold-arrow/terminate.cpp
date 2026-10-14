// Observed terminate__19ColdArrowProjectilei at 0x0029f368, 1676 bytes. O is the projectile and h the integer argument; n is the
// signed word at O+104 (read again whenever it is used), H the signed halfword at O+100, r the single at O+28, B the pair of singles
// at O+84 and O+88, and the word at O+96 a done flag. Nothing happens when that word is non-zero. "Unit-ball draw" below is the
// rejection draw used by msg_run for this class: each component is float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y,
// z in that order, and the whole draw repeats while 1.0 < x*x + y*y + z*z. K is the single 0x3f34fdf3 (0.707) that the image keeps
// in the small-data area. Steps, in order:
//  1. h == 0 skips to step 4. For h == 1 only: C = (O.x + icos__Fi(H) * r, O.y + isin__Fi(H) * r, O.z) (icos first). For i = 0 up
//     to 10n + 27 (re-read each turn; nothing runs when that is not positive): D is a unit-ball draw; when D.x*B.x + D.y*B.y is
//     above K, D.x and D.y change sign; m = float(n / 3 + 2) (signed division); the position is C + D * m and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(pack, position, (0, 0, 0), 0.0) runs.
//  2. (h != 0) For j = 0 while float(j) < float(20n) * 0.5 (n re-read each turn): V is a unit-ball draw times 31.0; when
//     V.x*B.x + V.y*B.y is above K, V.x and V.y change sign; a second unit-ball draw E follows (its three singles only reach a
//     position O+16 + 2*E*r that is computed and then not used); P_AddParticle(coldSpeck, O+16, V, 0.0) runs with the address of
//     the object's own position. When (j & 3) == 0, V is multiplied by 0.75 and P_AddParticle(coldSmoke, O+16, V, 0.0) runs.
//  3. (h != 0) soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_coldarrow_hit.anm"),
//     O, 1.0) runs.
//  4. objectAddToDeleteList__FP10GameObject(O) runs and the word at O+96 becomes 1.
// Inferred: the shatter of a cold arrow at its end: a ring of frozen specks and smoke, with more for a larger level number.
// Unknown: what the particle definition named pack is for, why the second loop passes the object's own position, and the meaning
// of h beyond 0, 1 and other.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char pack[];
extern unsigned char coldSmoke[];
extern unsigned char coldSpeck[];
int eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectAddToDeleteList__FP10GameObject(void* object);

void terminate__19ColdArrowProjectilei(void* self, int hit) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    if (hit != 0) {
        const float kdot = bits(0x3f34fdf3u);
        if (hit == 1) {
            const short heading = half(o, 100);
            Point3 c;
            c.x = real(o, 16) + icos__Fi(heading) * real(o, 28);
            c.y = real(o, 20) + isin__Fi(heading) * real(o, 28);
            c.z = real(o, 24);
            for (int i = 0; i < sword(o, 104) * 10 + 27; ++i) {
                Point3 d;
                unit_ball(o, d, rand);
                if (kdot < d.x * real(o, 84) + d.y * real(o, 88)) {
                    d.x = -d.x;
                    d.y = -d.y;
                }
                const float m = static_cast<float>(sword(o, 104) / 3 + 2);
                Point3 position;
                position.x = c.x + d.x * m;
                position.y = c.y + d.y * m;
                position.z = c.z + d.z * m;
                Point3 still;
                still.x = 0.0f;
                still.y = 0.0f;
                still.z = 0.0f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(pack, &position, &still, 0.0f);
            }
        }
        for (int j = 0; static_cast<float>(j) < static_cast<float>(sword(o, 104) * 20) * 0.5f; ++j) {
            Point3 d;
            unit_ball(o, d, rand);
            Point3 v;
            v.x = d.x * 31.0f;
            v.y = d.y * 31.0f;
            v.z = d.z * 31.0f;
            if (kdot < v.x * real(o, 84) + v.y * real(o, 88)) {
                v.x = -v.x;
                v.y = -v.y;
            }
            Point3 e;
            unit_ball(o, e, rand);
            P_AddParticle__FP11ParticleDefP6Point3T1f(coldSpeck, reinterpret_cast<Point3*>(o + 16), &v, 0.0f);
            if ((j & 3) == 0) {
                v.x = v.x * 0.75f;
                v.y = v.y * 0.75f;
                v.z = v.z * 0.75f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(coldSmoke, reinterpret_cast<Point3*>(o + 16), &v, 0.0f);
            }
        }
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_coldarrow_hit.anm"), o, 1.0f);
    }
    objectAddToDeleteList__FP10GameObject(o);
    word(o, 96) = 1;
}
}
