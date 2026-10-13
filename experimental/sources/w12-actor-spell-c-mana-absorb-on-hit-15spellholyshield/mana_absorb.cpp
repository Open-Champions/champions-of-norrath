// Observed manaAbsorbOnHit__15SpellHolyShield at 0x002ae060, 864 bytes. S is the spell object; the word at +12 is
// the owner object P and the word at +8 the target object T (read by how they are used below). Returns a float.
// Steps in order:
//  1. Slot byte 128 of P's table (word at P+72; each entry is a signed halfword adjusting the object plus a
//     function word at +4) is called on P and its result is passed to modelGetNmShadowPoints__FP12_modelHeader;
//     the integer it returns is the count N. The same slot is called again; its result M is the model used next.
//  2. P_OpenParticleSystem__Fi(word at S+36).
//  3. 241 times: r = eRand__5Spell(S); modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f
//     (M, P+288, P's position copied from P+16, the signed halfword at P+56, r % N, &point, &extra, null, 1.0)
//     fills a point; then a direction D is drawn inside the unit ball by rejection (each component is
//     float(eRand__5Spell(S)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//     1.0 < x*x + y*y + z*z); and P_AddParticle__FP11ParticleDefP6Point3T1f is called with the particle record at
//     0x008d7720 (hshit in the symbol table, file-local), the point, D * 37.0 and the float 0.0.
//  4. P_CloseParticleSystem__Fi(word at S+36).
//  5. When the float at S+28 is not above 0.0: a 188-byte block is allocated with blockAlloc__Fi and, when it
//     is not null, __11LightEffectG6Point3ffffffffffffP10GameObjecti is called on it with the point
//     (P.x + 0.0, P.y, P.z + 60.0), the floats 64, 64, 125, 0, 0, 0, 0, 0, 120, 60, 0.5 and k (the float at pool
//     0x00482c38, bits 0x3e86a832), then a null object and zero. Whether or not it was allocated, the float
//     at S+28 then becomes the float at pool 0x00482c3c (the same bits).
//  6. Slot byte 88 of the table whose address is the word at T+96 (not +72) is called on T with the arguments (0, the signed byte at S+0) and its integer
//     result, converted to float and multiplied by the float at pool 0x00482c40 (bits 0x3c23d70a, 0.01 rounded),
//     is returned.
// The remainder r % N uses signed division. When N is zero the image's divide leaves an unspecified remainder
// (it has no divide check); that case is not reproduced. Inferred: the spell sprays particles from the owner's
// shadow points and lights the owner once; the slot at 88 is an absorb/hit callback. Unknown: the meaning of
// S+28 (a once-only flag is a guess from use), of the byte at S+0, and of the slot at 128. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern unsigned char holyshield_particle_8d7720[];
int eRand__5Spell(void* self);
void P_OpenParticleSystem__Fi(int system);
void P_CloseParticleSystem__Fi(int system);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* point, float* extra, void* matrix, float scale);
void* blockAlloc__Fi(int size);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const Point3* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float one, float scaled, void* object, int flag);

float manaAbsorbOnHit__15SpellHolyShield(void* self) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    const float scale = bits(0x37800000u);
    unsigned char* owner = from_word(word(s, 12));
    void* model = reinterpret_cast<void*>(call_entry(owner, 128));
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
    owner = from_word(word(s, 12));
    model = reinterpret_cast<void*>(call_entry(owner, 128));
    P_OpenParticleSystem__Fi(sword(s, 36));
    int i = 0;
    do {
        const int r = eRand__5Spell(s);
        owner = from_word(word(s, 12));
        Point3 position = *reinterpret_cast<Point3*>(owner + 16);
        Point3 point;
        float extra;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, owner + 288, position, half(owner, 56), r % count, &point, &extra, nullptr, 1.0f);
        Point3 d;
        do {
            float a = static_cast<float>(eRand__5Spell(s)) * scale;
            d.x = a + a - 1.0f;
            a = static_cast<float>(eRand__5Spell(s)) * scale;
            d.y = a + a - 1.0f;
            a = static_cast<float>(eRand__5Spell(s)) * scale;
            d.z = a + a - 1.0f;
        } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
        Point3 velocity;
        velocity.x = d.x * 37.0f;
        velocity.y = d.y * 37.0f;
        velocity.z = d.z * 37.0f;
        ++i;
        P_AddParticle__FP11ParticleDefP6Point3T1f(holyshield_particle_8d7720, &point, &velocity, 0.0f);
    } while (i < 241);
    P_CloseParticleSystem__Fi(sword(s, 36));
    if (real(s, 28) <= 0.0f) {
        void* const block = blockAlloc__Fi(188);
        if (block != nullptr) {
            owner = from_word(word(s, 12));
            Point3 at;
            at.x = real(owner, 16) + 0.0f;
            at.y = real(owner, 20);
            at.z = real(owner, 24) + 60.0f;
            __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, &at, 64.0f, 64.0f, 125.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 120.0f, 60.0f, 0.5f,
                                                              bits(0x3e86a832u), nullptr, 0);
        }
        real(s, 28) = bits(0x3e86a832u);
    }
    unsigned char* const target = from_word(word(s, 8));
    const int absorbed = static_cast<int>(call_entry_from(target, 96, 88, 0, static_cast<unsigned long>(static_cast<signed char>(s[0]))));
    return static_cast<float>(absorbed) * bits(0x3c23d70au);
}
}
