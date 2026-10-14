// Observed msg_run__9dustCloud at 0x001e1190, 1052 bytes. O is the object. N = the signed word at O+92, G = the float
// at O+96, L = the signed word at O+88. "entry 8" is the entry at table byte 8 of the table whose address is the word at
// O+72, called on O (adjusted by the entry's signed halfword). Steps in order:
//  1. Unless the global frameNm is below (signed) L: entry 8 is called with 3 and that ends it.
//  2. When G is below float(N), G gains 0.5.
//  3. Passes: c = N / 10 (signed division, re-evaluated with the current N after every pass). While c is above zero, for
//     a pass counter that starts at 0 and counts up while below c: a unit-ball vector D is drawn by rejection (each
//     component float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//     1.0 < x*x + y*y + z*z); the position P = O+16 + D * G (with G re-read); a second unit-ball vector E scaled by 5.0 is
//     the velocity; P_AddParticle__FP11ParticleDefP6Point3T1f(the file-local particle record at 0x007d69c0, P, E * 5.0,
//     0.0) is called.
//  4. When frameNm has a remainder other than zero by 60 (signed), that ends it. Otherwise r = (int)G, and the objects in
//     the box O+16 -/+ float(r) on each axis are found with objectFindInBox__FG6Point3T0PP10GameObjecti (capacity 64, a
//     local list ended by a null word). For each listed object T in order: when the word at O+100 is zero and the
//     run-time type of T (read the way the image does it: the function at word +4 of the table that the word at T+72
//     points to, called with no arguments) equals __tf7Cyclops (compared through __eq__C9type_infoRC9type_info), T is
//     skipped. Otherwise gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T, null, float(word O+104),
//     float(word O+104), 100000, 0, 0, 0x5000040, null, false) runs.
// The particle record is a file-local object of the original unit (two statics share the name hitParticleDust2), so it is
// imported under an address name. The image also tests O and each listed T against null before using them; those tests
// cannot be true here and are not reproduced.
// Inferred: a dust cloud that damages everything around it once a second, except Cyclops, unless the word at +100 is
// set. Unknown: the meaning of the words at +88, +92, +100 and +104. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char dustcloud_msg_run_particle_7d69c0[];
int eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
void* __tf7Cyclops();
bool __eq__C9type_infoRC9type_info(const void* left, const void* right);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);

void msg_run__9dustCloud(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(frameNm < sword(o, 88))) {
        call_entry(o, 8, 3);
        return;
    }
    if (real(o, 96) < static_cast<float>(sword(o, 92))) real(o, 96) = real(o, 96) + 0.5f;
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    int count = sword(o, 92) / 10;
    if (count > 0) {
        int pass = 0;
        do {
            Point3 d;
            unit_ball(o, d, rand);
            const float g = real(o, 96);
            Point3 position;
            position.x = d.x * g + real(o, 16);
            position.y = d.y * g + real(o, 20);
            position.z = d.z * g + real(o, 24);
            Point3 velocity;
            unit_ball(o, velocity, rand);
            velocity.x = velocity.x * 5.0f;
            velocity.y = velocity.y * 5.0f;
            velocity.z = velocity.z * 5.0f;
            ++pass;
            P_AddParticle__FP11ParticleDefP6Point3T1f(dustcloud_msg_run_particle_7d69c0, &position, &velocity, 0.0f);
            count = sword(o, 92) / 10;
        } while (pass < count);
    }
    if (frameNm % 60 != 0) return;
    const float r = static_cast<float>(static_cast<int>(real(o, 96)));
    Point3 low, high;
    low.x = real(o, 16) - r;
    low.y = real(o, 20) - r;
    low.z = real(o, 24) - r;
    high.x = real(o, 16) + r;
    high.y = real(o, 20) + r;
    high.z = real(o, 24) + r;
    void* found[64];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
    if (found[0] == nullptr) return;
    void** cursor = found;
    do {
        unsigned char* const target = static_cast<unsigned char*>(*cursor);
        ++cursor;
        if (word(o, 100) == 0u) {
            const void* const type = from_word(call_word(word(from_word(word(target, 72)), 4)));
            if (__eq__C9type_infoRC9type_info(type, __tf7Cyclops())) continue;
        }
        const float amount = static_cast<float>(sword(o, 104));
        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(target, nullptr, amount, amount, 100000, 0, 0, 0x5000040, nullptr, false);
    } while (*cursor != nullptr);
}
}
