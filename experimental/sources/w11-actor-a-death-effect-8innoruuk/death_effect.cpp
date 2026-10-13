// Observed deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff at 0x001fc8c0, 824
// bytes. Arguments: the Innoruuk object (used as the eRand source), a GameObject pointer G, a model, an
// animation state, a Point3 by value, a short heading and two floats A and B. Nothing is returned. The
// image reads five float constants through the global pointer: K1 = 1.0e-5 (0x3727c5ac), K2 = 0x54da475a
// (about 7.5e12), K3 = 15.35 (0x41759999), K4 = 4.14 (0x40847ae1) and K5 = 0.4 (0x3ecccccc); none has a
// symbol. R(x) below means (float)(int)eRand * 2^-16.
//  1. Nothing happens when the model is null. When G is non-null, bit 0x2 of its word at +12 is set.
//  2. n is modelGetNmShadowPoints(model) divided by 2 (rounded toward zero). For i = 0 .. n-1 (nothing when n
//     is not positive), with a running total T that starts at 0.0 and is kept across i:
//     modelGetShadowPos(model, animation, the Point3, heading, i, &out, &value, null, A) is called, and
//     T += value*value*value.
//  3. While T > 0: T -= K2; and a particle is added. The position P is three eRand draws (x, y, z), each
//     (R - 0.5) * (value * K1 * K3), plus out. The velocity V is three more draws, each R * K4. When B is not
//     zero, (icos(heading) * B, isin(heading) * B, 0.0) is added to P (icos is called before isin).
//     Then P_AddParticle(disintoInnoruukParticle, &P, &V, R * K5) is called with one more eRand draw.
//     The eRand draws run in the order given: six for the vectors, then one for the age.
// The heading argument is truncated to a short inside the function (the image shifts it left and arithmetically
// right by 16 itself), so it is declared as an int here. disintoInnoruukParticle is the file-local object at 0x007d7060. Unknown: what the shadow points and the
// value are, and what the constants mean. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern unsigned char disintoInnoruukParticle[];
unsigned eRand__10GameObject(void* self);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short heading, int index, Point3* outPoint, float* outValue, void* matrix, float scale);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

void deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff(
    void* self, void* object, void* model, void* animation, Point3 position, int headingArgument, float a, float b) {
    using namespace w11a;
    const short heading = static_cast<short>(headingArgument);
    if (model == nullptr) return;
    if (object != nullptr) word(static_cast<unsigned char*>(object), 12) |= 2u;
    int n = modelGetNmShadowPoints__FP12_modelHeader(model) / 2;
    float total = 0.0f;
    for (int i = 0; i < n; ++i) {
        Point3 out;
        float value;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, animation, position, heading, i, &out, &value, nullptr, a);
        total += value * value * value;
        while (0.0f < total) {
            const float spread = value * bits(0x3727c5acu) * bits(0x41759999u);
            total -= bits(0x54da475au);
            const float rx = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            const float ry = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            const float rz = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            Point3 p;
            p.x = (rx * bits(0x37800000u) - bits(0x3f000000u)) * spread;
            p.y = (ry * bits(0x37800000u) - bits(0x3f000000u)) * spread;
            p.z = (rz * bits(0x37800000u) - bits(0x3f000000u)) * spread;
            p.x += out.x;
            p.y += out.y;
            p.z += out.z;
            const float vx = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            const float vy = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            const float vz = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
            Point3 v;
            v.x = vx * bits(0x37800000u) * bits(0x40847ae1u);
            v.y = vy * bits(0x37800000u) * bits(0x40847ae1u);
            v.z = vz * bits(0x37800000u) * bits(0x40847ae1u);
            if (b != 0.0f) {
                Point3 w;
                w.x = icos__Fi(heading) * b;
                w.y = isin__Fi(heading) * b;
                w.z = 0.0f;
                p.x += w.x;
                p.y += w.y;
                p.z += w.z;
            }
            const float age = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u) * bits(0x3ecccccc);
            P_AddParticle__FP11ParticleDefP6Point3T1f(disintoInnoruukParticle, &p, &v, age);
        }
    }
}
}
