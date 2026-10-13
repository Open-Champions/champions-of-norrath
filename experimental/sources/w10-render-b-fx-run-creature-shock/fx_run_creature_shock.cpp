// Observed fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi
// at 0x001dbbb0, extent 1804. Arguments: model, animation, a Point3 (pointer to a caller copy,
// copied to a local), a halfword angle, the FxCreatureState, three floats (a, b, c) and a word.
// Offsets are from the state address. Three 8-byte records start at +8: halfwords at +8+8i and
// +10+8i, and a word at +12+8i (a handle returned by the lightning start call, 0 when empty).
//  1. Returns at once when gameCloseToCamera(&position) is zero, when model is null, or when
//     modelGetNmShadowPoints(model) (n) is zero. float a is limited to at most 1.0 (a NaN stays).
//  2. For each record i = 0, 1, 2: with a handle present the generator is advanced once
//     (eRandState = eRandState * 0x19660d + 0x3c6ef35f, value = state >> 16) and the record is
//     rebuilt when value < 20000, otherwise it is only moved. An empty record is always rebuilt.
//  Rebuild: handle = 0; two more advances give halfwords (value % n) at +8+8i and +10+8i.
//     modelGetShadowPos(model, anim, &position, angle, index, &p0, &extra, 0, b) is called with
//     the first halfword and then, for p1, with the second halfword reloaded from the state.
//     When float c is nonzero, (icos(angle) * c, isin(angle) * c, 0) is added to both points.
//     handle = P_StartLightning(&p0, &p1, 207, 4, K1, K2, word) with the stored constants K1, K2.
//  Move: a halfword greater than n is reset to 0 (checked as n < halfword, signed); the same
//     two points are made the same way from the halfwords, then
//     P_MoveLightning(handle, &p0, &p1, (int)(a * 94.0f), -1). Then, for p0 and for p1 in turn:
//     a point is drawn in the unit ball (three draws 2 * (value * 2^-16) - 1, repeated while
//     1.0 < (x*x + y*y) + z*z), the state is stored, velocity = point * K0 with K3 added to z,
//     and P_AddParticle(ballSparkParticle, &p, &velocity, 0.0f) is called. Then the generator
//     is advanced once more; when value % 10 is zero, velocity is scaled by 0.25 in place and
//     P_AddParticle(ballSmokeParticle, &p, &velocity, 0.0f) is called.
// Constants (from the executable's float pool): K0 = 0x41accccc, K1 = 0x40f8f5c2, K2 = 0x408f0a3d,
// K3 = 0x3fd10624 (a second pool entry with the same bits is used for p1).
// Inferred: a lightning bolt between two shadow points of a shocked creature with sparks and
// smoke. Unknown: the roles of a, b, c and the word; ballSparkParticle and ballSmokeParticle are
// file-local objects of the original, so they stay unresolved imports here.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern int eRandState;
extern unsigned char ballSparkParticle[];
extern unsigned char ballSmokeParticle[];
int gameCloseToCamera__FP6Point3(Point3* position);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3* position, short yaw, int index, Point3* out_point, float* out_extra,
    void* matrix33, float scale);
float icos__Fi(int angle);
float isin__Fi(int angle);
int P_StartLightning__FG6Point3T0iiffUi(Point3* from, Point3* to, int a, int b, float c, float d, unsigned flags);
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3* from, Point3* to, int a, int b);
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* definition, Point3* position, Point3* velocity, float arg);
}

namespace {
constexpr float K0 = __builtin_bit_cast(float, 0x41acccccu);
constexpr float K1 = __builtin_bit_cast(float, 0x40f8f5c2u);
constexpr float K2 = __builtin_bit_cast(float, 0x408f0a3du);
constexpr float K3 = __builtin_bit_cast(float, 0x3fd10624u);
constexpr float TWO_NEG_16 = __builtin_bit_cast(float, 0x37800000u);

inline unsigned advance(unsigned state) { return state * 0x19660du + 0x3c6ef35fu; }

inline void makePoints(void* model, void* anim, Point3* position, short angle, int a, int b, float scale,
                       float shift, Point3* p0, Point3* p1) {
    float extra[4];
    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
        model, anim, position, angle, a, p0, extra, 0, scale);
    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
        model, anim, position, angle, b, p1, extra, 0, scale);
    if (shift != 0.0f) {
        float dx = icos__Fi(angle) * shift;
        float dy = isin__Fi(angle) * shift;
        float dz = 0.0f;
        p0->x = p0->x + dx;
        p0->y = p0->y + dy;
        p0->z = p0->z + dz;
        p1->x = p1->x + dx;
        p1->y = p1->y + dy;
        p1->z = p1->z + dz;
    }
}

inline void sparksAndSmoke(Point3* p) {
    Point3 v;
    unsigned s = static_cast<unsigned>(eRandState);
    float x, y, z;
    do {
        s = advance(s);
        x = static_cast<float>(s >> 16) * TWO_NEG_16;
        s = advance(s);
        y = static_cast<float>(s >> 16) * TWO_NEG_16;
        s = advance(s);
        z = static_cast<float>(s >> 16) * TWO_NEG_16;
        x = x + x - 1.0f;
        y = y + y - 1.0f;
        z = z + z - 1.0f;
    } while (1.0f < (x * x + y * y) + z * z);
    v.z = z;
    v.y = y;
    v.x = x * K0;
    eRandState = static_cast<int>(s);
    v.z = v.z * K0;
    v.y = v.y * K0;
    v.z = v.z + K3;
    P_AddParticle__FP11ParticleDefP6Point3T1f(ballSparkParticle, p, &v, 0.0f);
    s = advance(static_cast<unsigned>(eRandState));
    eRandState = static_cast<int>(s);
    if ((s >> 16) % 10u == 0) {
        v.x = v.x * 0.25f;
        v.y = v.y * 0.25f;
        v.z = v.z * 0.25f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(ballSmokeParticle, p, &v, 0.0f);
    }
}
}  // namespace

extern "C" void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
    void* model, void* anim, Point3* positionArg, short angle, unsigned char* state, float a, float b, float c,
    unsigned flags) {
    Point3 position = *positionArg;
    if (!gameCloseToCamera__FP6Point3(&position)) return;
    if (!model) return;
    int n = modelGetNmShadowPoints__FP12_modelHeader(model);
    if (!n) return;
    if (1.0f < a) a = 1.0f;
    for (int i = 0; i < 3; ++i) {
        unsigned char* rec = state + 8 * i;
        int& handle = *reinterpret_cast<int*>(rec + 12);
        short& h0 = *reinterpret_cast<short*>(rec + 8);
        short& h1 = *reinterpret_cast<short*>(rec + 10);
        bool rebuild = true;
        if (handle) {
            unsigned s = advance(static_cast<unsigned>(eRandState));
            eRandState = static_cast<int>(s);
            rebuild = (s >> 16) < 20000u;
        }
        if (rebuild) {
            handle = 0;
            unsigned s = advance(static_cast<unsigned>(eRandState));
            int ia = static_cast<int>(s >> 16) % n;
            eRandState = static_cast<int>(s);
            s = advance(s);
            int ib = static_cast<int>(s >> 16) % n;
            h0 = static_cast<short>(ia);
            eRandState = static_cast<int>(s);
            h1 = static_cast<short>(ib);
            Point3 p0, p1;
            float extra[4];
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, anim, &position, angle, ia, &p0, extra, 0, b);
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, anim, &position, angle, h1, &p1, extra, 0, b);
            if (c != 0.0f) {
                float dx = icos__Fi(angle) * c;
                float dy = isin__Fi(angle) * c;
                float dz = 0.0f;
                p0.x = p0.x + dx;
                p0.y = p0.y + dy;
                p0.z = p0.z + dz;
                p1.x = p1.x + dx;
                p1.y = p1.y + dy;
                p1.z = p1.z + dz;
            }
            handle = P_StartLightning__FG6Point3T0iiffUi(&p0, &p1, 207, 4, K1, K2, flags);
        } else {
            if (n < h0) h0 = 0;
            if (n < h1) h1 = 0;
            Point3 p0, p1;
            makePoints(model, anim, &position, angle, h0, h1, b, c, &p0, &p1);
            P_MoveLightning__FiG6Point3T1ii(handle, &p0, &p1, static_cast<int>(a * 94.0f), -1);
            sparksAndSmoke(&p0);
            sparksAndSmoke(&p1);
        }
    }
}
