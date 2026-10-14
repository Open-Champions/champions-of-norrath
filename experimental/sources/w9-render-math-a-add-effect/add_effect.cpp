// Observed addEffect__FiG6Point3 at 0x001d5630, 10368 bytes. a0 = effect type, a1 = address of a by-value
// Point3 (copied to the stack at entry). The result is void.
//  1. Unless the type is 5, gameCloseToCamera(&point) must return non-zero or the function returns.
//  2. Seventeen function-local ParticleDef statics (80 bytes each, 0x007d57a0 upward, named like
//     hitParticleEcto.1113) are constructed in order the first time they are reached; each has a guard word. The
//     constructor is __11ParticleDefiiiiiiiifffiiiPFPv_v(this, seven register ints, a stack int, three floats,
//     three stack ints, a null function pointer). The argument sets are the observed words in the code below.
//  3. A type above 18 (unsigned) returns. Otherwise a jump table selects the case. All random numbers come from
//     eRandState (0x00483e58) as state*0x19660d + 0x3c6ef35f, taking the upper 16 bits; the state is stored to
//     memory exactly where the original stores it, because P_AddParticle reads and advances it. A "unit sample" is
//     three draws mapped to (2*r*2^-16 - 1) each, repeated while the squared length is above 1.0.
//     type 0, 1  g_BloodLevel (0x00484498) zero returns; otherwise as type 9.
//     type 9     a point is drawn in a 12-unit cube around the argument (-6 + r % 12), a unit sample is normalized and
//                scaled to (x*34, y*34, z*34 + 4), "dism_blood_goosh%d.anm" (%d = (r & 3) + 1) is looked up in
//                "fx.lmp" and played at the argument, then 300 particles of hitParticleBlood (type 0),
//                hitParticleBigBlood (type 1) or the first static (otherwise) leave the drawn point. Each velocity
//                is a unit sample times 7.86 plus the stored direction times a fresh draw (2^-16 scaled). A
//                second unit sample per particle is drawn and discarded (it advances the generator only).
//     type 2     150 fxDust particles from a point in a 24 by 24 by 36 box; velocity 10.45 * a unit sample, and an
//                offset of (r % 12 - 6) per axis.
//     type 3, 6  150 particles of the third static from a 12-cube raised by 18; velocity 10.16 (type 3) or 16.0
//                (type 6) times a unit sample, all at the drawn point.
//     type 4     150 particles of the fourth static from the same raised cube; velocity 16.0 times a unit sample and
//                an offset of (r % 24 - 12) per axis.
//     type 5     syncRandState (0x00483e5c) draws 3 or 4; each blockAlloc(80) result that is non-null is
//                constructed with __4FireG6Point3 at a point from a 12 by 12 by 18 box.
//     type 7     three particles of the fifth static: a position unit sample scaled to 15.75 (z also * 0.75 and
//                + 0.07) added to the argument, and a velocity unit sample scaled to 4.75 (z also * 0.77 + 6.05).
//     type 8     300 particles of the sixth static from a 24 by 24 by 36 box; velocity 10.45 * a unit sample and an
//                offset of (r % 24 - 12) per axis.
//     type 10    25 particles of the seventh static around a 10-cube (velocity scale 5.45, offset r % 24 - 12),
//                then 80 of the eighth static from a raised 12-cube (velocity scale 20.0).
//     type 11, 12  15 of the ninth static (scale 5.45) then 20 of the tenth (scale 40.0) from the same two boxes;
//                type 11 adds five sparks of the eleventh static: a unit sample times 5 added to the second base, and a
//                velocity unit sample times 21.6 with z + 1.633.
//     type 13-16, 18  one particle (magic blue, green, red, stun, confuse) at the argument plus
//                (icos(a) * r * 15, isin(a) * r * 15, (r2 - 0.5) * 15), where a is the signed upper half of one draw,
//                r the next draw times 2^-16 and r2 the next; zero velocity.
//     type 17    one bubble particle at the argument with zero velocity.
// Every P_AddParticle call passes 0.0 as its single. The case split between types 10, 11 and 12 and between
// 3 and 6 is merged here where the observed code runs the same steps; the compared behaviour is identical.
// Unknown: what the effect numbers mean (names such as blood or onyx come from the static names only), and the
// meaning of the constructor arguments. Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern int eRandState;
extern int syncRandState;
extern int g_BloodLevel;
extern unsigned char hitParticleBlood[];
extern unsigned char hitParticleBigBlood[];
extern unsigned char fxDust[];
int gameCloseToCamera__FP6Point3(const Point3* point);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x,
                                         float y, float z, int i, int j, int k, void (*fn)(void*));
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, const Point3* position, const Point3* velocity,
                                               float value);
int sprintf(char* buffer, const char* format, ...);
void* lumpFindResource__FPcT0(const char* name, const char* group);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, const Point3* position, float volume);
void* blockAlloc__Fi(int bytes);
void __4FireG6Point3(void* self, const Point3* position);
float icos__Fi(int angle);
float isin__Fi(int angle);
}

namespace {
inline void particle_def_construct(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x,
                                   float y, float z, int i, int j, int k, void (*fn)(void*)) {
    __11ParticleDefiiiiiiiifffiiiPFPv_v(self, a, b, c, d, e, f, g, h, x, y, z, i, j, k, fn);
}
inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }
inline unsigned step(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }
inline float unit_value(unsigned state) { return static_cast<float>(static_cast<int>(state >> 16)); }
inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
// Three draws mapped to (-1, 1) each, repeated while the squared length exceeds 1.0.
inline void unit_sphere_sample(unsigned& state, Point3& out) {
    const float scale = bits(0x37800000u);
    const float one = bits(0x3f800000u);
    float x, y, z;
    do {
        state = step(state);
        float a = unit_value(state);
        state = step(state);
        float b = unit_value(state);
        state = step(state);
        float c = unit_value(state);
        a = a * scale;
        b = b * scale;
        a = a + a;
        b = b + b;
        c = c * scale;
        x = a - one;
        y = b - one;
        c = c + c;
        z = c - one;
        out.z = z;
    } while (one < (x * x + y * y) + z * z);
    out.x = x;
    out.y = y;
}
}
namespace {
// Function-local statics of the original (named like hitParticleEcto.1113 in the symbol table). Each is an
// 80-byte ParticleDef that is constructed the first time the function reaches it, guarded by a word.
alignas(8) unsigned char hit_particle_ecto[80];  // observed at 0x007d57a0
int hit_particle_ecto_ready;  // observed guard word at gp-10564
alignas(8) unsigned char hit_particle_dust[80];  // observed at 0x007d57f0
int hit_particle_dust_ready;  // observed guard word at gp-10560
alignas(8) unsigned char hit_particle_slime[80];  // observed at 0x007d5840
int hit_particle_slime_ready;  // observed guard word at gp-10556
alignas(8) unsigned char hit_particle_jelly[80];  // observed at 0x007d5890
int hit_particle_jelly_ready;  // observed guard word at gp-10552
alignas(8) unsigned char dirt_particle1[80];  // observed at 0x007d58e0
int dirt_particle1_ready;  // observed guard word at gp-10548
alignas(8) unsigned char hit_particle_snow_hit[80];  // observed at 0x007d5930
int hit_particle_snow_hit_ready;  // observed guard word at gp-10544
alignas(8) unsigned char hit_particle_ice_hit[80];  // observed at 0x007d5980
int hit_particle_ice_hit_ready;  // observed guard word at gp-10540
alignas(8) unsigned char hit_particle_ice1[80];  // observed at 0x007d59d0
int hit_particle_ice1_ready;  // observed guard word at gp-10536
alignas(8) unsigned char onyx_hit_particle1[80];  // observed at 0x007d5a20
int onyx_hit_particle1_ready;  // observed guard word at gp-10532
alignas(8) unsigned char onyx_hit_particle2[80];  // observed at 0x007d5a70
int onyx_hit_particle2_ready;  // observed guard word at gp-10528
alignas(8) unsigned char onyx_spark_particle[80];  // observed at 0x007d5ac0
int onyx_spark_particle_ready;  // observed guard word at gp-10524
alignas(8) unsigned char magic_blue_particle[80];  // observed at 0x007d5b10
int magic_blue_particle_ready;  // observed guard word at gp-10520
alignas(8) unsigned char magic_green_particle[80];  // observed at 0x007d5b60
int magic_green_particle_ready;  // observed guard word at gp-10516
alignas(8) unsigned char magic_red_particle[80];  // observed at 0x007d5bb0
int magic_red_particle_ready;  // observed guard word at gp-10512
alignas(8) unsigned char stun_particle[80];  // observed at 0x007d5c00
int stun_particle_ready;  // observed guard word at gp-10508
alignas(8) unsigned char confuse_particle[80];  // observed at 0x007d5c50
int confuse_particle_ready;  // observed guard word at gp-10504
alignas(8) unsigned char bubble_particle[80];  // observed at 0x007d5ca0
int bubble_particle_ready;  // observed guard word at gp-10500

__attribute__((always_inline)) inline void construct_statics() {
    if (hit_particle_ecto_ready == 0) {
        particle_def_construct(hit_particle_ecto, 100, 90, 15, 204, 80, 70, 5, 0, bits(0x40900000u), bits(0x3f999999u), bits(0xc0299999u), 1, 0, 6, 0);
        hit_particle_ecto_ready = 1;
    }
    if (hit_particle_dust_ready == 0) {
        particle_def_construct(hit_particle_dust, 92, 89, 64, 34, 92, 85, 64, 0, bits(0x41a570a3u), bits(0x4118ccccu), bits(0xc0a0f5c2u), 2, 0, 2, 0);
        hit_particle_dust_ready = 1;
    }
    if (hit_particle_slime_ready == 0) {
        particle_def_construct(hit_particle_slime, 39, 108, 40, 53, 40, 87, 19, 0, bits(0x407eb851u), bits(0x4183d70au), bits(0xc0afae14u), 3, 0, 2, 0);
        hit_particle_slime_ready = 1;
    }
    if (hit_particle_jelly_ready == 0) {
        particle_def_construct(hit_particle_jelly, 39, 40, 108, 53, 160, 160, 220, 0, bits(0x407eb851u), bits(0x4183d70au), bits(0xc0afae14u), 3, 0, 2, 0);
        hit_particle_jelly_ready = 1;
    }
    if (dirt_particle1_ready == 0) {
        particle_def_construct(dirt_particle1, 22, 21, 6, 235, 20, 19, 6, 0, bits(0x422bb6bau), bits(0x426154a5u), bits(0xbf9accf6u), 2, 0, 4, 0);
        dirt_particle1_ready = 1;
    }
    if (hit_particle_snow_hit_ready == 0) {
        particle_def_construct(hit_particle_snow_hit, 108, 115, 130, 34, 108, 115, 130, 0, bits(0x41a570a3u), bits(0x4118ccccu), bits(0xc0a0f5c2u), 2, 0, 2, 0);
        hit_particle_snow_hit_ready = 1;
    }
    if (hit_particle_ice_hit_ready == 0) {
        particle_def_construct(hit_particle_ice_hit, 100, 210, 238, 90, 4, 63, 105, 0, bits(0x41a570a3u), bits(0x41f46666u), bits(0x4001eb85u), 2, 0, 2, 0);
        hit_particle_ice_hit_ready = 1;
    }
    if (hit_particle_ice1_ready == 0) {
        particle_def_construct(hit_particle_ice1, 100, 210, 238, 90, 4, 63, 105, 0, bits(0x40b3d70au), bits(0x40b80000u), bits(0xc0a00000u), 3, 1, 4, 0);
        hit_particle_ice1_ready = 1;
    }
    if (onyx_hit_particle1_ready == 0) {
        particle_def_construct(onyx_hit_particle1, 23, 23, 23, 90, 4, 13, 8, 0, bits(0x41a570a3u), bits(0x41f46666u), bits(0x4001eb85u), 2, 0, 2, 0);
        onyx_hit_particle1_ready = 1;
    }
    if (onyx_hit_particle2_ready == 0) {
        particle_def_construct(onyx_hit_particle2, 10, 10, 10, 90, 10, 10, 10, 0, bits(0x40b3d70au), bits(0x40b80000u), bits(0xc0a00000u), 1, 0, 4, 0);
        onyx_hit_particle2_ready = 1;
    }
    if (onyx_spark_particle_ready == 0) {
        particle_def_construct(onyx_spark_particle, 255, 255, 255, 255, 0, 0, 0, 0, bits(0x41d00000u), bits(0x41200000u), bits(0xc1700000u), 2, 1, 0, 0);
        onyx_spark_particle_ready = 1;
    }
    if (magic_blue_particle_ready == 0) {
        particle_def_construct(magic_blue_particle, 80, 80, 250, 155, 50, 50, 250, 0, bits(0x3f800000u), bits(0x41f00000u), bits(0x00000000u), 2, 0, 9, 0);
        magic_blue_particle_ready = 1;
    }
    if (magic_green_particle_ready == 0) {
        particle_def_construct(magic_green_particle, 50, 250, 50, 105, 50, 250, 50, 0, bits(0x3f800000u), bits(0x41f00000u), bits(0x00000000u), 2, 0, 9, 0);
        magic_green_particle_ready = 1;
    }
    if (magic_red_particle_ready == 0) {
        particle_def_construct(magic_red_particle, 250, 20, 40, 185, 250, 20, 40, 0, bits(0x3f800000u), bits(0x41a00000u), bits(0x00000000u), 2, 0, 9, 0);
        magic_red_particle_ready = 1;
    }
    if (stun_particle_ready == 0) {
        particle_def_construct(stun_particle, 255, 255, 255, 255, 0, 0, 0, 0, bits(0x3f800000u), bits(0x41a00000u), bits(0x00000000u), 1, 0, 9, 0);
        stun_particle_ready = 1;
    }
    if (confuse_particle_ready == 0) {
        particle_def_construct(confuse_particle, 128, 128, 0, 255, 0, 0, 0, 0, bits(0x3f800000u), bits(0x42200000u), bits(0x40000000u), 2, 0, 9, 0);
        confuse_particle_ready = 1;
    }
    if (bubble_particle_ready == 0) {
        particle_def_construct(bubble_particle, 180, 180, 180, 190, 120, 120, 120, 15, bits(0x3f000000u), bits(0x41c00000u), bits(0x40a00000u), 2, 1, 16, 0);
        bubble_particle_ready = 1;
    }
}
}

extern "C" void addEffect__FiG6Point3(int type, const Point3* position) {
    const Point3 pos = *position;
    if (type != 5) {
        if (gameCloseToCamera__FP6Point3(&pos) == 0) return;
    }
    construct_statics();
    if (static_cast<unsigned>(type) > 18u) return;
    switch (type) {
    case 0:
    case 1:
        if (g_BloodLevel == 0) return;
        // fall through
    case 9: {
        unsigned s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r2 = s >> 16;
        s = step(s);
        const unsigned r3 = s >> 16;
        Point3 base;
        base.x = (pos.x - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r1 % 12u));
        base.y = (pos.y - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r2 % 12u));
        base.z = (pos.z - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r3 % 12u));
        Point3 direction;
        unit_sphere_sample(s, direction);
        s = step(s);
        eRandState = static_cast<int>(s);
        const int variant = static_cast<int>((s >> 16) & 3u) + 1;
        const float inverse = reciprocal_sqrt((direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z);
        direction.x = direction.x * inverse;
        direction.y = direction.y * inverse;
        direction.z = direction.z * inverse;
        const float scale_out = bits(0x42080000u);
        direction.x = direction.x * scale_out;
        direction.z = direction.z * scale_out;
        direction.y = direction.y * scale_out;
        direction.z = direction.z + bits(0x40800000u);
        char name[32];
        sprintf(name, "dism_blood_goosh%d.anm", variant);
        void* header = lumpFindResource__FPcT0("fx.lmp", name);
        soundPlaySequence__FP15AnimationHeaderG6Point3f(header, &pos, 1.0f);
        void* const definition = type == 0 ? static_cast<void*>(hitParticleBlood)
                                           : (type == 1 ? static_cast<void*>(hitParticleBigBlood)
                                                        : static_cast<void*>(hit_particle_ecto));
        for (int n = 0; n < 300; ++n) {
            s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            s = step(s);
            const float along = unit_value(s) * bits(0x37800000u);
            velocity.x = velocity.x * bits(0x40fb851eu);
            velocity.y = velocity.y * bits(0x40fb851eu);
            velocity.z = velocity.z * bits(0x40fb851eu);
            Point3 along_direction;
            along_direction.x = direction.x * along;
            along_direction.y = direction.y * along;
            along_direction.z = direction.z * along;
            velocity.x = velocity.x + along_direction.x;
            velocity.y = velocity.y + along_direction.y;
            velocity.z = velocity.z + along_direction.z;
            Point3 discarded;
            unit_sphere_sample(s, discarded);
            eRandState = static_cast<int>(s);
            P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &base, &velocity, 0.0f);
        }
        return;
    }

    case 2: {
        unsigned s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r2 = s >> 16;
        s = step(s);
        const unsigned r3 = s >> 16;
        Point3 base;
        base.x = (pos.x - bits(0x41400000u)) + static_cast<float>(static_cast<int>(r1 % 24u));
        base.y = (pos.y - bits(0x41400000u)) + static_cast<float>(static_cast<int>(r2 % 24u));
        base.z = (pos.z - bits(0x41900000u)) + static_cast<float>(static_cast<int>(r3 % 36u));
        for (int n = 0; n < 150; ++n) {
            if (n != 0) s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            s = step(s);
            const unsigned ra = s >> 16;
            s = step(s);
            const unsigned rb = s >> 16;
            s = step(s);
            const unsigned rc = s >> 16;
            eRandState = static_cast<int>(s);
            velocity.y = velocity.y * bits(0x41273333u);
            velocity.z = velocity.z * bits(0x41273333u);
            velocity.x = velocity.x * bits(0x41273333u);
            Point3 where;
            where.x = static_cast<float>(static_cast<int>(ra % 12u) - 6) + base.x;
            where.y = static_cast<float>(static_cast<int>(rb % 12u) - 6) + base.y;
            where.z = static_cast<float>(static_cast<int>(rc % 12u) - 6) + base.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(fxDust, &where, &velocity, 0.0f);
        }
        return;
    }
    case 3:
    case 6: {
        unsigned s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r2 = s >> 16;
        s = step(s);
        const unsigned r3 = s >> 16;
        Point3 base;
        base.x = (pos.x - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r1 % 12u));
        base.y = (pos.y - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r2 % 12u));
        base.z = ((pos.z - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r3 % 12u))) + bits(0x41900000u);
        for (int n = 0; n < 150; ++n) {
            if (n != 0) s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            eRandState = static_cast<int>(s);
            const float scale = type == 6 ? bits(0x41800000u) : bits(0x41228f5cu);
            velocity.x = velocity.x * scale;
            velocity.y = velocity.y * scale;
            velocity.z = velocity.z * scale;
            Point3 where = base;
            P_AddParticle__FP11ParticleDefP6Point3T1f(hit_particle_slime, &where, &velocity, 0.0f);
        }
        return;
    }
    case 4: {
        unsigned s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r2 = s >> 16;
        s = step(s);
        const unsigned r3 = s >> 16;
        Point3 base;
        base.x = (pos.x - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r1 % 12u));
        base.y = (pos.y - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r2 % 12u));
        base.z = ((pos.z - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(r3 % 12u))) + bits(0x41900000u);
        for (int n = 0; n < 150; ++n) {
            if (n != 0) s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            s = step(s);
            const unsigned ra = s >> 16;
            s = step(s);
            const unsigned rb = s >> 16;
            s = step(s);
            const unsigned rc = s >> 16;
            eRandState = static_cast<int>(s);
            velocity.y = velocity.y * bits(0x41800000u);
            velocity.z = velocity.z * bits(0x41800000u);
            velocity.x = velocity.x * bits(0x41800000u);
            Point3 where;
            where.x = static_cast<float>(static_cast<int>(ra % 24u) - 12) + base.x;
            where.y = static_cast<float>(static_cast<int>(rb % 24u) - 12) + base.y;
            where.z = static_cast<float>(static_cast<int>(rc % 24u) - 12) + base.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(hit_particle_jelly, &where, &velocity, 0.0f);
        }
        return;
    }
    case 5: {
        unsigned s = static_cast<unsigned>(syncRandState);
        s = step(s);
        const int count = static_cast<int>((s >> 16) & 1u) + 3;
        syncRandState = static_cast<int>(s);
        for (int n = 0; n < count; ++n) {
            if (n != 0) s = static_cast<unsigned>(syncRandState);
            s = step(s);
            const unsigned ra = s >> 16;
            syncRandState = static_cast<int>(s);
            s = step(s);
            const unsigned rb = s >> 16;
            s = step(s);
            const unsigned rc = s >> 16;
            syncRandState = static_cast<int>(s);
            Point3 where;
            where.x = (pos.x - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(ra % 12u));
            where.y = (pos.y - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(rb % 12u));
            where.z = pos.z + static_cast<float>(static_cast<int>(rc % 18u));
            void* const memory = blockAlloc__Fi(80);
            if (memory != 0) __4FireG6Point3(memory, &where);
        }
        return;
    }
    case 7: {
        for (int n = 0; n < 3; ++n) {
            unsigned s = static_cast<unsigned>(eRandState);
            Point3 first;
            unit_sphere_sample(s, first);
            first.x = first.x * bits(0x417c0000u);
            first.y = first.y * bits(0x417c0000u);
            first.z = (first.z * bits(0x417c0000u)) * bits(0x3f400000u);
            eRandState = static_cast<int>(s);
            first.z = first.z + bits(0x3d8f5c28u);
            Point3 second;
            unit_sphere_sample(s, second);
            second.x = second.x * bits(0x40980000u);
            second.y = second.y * bits(0x40980000u);
            second.z = (second.z * bits(0x40980000u)) * bits(0x3f451eb8u) + bits(0x40c19999u);
            eRandState = static_cast<int>(s);
            first.x = first.x + pos.x;
            first.y = first.y + pos.y;
            first.z = first.z + pos.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(dirt_particle1, &first, &second, 0.0f);
        }
        return;
    }
    case 8: {
        unsigned s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r2 = s >> 16;
        s = step(s);
        const unsigned r3 = s >> 16;
        Point3 base;
        base.x = (pos.x - bits(0x41400000u)) + static_cast<float>(static_cast<int>(r1 % 24u));
        base.y = (pos.y - bits(0x41400000u)) + static_cast<float>(static_cast<int>(r2 % 24u));
        base.z = (pos.z - bits(0x41900000u)) + static_cast<float>(static_cast<int>(r3 % 36u));
        for (int n = 0; n < 300; ++n) {
            if (n != 0) s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            s = step(s);
            const unsigned ra = s >> 16;
            s = step(s);
            const unsigned rb = s >> 16;
            s = step(s);
            const unsigned rc = s >> 16;
            eRandState = static_cast<int>(s);
            velocity.y = velocity.y * bits(0x41273333u);
            velocity.z = velocity.z * bits(0x41273333u);
            velocity.x = velocity.x * bits(0x41273333u);
            Point3 where;
            where.x = static_cast<float>(static_cast<int>(ra % 24u) - 12) + base.x;
            where.y = static_cast<float>(static_cast<int>(rb % 24u) - 12) + base.y;
            where.z = static_cast<float>(static_cast<int>(rc % 24u) - 12) + base.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(hit_particle_snow_hit, &where, &velocity, 0.0f);
        }
        return;
    }
    case 10:
    case 11:
    case 12: {
        // A first burst around a corner-shifted base, then a second burst around a raised base; types 11
        // and 12 use the onyx definitions and type 11 adds five sparks.
        unsigned s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned r2 = s >> 16;
        s = step(s);
        const unsigned r3 = s >> 16;
        Point3 base;
        base.x = (pos.x - bits(0x40a00000u)) + static_cast<float>(static_cast<int>(r1 % 10u));
        base.y = (pos.y - bits(0x40a00000u)) + static_cast<float>(static_cast<int>(r2 % 10u));
        base.z = (pos.z - bits(0x40a00000u)) + static_cast<float>(static_cast<int>(r3 % 10u));
        const bool onyx = type != 10;
        const int first_count = onyx ? 15 : 25;
        void* const first_definition = onyx ? static_cast<void*>(onyx_hit_particle1) : static_cast<void*>(hit_particle_ice_hit);
        const float first_scale = bits(0x40ae6666u);
        for (int n = 0; n < first_count; ++n) {
            if (n != 0) s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            s = step(s);
            const unsigned ra = s >> 16;
            s = step(s);
            const unsigned rb = s >> 16;
            s = step(s);
            const unsigned rc = s >> 16;
            eRandState = static_cast<int>(s);
            velocity.y = velocity.y * first_scale;
            velocity.z = velocity.z * first_scale;
            velocity.x = velocity.x * first_scale;
            Point3 where;
            where.x = static_cast<float>(static_cast<int>(ra % 24u) - 12) + base.x;
            where.y = static_cast<float>(static_cast<int>(rb % 24u) - 12) + base.y;
            where.z = static_cast<float>(static_cast<int>(rc % 24u) - 12) + base.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(first_definition, &where, &velocity, 0.0f);
        }
        s = static_cast<unsigned>(eRandState);
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned q1 = s >> 16;
        s = step(s);
        eRandState = static_cast<int>(s);
        const unsigned q2 = s >> 16;
        s = step(s);
        const unsigned q3 = s >> 16;
        base.x = (pos.x - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(q1 % 12u));
        base.y = (pos.y - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(q2 % 12u));
        base.z = ((pos.z - bits(0x40c00000u)) + static_cast<float>(static_cast<int>(q3 % 12u))) + bits(0x41900000u);
        const int second_count = onyx ? 20 : 80;
        void* const second_definition = onyx ? static_cast<void*>(onyx_hit_particle2) : static_cast<void*>(hit_particle_ice1);
        for (int n = 0; n < second_count; ++n) {
            if (n != 0) s = static_cast<unsigned>(eRandState);
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            const float scale = bits(onyx ? 0x42200000u : 0x41a00000u);
            s = step(s);
            const unsigned ra = s >> 16;
            s = step(s);
            const unsigned rb = s >> 16;
            s = step(s);
            const unsigned rc = s >> 16;
            eRandState = static_cast<int>(s);
            velocity.x = velocity.x * scale;
            velocity.y = velocity.y * scale;
            velocity.z = velocity.z * scale;
            Point3 where;
            where.x = static_cast<float>(static_cast<int>(ra % 24u) - 12) + base.x;
            where.y = static_cast<float>(static_cast<int>(rb % 24u) - 12) + base.y;
            where.z = static_cast<float>(static_cast<int>(rc % 24u) - 12) + base.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(second_definition, &where, &velocity, 0.0f);
        }
        if (type != 11) return;
        for (int n = 0; n < 5; ++n) {
            s = static_cast<unsigned>(eRandState);
            Point3 offset;
            unit_sphere_sample(s, offset);
            eRandState = static_cast<int>(s);
            Point3 where = offset;
            where.x = where.x * bits(0x40a00000u);
            where.y = where.y * bits(0x40a00000u);
            where.z = where.z * bits(0x40a00000u);
            where.y = where.y + base.y;
            where.x = where.x + base.x;
            where.z = where.z + base.z;
            Point3 velocity;
            unit_sphere_sample(s, velocity);
            eRandState = static_cast<int>(s);
            velocity.x = velocity.x * bits(0x41acccccu);
            velocity.y = velocity.y * bits(0x41acccccu);
            velocity.z = velocity.z * bits(0x41acccccu) + bits(0x3fd10624u);
            P_AddParticle__FP11ParticleDefP6Point3T1f(onyx_spark_particle, &where, &velocity, 0.0f);
        }
        return;
    }
    case 13:
    case 14:
    case 15:
    case 16:
    case 18: {
        // One particle at a position scattered around the point, with zero velocity.
        unsigned s = step(static_cast<unsigned>(eRandState));
        const int angle = static_cast<int>(s) >> 16;
        s = step(s);
        const float distance = unit_value(s);
        Point3 where = pos;
        Point3 velocity;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = 0.0f;
        eRandState = static_cast<int>(s);
        const float cosine = icos__Fi(angle);
        const float radius = distance * bits(0x37800000u);
        where.x = where.x + (cosine * radius) * bits(0x41700000u);
        const float sine = isin__Fi(angle);
        const float sine_scaled = (sine * radius) * bits(0x41700000u);
        s = step(static_cast<unsigned>(eRandState));
        eRandState = static_cast<int>(s);
        where.y = where.y + sine_scaled;
        where.z = where.z + ((unit_value(s) * bits(0x37800000u)) + bits(0xbf000000u)) * bits(0x41700000u);
        void* definition;
        switch (type) {
        case 13: definition = magic_blue_particle; break;
        case 14: definition = magic_green_particle; break;
        case 15: definition = magic_red_particle; break;
        case 16: definition = stun_particle; break;
        default: definition = confuse_particle; break;
        }
        P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &where, &velocity, 0.0f);
        return;
    }
    case 17: {
        Point3 velocity;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = 0.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(bubble_particle, &pos, &velocity, 0.0f);
        return;
    }
    default:
        return;
    }
}
