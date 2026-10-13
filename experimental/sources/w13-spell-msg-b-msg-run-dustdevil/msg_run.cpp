// Observed msg_run__9DustDevil at 0x0023b2e8, 672 bytes. O is the object. Nothing happens unless the global frameNm is
// below (signed) the word at O+80. Otherwise, in order:
//  1. When the float at O+76 is below 96.0 it gains 2.0.
//  2. P_OpenParticleSystem__Fi(the word at +84) opens a particle system; a local matrix M is made with
//     matMakeTrans__FP8Matrix34G6Point3(M, the three floats at O+16) and rotated with matRot__FP8Matrix34is(M, 2,
//     (short)((frameNm & 15) << 12)); then P_SetParticleSystemMatrix__FiP8Matrix34(the word at +84, M).
//  3. n = 2 * (int)(float at O+32), re-read after every pass. While the pass count is below n, a pass is:
//       h = float(eRand__10GameObject(O)) * 2^-16 * (float at O+76); a = (short)eRand__10GameObject(O);
//       d = (float(eRand__10GameObject(O)) * 2^-16) * (h * 0.25 + 16.0);
//       the position (cos(a) * d, sin(a) * d, h), cos and sin being icos__Fi(a) and isin__Fi(a);
//       a unit-ball vector D (each component float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z,
//       redrawn while 1.0 < x*x + y*y + z*z) scaled by the float 0x41273333 (10.45) read through the global pointer;
//       P_AddParticle__FP11ParticleDefP6Point3T1f(fxDust, position, D * 10.45, 0.0).
//  4. P_CloseParticleSystem__Fi(the word at +84) closes the system, also when n was not positive.
// Inferred: dust thrown up by a dust devil inside a local particle frame. Unknown: the meaning of the words at +80 and
// +84 and of the floats at +32 and +76. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

struct Matrix34 { float m[12]; };

extern "C" {
extern int frameNm;
extern unsigned char fxDust[];
int eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_OpenParticleSystem__Fi(int system);
void P_CloseParticleSystem__Fi(int system);
void P_SetParticleSystemMatrix__FiP8Matrix34(int system, Matrix34* matrix);
void matMakeTrans__FP8Matrix34G6Point3(Matrix34* matrix, Point3 position);
void matRot__FP8Matrix34is(Matrix34* matrix, int axis, short angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

void msg_run__9DustDevil(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(frameNm < sword(o, 80))) return;
    if (real(o, 76) < 96.0f) real(o, 76) = real(o, 76) + 2.0f;
    P_OpenParticleSystem__Fi(sword(o, 84));
    Matrix34 matrix;
    Point3 here;
    here.x = real(o, 16);
    here.y = real(o, 20);
    here.z = real(o, 24);
    matMakeTrans__FP8Matrix34G6Point3(&matrix, here);
    matRot__FP8Matrix34is(&matrix, 2, static_cast<short>(static_cast<int>(static_cast<unsigned>(static_cast<unsigned short>(frameNm)) << 28) >> 16));
    P_SetParticleSystemMatrix__FiP8Matrix34(sword(o, 84), &matrix);
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    if (static_cast<int>(real(o, 32)) * 2 > 0) {
        int pass = 0;
        do {
            const float scale = bits(0x37800000u);
            const float h = static_cast<float>(eRand__10GameObject(o)) * scale * real(o, 76);
            const short angle = static_cast<short>(eRand__10GameObject(o));
            float d = static_cast<float>(eRand__10GameObject(o)) * scale;
            d = d * (h * 0.25f + 16.0f);
            Point3 position;
            position.x = icos__Fi(angle) * d;
            position.y = isin__Fi(angle) * d;
            position.z = h;
            Point3 velocity;
            unit_ball(o, velocity, rand);
            const float speed = bits(0x41273333u);
            velocity.x = velocity.x * speed;
            velocity.y = velocity.y * speed;
            velocity.z = velocity.z * speed;
            ++pass;
            P_AddParticle__FP11ParticleDefP6Point3T1f(fxDust, &position, &velocity, 0.0f);
        } while (pass < static_cast<int>(real(o, 32)) * 2);
    }
    P_CloseParticleSystem__Fi(sword(o, 84));
}
}
