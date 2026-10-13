// Observed fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff at
// 0x001dace0, 1524 bytes. Arguments: model, animation state, a Point3 (passed by address and copied
// to a local), a short angle, an FxCreatureState pointer, then four floats f12..f15. Only f13 (a
// scale, here "scale") and f14 (a push distance, "push") are read; the pointer, f12 and f15 are
// not. Everything below follows the image's order.
//  - gameCloseToCamera__FP6Point3 is called on the Point3 copy; a zero result, a null model, or a
//    zero count from modelGetNmShadowPoints__FP12_modelHeader ends the call.
//  - Two passes run, pass 0 with the definition cFireParticle and pass 1 with cSmokeParticle. The
//    pass count is scale times a constant (16.5935879 for pass 0, 3.76093793 for pass 1) rounded
//    half away from zero by adding or subtracting 0.5 and truncating (a non-negative product, or one
//    comparing 0.0 <= product, adds). A count that is not positive skips the pass.
//  - Each repetition steps eRandState (state * 0x19660d + 0x3c6ef35f) and uses (state >> 16)
//    modulo the shadow point count as the shadow index; modelGetShadowPos__FP12_modelHeaderP14Anim
//    ationStateG6Point3siP6Point3PfP8Matrix33f is called with the model, state, the Point3 copy,
//    the angle, the index, an out Point3, an out float height, a null matrix and scale. The out
//    position moves by icos(angle) * push in x and isin(angle) * push in y, and 0.0f is added to z.
//  - With e = height * height, while e > 0: range = height * 1.0059698820114136 * 9.99999975e-05; three
//    generator steps give offsets (r * 2^-16 - 0.5) * range added to x, y, z of the position; then
//    generator steps are repeated, three at a time, forming x, y, z as 2 * (r * 2^-16) - 1, until
//    x*x + y*y + z*z (added in that order) is not more than 1.0; one more step gives a life value
//    (r * 2^-16) * a pass constant (0.29999998211860657 both times); the unit vector is scaled by 10.0
//    (pass 0) or 9.0 (pass 1) into a velocity; eRandState is stored and
//    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position, velocity, life) is called;
//    finally e -= 114511400.0.
// Nothing is returned.
// Inferred: fire and smoke particles are emitted around a burning creature's shadow points, with
// more emitted for a larger scale; the "height" is the shadow-position routine's extra float
// output. Unknown: what FxCreatureState, the angle and the unread floats mean, and what the
// 114511400.0 limit is for (it makes the inner loop run once unless height is above about 10700).
struct Point3 { float x, y, z; };
extern "C" {
extern int eRandState;
extern unsigned char cFireParticle[];
extern unsigned char cSmokeParticle[];
int gameCloseToCamera__FP6Point3(Point3* position);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, Point3 point, short value, int index, Point3* position, float* height,
    void* matrix, float scale);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity,
                                              float life);

static unsigned nextRandom() {
    eRandState = static_cast<int>(static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu);
    return static_cast<unsigned>(eRandState) >> 16;
}

void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
    void* model, void* state, Point3 point, short angle, void* creature, float unused0, float scale,
    float push, float unused1) {
    (void)creature;
    (void)unused0;
    (void)unused1;
    Point3 origin = point;
    if (!gameCloseToCamera__FP6Point3(&origin)) return;
    if (model == nullptr) return;
    const int shadowPoints = modelGetNmShadowPoints__FP12_modelHeader(model);
    if (shadowPoints == 0) return;
    for (int pass = 0; pass < 2; ++pass) {
        const float want = scale * (pass == 0 ? 16.5935879f : 3.76093793f);
        const int count = static_cast<int>(0.0f <= want ? want + 0.5f : want - 0.5f);
        for (int repetition = 0; repetition < count; ++repetition) {
            const int index = static_cast<int>(nextRandom()) % shadowPoints;
            Point3 position;
            float height;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, state, origin, angle, index, &position, &height, nullptr, scale);
            const float dx = icos__Fi(angle) * push;
            const float dy = isin__Fi(angle) * push;
            const float dz = 0.0f;
            float energy = height * height;
            position.y = position.y + dy;
            position.x = position.x + dx;
            position.z = position.z + dz;
            while (0.0f < energy) {
                const float range = height * 1.0059698820114136f * 9.99999975e-05f;
                const float sx = (static_cast<float>(nextRandom()) * (1.0f / 65536.0f) - 0.5f) * range;
                const float sy = (static_cast<float>(nextRandom()) * (1.0f / 65536.0f) - 0.5f) * range;
                const float sz = (static_cast<float>(nextRandom()) * (1.0f / 65536.0f) - 0.5f) * range;
                position.y = position.y + sy;
                position.x = position.x + sx;
                position.z = position.z + sz;
                Point3 direction;
                float length;
                do {
                    const float a = static_cast<float>(nextRandom()) * (1.0f / 65536.0f);
                    const float b = static_cast<float>(nextRandom()) * (1.0f / 65536.0f);
                    const float c = static_cast<float>(nextRandom()) * (1.0f / 65536.0f);
                    direction.x = (a + a) - 1.0f;
                    direction.y = (b + b) - 1.0f;
                    direction.z = (c + c) - 1.0f;
                    length = direction.x * direction.x + direction.y * direction.y;
                    length = length + direction.z * direction.z;
                } while (1.0f < length);
                const float speed = pass == 0 ? 10.0f : 9.0f;
                Point3 velocity = {direction.x * speed, direction.y * speed, direction.z * speed};
                const float life = static_cast<float>(nextRandom()) * (1.0f / 65536.0f) * 0.29999998211860657f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(pass == 0 ? cFireParticle : cSmokeParticle,
                                                          &position, &velocity, life);
                energy = energy - 114511400.0f;
            }
        }
    }
}
}
