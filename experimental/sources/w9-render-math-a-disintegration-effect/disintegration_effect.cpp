// Observed disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi
// at 0x001d9e98, 1008 bytes. Arguments: a0 = object, a1 = model header, a2 = animation state,
// a3 = address of a by-value Point3 (copied to the stack at entry), t0 = short angle (the original sign-extends the low halfword at entry, so the parameter is
// taken as an int here and narrowed explicitly), F12 =
// single (call it shadow_scale), F13 = single (call it spread), t1 = int flag.
//  1. A null model returns at once (before anything else is touched).
//  2. object+12 |= 2. lumpFindResource("fx.lmp", "spel_dsrptn_afteraffct.anm") gives a header
//     passed to soundPlaySequence(header, &position, 1.0f).
//  3. n = modelGetNmShadowPoints(model); n <= 0 returns. A running single "budget" starts at
//     0.0. For each index i in 0 .. n-1: modelGetShadowPos(model, anim, &position, angle, i,
//     &point, &size, 0, shadow_scale) fills a Point3 and a single; budget += size*size*size.
//  4. While 0 < budget: budget -= 8.09e10 (0x5196b020), then six pseudo-random draws
//     (state = state*0x19660d + 0x3c6ef35f, value = (state >> 16) as single times 2^-16) give
//     offsets and velocity: with S = (size * 9.99999975e-6) * 35.35 (0x3727c5ac, 0x420d6666),
//     position = point + ((r1-0.5)*S, (r2-0.5)*S, (r3-0.5)*S) and velocity =
//     ((r4-0.5)*3.14, (r5-0.5)*3.14, (r6-0.5)*3.14) (0x4048f5c2).
//  5. If spread != 0.0 the state is stored first, then position.x += icos(angle)*spread and
//     position.y += isin(angle)*spread, with the state read back afterwards. position.z
//     gains 0.0.
//  6. A seventh draw is made and stored to eRandState (0x00483e58); P_AddParticle(def,
//     &position, &velocity, value * 0.4) is called, where value is that draw times 2^-16, the
//     factor 0.4 is 0x3ecccccc, and def is blueDisintoParticle when the flag is non-zero,
//     else disintoParticle. The loop in step 4 repeats while 0 < budget.
//  7. After the last index the function returns.
// Unknown: what budget, size and flag mean (inferred only from the call names). The two
// particle definitions are file-local objects in the executable (0x007d6100, 0x007d60b0); they
// are used here as imports. Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};
struct GameObject;
struct ModelHeader;
struct AnimationState;
struct ParticleDef;

extern "C" {
extern int eRandState;
extern unsigned char blueDisintoParticle[];
extern unsigned char disintoParticle[];
void* lumpFindResource__FPcT0(const char* name, const char* group);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, const Point3* position, float volume);
int modelGetNmShadowPoints__FP12_modelHeader(ModelHeader* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    ModelHeader* model, AnimationState* anim, const Point3* position, int angle, int index, Point3* out_point,
    float* out_size, void* matrix, float shadow_scale);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float value);

void disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(
    GameObject* object, ModelHeader* model, AnimationState* anim, const Point3* position_argument, int angle_argument,
    float shadow_scale, float spread, int blue) {
    const Point3 position = *position_argument;
    const int angle = static_cast<short>(angle_argument);
    if (model == 0) return;
    *reinterpret_cast<unsigned*>(reinterpret_cast<unsigned char*>(object) + 12) |= 2u;
    void* header = lumpFindResource__FPcT0("fx.lmp", "spel_dsrptn_afteraffct.anm");
    soundPlaySequence__FP15AnimationHeaderG6Point3f(header, &position, 1.0f);
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
    if (count <= 0) return;
    float budget = 0.0f;
    const unsigned multiplier = 0x19660du;
    const unsigned increment = 0x3c6ef35fu;
    const float unit = __builtin_bit_cast(float, 0x37800000u);
    for (int index = 0; index < count; ++index) {
        Point3 point;
        float size;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, anim, &position, angle, index, &point, &size, 0, shadow_scale);
        budget += (size * size) * size;
        while (0.0f < budget) {
            unsigned state = static_cast<unsigned>(eRandState);
            const float scale_a = __builtin_bit_cast(float, 0x3727c5acu);
            const float scale_b = __builtin_bit_cast(float, 0x420d6666u);
            const float cost = __builtin_bit_cast(float, 0x5196b020u);
            const float velocity_scale = __builtin_bit_cast(float, 0x4048f5c2u);
            const float life_scale = __builtin_bit_cast(float, 0x3ecccccc);
            float scaled = (size * scale_a) * scale_b;
            budget = budget - cost;
            float r[6];
            for (int k = 0; k < 6; ++k) {
                state = state * multiplier + increment;
                r[k] = static_cast<float>(static_cast<int>(state >> 16)) * unit;
            }
            Point3 spawn;
            spawn.x = (r[0] - 0.5f) * scaled;
            spawn.y = (r[1] - 0.5f) * scaled;
            spawn.z = (r[2] - 0.5f) * scaled;
            Point3 velocity;
            velocity.x = (r[3] - 0.5f) * velocity_scale;
            velocity.y = (r[4] - 0.5f) * velocity_scale;
            velocity.z = (r[5] - 0.5f) * velocity_scale;
            spawn.x = spawn.x + point.x;
            spawn.y = spawn.y + point.y;
            spawn.z = spawn.z + point.z;
            if (spread != 0.0f) {
                eRandState = static_cast<int>(state);
                const float cosine = icos__Fi(angle) * spread;
                const float sine = isin__Fi(angle) * spread;
                state = static_cast<unsigned>(eRandState);
                spawn.x = spawn.x + cosine;
                spawn.y = spawn.y + sine;
                spawn.z = spawn.z + 0.0f;
            }
            state = state * multiplier + increment;
            eRandState = static_cast<int>(state);
            const float value = (static_cast<float>(static_cast<int>(state >> 16)) * unit) * life_scale;
            P_AddParticle__FP11ParticleDefP6Point3T1f(blue != 0 ? blueDisintoParticle : disintoParticle, &spawn,
                                                      &velocity, value);
        }
    }
}
}
