// Observed msg_run__19BlindingLightEffect at 0x002b9868, 440 bytes. O is the object, the first argument. Constants named
// by their float bits (no symbol names them): a = 2^-16 (0x37800000), b = 0x413fff59 (about 11.99984) and
// c = 0x42d9bdc5 (about 108.871).
//  - When the signed word frameNm is below the signed word at O+76, twelve particles are added (the loop body runs once,
//    then again while its count is below 12). Each draws four rolls with eRand__10GameObject(O), in this order: R1 is an
//    angle passed to icos__Fi (result X) and later to isin__Fi (result Y; that call is made after R2 is drawn); R2 and R3
//    scale X and Y; R4 gives the height. With D = the int at O+80 converted to a single, the position is
//    (X * (R2 * a * D) + O.x, Y * (R3 * a * D) + O.y, (R4 * a) * b + O.z), where O.x, O.y, O.z are the singles at O+16,
//    +20 and +24, and the velocity is (0, 0, c). P_AddParticle__FP11ParticleDefP6Point3T1f is called with the particle
//    definition record at 0x008d7d10 (80 bytes, a local of the image), the position, the velocity and 0.0.
//  - Otherwise, when O is not null, the entry at offset 8 of the table whose address is the word at O+72 is called on O
//    (adjusted by the entry's signed halfword) with the argument 3.
// Inferred: a burst effect that sprays particles around a point for a number of frames and then ends itself. Unknown: the
// meaning of the words at O+76 (read as a frame limit) and O+80 (read as a radius). Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char norrath_w16_spellmsg_a_view_8d7d10[80];
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);

void msg_run__19BlindingLightEffect(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    if (frameNm < sword(o, 76)) {
        int count = 0;
        do {
            ++count;
            const int angle = eRand__10GameObject(o);
            const float cosine = icos__Fi(angle);
            const float rx = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u) * static_cast<float>(sword(o, 80));
            Point3 position;
            Point3 velocity;
            position.x = cosine * rx;
            const float sine = isin__Fi(angle);
            const float ry = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u) * static_cast<float>(sword(o, 80));
            position.y = sine * ry;
            const float rz = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u) * bits(0x413fff59u);
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            velocity.z = bits(0x42d9bdc5u);
            position.x = position.x + real(o, 16);
            position.y = position.y + real(o, 20);
            position.z = rz + real(o, 24);
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d7d10, &position, &velocity, 0.0f);
        } while (count < 12);
    } else {
        // The original reads through O before testing it for null; keep the compiler from dropping the test.
        __asm__("" : "+r"(o));
        if (o != nullptr) call_entry(o, 8, 3);
    }
}
}
