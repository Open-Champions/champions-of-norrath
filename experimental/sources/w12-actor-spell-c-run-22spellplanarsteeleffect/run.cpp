// Observed run__22SpellPlanarSteelEffectG6Point3s at 0x002c8da8, 2616 bytes. Arguments: the effect S, a position P (by
// value) and a short that is never read. S+12 is the owner object O (read each time), S+36 the end frame, S+28 a
// signed halfword countdown, S+32 a done-once word, S+24 and S+26 two angle halfwords, S+20 the word that holds the
// effect's own slot table, and S+48.. an embedded light (its position is the Point3 at S+96). Steps, in order:
//  1. When frameNm is not below the end frame (signed word at +36), the effect ends: when S is not null, slot byte 8 of the
//     table at word S+20 is called on S with the argument 3.
//  2. v = isin__Fi(frameNm * 1000). O's position (+16..+24) is copied to S+96 and lightUpdatePosition__FP5Light(S + 48)
//     is called. With g = float(signed byte at S+0 + 66): the floats at +76 and +84 become g + v * 10; those at +80
//     and +88 become 1.0 / (that value squared). Halfwords of O: +152 = +144 = (int)(v * 49 + 200), +154 = +146 =
//     (int)(v * 5 + 15), +150 = +140 = +148 = +142 = (int)(v * 49 + 75).
//  3. The halfword at +28 is decremented. When it has become negative and the word at +32 is zero: the word at +32 becomes
//     1, sixty particles are added (angle = 0, 1092, ... as a short): a velocity (icos(angle) * 100, isin(angle) * 100,
//     float(eRand__5Spell(S)) * 2^-16 * 20) at the point (O.x + 0.0, O.y, O.z + 18.0) with the record 0x008d85b0
//     (planarsteeleffectParticle3), then lumpFindResource__FPcT0("fx.lmp", "spel_fire_hit.anm") is played at O's position
//     with soundPlaySequence__FP15AnimationHeaderG6Point3f(volume 1.0).
//  4. Twice (for the angle halfwords at +24 and +26 in turn): the angle falls by 1500; E = O's position + (icos(angle) * 40,
//     isin(angle) * 40, float(signed halfword +28)); a sprite is drawn with P_FaceSprite__FG6Point3ffiiiiP8_texturesi(E, 0.0,
//     4000.0, c, c, d, c, haloTexture5, 0, 2) where c = (int)(v * 40 + 50) and d = (int)(v * 40 + 100); then six particles with the
//     record 0x008d8510 (planarsteeleffectParticle1): a unit-ball direction D1 (each component float(eRand__5Spell(S)) * 2^-16
//     doubled minus 1, drawn x, y, z, redrawn while 1.0 < x*x + y*y + z*z) times the pool float 0x3ecccccc is the velocity and
//     twice a second direction D2 plus E is the point; then four particles with the record 0x008d8560 (planarsteeleffectParticle2):
//     velocity D1 * 6.0, point 2 * D2 + E.
//  5. Ten times: two rolls (the first is a short angle, the second float(roll) * 2^-16 * 40 is the radius): a point (icos *
//     radius + P.x, isin * radius + P.y, 200 + float(eRand % 200) - 100 + P.z) and the velocity (0, 0, -80) go to the record
//     0x008d8600 (planarsteeleffectParticle5).
//  6. Ten times: the same angle/radius pair gives (icos * radius, isin * radius, 0) plus P; the velocity is ((roll & 1) - 1, (roll & 1) - 1, 0);
//     the point's z becomes worldFindPointHeight__FP12_worldHeaderG6Point3(world, that point) + 5.0; a particle goes to the record
//     0x008d8510.
// Inferred: this plays the visual of a falling-blade effect around the owner. Unknown: the meaning of the S fields beyond
// their use and the owner halfwords at +140..154 (they look like paired colour values). Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern void* haloTexture5;
extern unsigned char planarsteel_run_particle_8d8510[];
extern unsigned char planarsteel_run_particle_8d8560[];
extern unsigned char planarsteel_run_particle_8d85b0[];
extern unsigned char planarsteel_run_particle_8d8600[];
int eRand__5Spell(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void lightUpdatePosition__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* sequence, Point3 position, float volume);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);
float worldFindPointHeight__FP12_worldHeaderG6Point3(void* world, Point3 position);

namespace {
using namespace w12c;

void draw_direction(unsigned char* s, Point3& d) {
    do {
        float t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        d.x = t + t - 1.0f;
        t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        d.y = t + t - 1.0f;
        t = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u);
        d.z = t + t - 1.0f;
    } while (1.0f < d.x * d.x + d.y * d.y + d.z * d.z);
}
}  // namespace

void run__22SpellPlanarSteelEffectG6Point3s(void* self, Point3 position, short unused) {
    (void)unused;
    unsigned char* const s = static_cast<unsigned char*>(self);
    const int frame = frameNm;
    if (!(frame < sword(s, 36))) {
        if (s != nullptr) call_entry_from(s, 20, 8, 3, 0);
        return;
    }
    const float v = isin__Fi(frame * 1000);
    {
        unsigned char* const owner = from_word(word(s, 12));
        *reinterpret_cast<Point3*>(s + 96) = *reinterpret_cast<Point3*>(owner + 16);
    }
    lightUpdatePosition__FP5Light(s + 48);
    {
        unsigned char* const owner = from_word(word(s, 12));
        const float g = static_cast<float>(static_cast<signed char>(s[0]) + 66);
        const float v49 = v * 49.0f;
        const float level = g + v * 10.0f;
        const float tilt = v * 5.0f;
        const float lowc = v49 + 200.0f;
        const float midc = tilt + 15.0f;
        const float highc = v49 + 75.0f;
        const float squared = level * level;
        const int a1 = static_cast<int>(lowc);
        const int a0 = static_cast<int>(midc);
        const int v1 = static_cast<int>(highc);
        const float inverse = 1.0f / squared;
        half(owner, 152) = static_cast<short>(a1);
        half(owner, 154) = static_cast<short>(a0);
        half(owner, 150) = static_cast<short>(v1);
        half(owner, 140) = static_cast<short>(v1);
        half(owner, 148) = static_cast<short>(v1);
        half(owner, 142) = static_cast<short>(v1);
        half(owner, 144) = static_cast<short>(a1);
        half(owner, 146) = static_cast<short>(a0);
        real(s, 76) = level;
        real(s, 84) = level;
        real(s, 88) = inverse;
        real(s, 80) = inverse;
    }
    const unsigned short before = static_cast<unsigned short>(half(s, 28));
    const short counted = static_cast<short>(before - 1);
    half(s, 28) = counted;
    if (counted < 0 && word(s, 32) == 0u) {
        word(s, 32) = 1u;
        int angle = 0;
        for (int i = 0; i < 60; ++i) {
            const float cx = icos__Fi(angle) * 100.0f;
            const float cy = isin__Fi(angle) * 100.0f;
            const float roll = static_cast<float>(eRand__5Spell(s)) * bits(0x37800000u) * 20.0f;
            Point3 velocity;
            velocity.x = cx;
            velocity.y = cy;
            velocity.z = roll;
            unsigned char* const owner = from_word(word(s, 12));
            Point3 point;
            point.x = real(owner, 16) + 0.0f;
            point.y = real(owner, 20);
            point.z = real(owner, 24) + 18.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(planarsteel_run_particle_8d85b0, &point, &velocity, 0.0f);
            angle = static_cast<short>(angle + 1092);
        }
        void* const sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_fire_hit.anm"));
        soundPlaySequence__FP15AnimationHeaderG6Point3f(sequence, *reinterpret_cast<Point3*>(from_word(word(s, 12)) + 16), 1.0f);
    }
    for (int k = 0; k < 2; ++k) {
        short* const angle_cell = reinterpret_cast<short*>(s + 24) + k;
        const short angle = static_cast<short>(static_cast<unsigned short>(*angle_cell) - 1500);
        *angle_cell = angle;
        Point3 offset;
        offset.x = icos__Fi(angle) * 40.0f;
        offset.y = isin__Fi(*angle_cell) * 40.0f;
        offset.z = static_cast<float>(half(s, 28));
        unsigned char* const owner = from_word(word(s, 12));
        const float scaled = v * 40.0f;
        const float upper = scaled + 50.0f;
        Point3 emit;
        emit.x = offset.x + real(owner, 16);
        const float lowerf = scaled + 100.0f;
        const int c = static_cast<int>(upper);
        emit.y = offset.y + real(owner, 20);
        const int d = static_cast<int>(lowerf);
        emit.z = offset.z + real(owner, 24);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(emit, 0.0f, 4000.0f, c, c, d, c, haloTexture5, 0, 2);
        for (int i = 0; i < 6; ++i) {
            Point3 velocity;
            draw_direction(s, velocity);
            velocity.x = velocity.x * bits(0x3eccccccu);
            velocity.y = velocity.y * bits(0x3eccccccu);
            velocity.z = velocity.z * bits(0x3eccccccu);
            Point3 spread;
            draw_direction(s, spread);
            Point3 point;
            point.x = spread.x + spread.x;
            point.y = spread.y + spread.y;
            point.z = spread.z + spread.z;
            point.x = point.x + emit.x;
            point.y = point.y + emit.y;
            point.z = point.z + emit.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(planarsteel_run_particle_8d8510, &point, &velocity, 0.0f);
        }
        for (int i = 0; i < 4; ++i) {
            Point3 velocity;
            draw_direction(s, velocity);
            velocity.x = velocity.x * 6.0f;
            velocity.y = velocity.y * 6.0f;
            velocity.z = velocity.z * 6.0f;
            Point3 spread;
            draw_direction(s, spread);
            Point3 point;
            point.x = spread.x + spread.x;
            point.y = spread.y + spread.y;
            point.z = spread.z + spread.z;
            point.x = point.x + emit.x;
            point.y = point.y + emit.y;
            point.z = point.z + emit.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(planarsteel_run_particle_8d8560, &point, &velocity, 0.0f);
        }
    }
    for (int i = 9; i >= 0; --i) {
        const int first = eRand__5Spell(s);
        const int second = eRand__5Spell(s);
        const short angle = static_cast<short>(first);
        float radius = static_cast<float>(second);
        const float cosine = icos__Fi(angle);
        radius = radius * bits(0x37800000u);
        radius = radius * 40.0f;
        const float sine = isin__Fi(angle);
        Point3 point;
        point.x = cosine * radius;
        point.y = sine * radius;
        point.z = 200.0f;
        const unsigned rem = static_cast<unsigned>(eRand__5Spell(s)) % 200u;
        Point3 velocity;
        velocity.z = -80.0f;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        const float z = point.z + static_cast<float>(static_cast<int>(rem));
        point.x = point.x + position.x;
        const float z2 = z + -100.0f;
        point.y = point.y + position.y;
        point.z = z2;
        point.z = z2 + position.z;
        P_AddParticle__FP11ParticleDefP6Point3T1f(planarsteel_run_particle_8d8600, &point, &velocity, 0.0f);
    }
    for (int i = 0; i < 10; ++i) {
        const int first = eRand__5Spell(s);
        const int second = eRand__5Spell(s);
        const short angle = static_cast<short>(first);
        float radius = static_cast<float>(second);
        const float cosine = icos__Fi(angle);
        radius = radius * bits(0x37800000u);
        radius = radius * 40.0f;
        const float sine = isin__Fi(angle);
        Point3 point;
        point.x = cosine * radius;
        point.y = sine * radius;
        point.z = 0.0f;
        Point3 velocity;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = 0.0f;
        velocity.x = static_cast<float>(eRand__5Spell(s) & 1) + -1.0f;
        const int bit = eRand__5Spell(s) & 1;
        point.x = point.x + position.x;
        point.y = point.y + position.y;
        const float zz = point.z + position.z;
        velocity.z = 0.0f;
        point.z = zz;
        velocity.y = static_cast<float>(bit) + -1.0f;
        const float h = worldFindPointHeight__FP12_worldHeaderG6Point3(world, point);
        point.z = h + 5.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(planarsteel_run_particle_8d8510, &point, &velocity, 0.0f);
    }
}
}
