// Observed msg_run__7OniBall at 0x0021a5e0, 1292 bytes. O is the ball; P is its position (floats at +16, +20,
// +24); a "player record" is player + 23984 * p, whose word at +21872 holds the address of that player's object
// (read only for its position at +16..+24). Steps in order:
//  1. Nearest player: best = k0 (global float at gp - 25540, bits 0x4cbebc20, 1.0e8) and no player. For each of
//     the nmPlayersAlive entries p of playersAlive (the count is read again after each entry; the first entry is
//     read only when the count is above zero): when playerGetSkillRank__Fii(p, 59) is above zero, d = the
//     distance from P to that player's object position, summed x, y, z in that order; a player with d below
//     best becomes the nearest.
//  2. eRand__10GameObject(O): when the result modulo 65536 is below 5160, a new wander target is stored:
//     O+104 = P.x - 500 + float(eRand % 1000), O+108 = P.y - 500 + float(eRand % 1000), O+112 = P.z - float(eRand
//     % 500) + 500 (the remainders are unsigned; three further eRand calls).
//  3. turn = 1638 and speed = 200. A nearest player p counts as a target when the float at its record +172
//     is above zero and the word blockButton[p] ANDed with the word at pad + 88p + 8 is non-zero; then the target
//     point (+104, +108, +112) is that player's object position with 36.0 added to z, and the ball is "homing".
//     When homing, d = the horizontal distance from P to the target point and speed = (int)(clamp(d * k1, k2,
//     1.0) * 200) (k1 at gp - 25536, bits 0x3c638e38, 1/72 rounded; k2 at gp - 25532, bits 0x3f199999, 0.6 rounded;
//     the clamp tests 1.0 first, then k2). When not homing turn = 1638 / 5 = 327.
//  4. Heading: the halfword at +56 (h) turns towards iatan2__Fff(target.y - P.y, target.x - P.x): with
//     diff = (short)(angle - h), the new h is h + diff when |diff| < turn, else h + turn for a positive diff and
//     h - turn otherwise. Then O+92 = icos(h) * speed and O+96 = isin(h) * speed (new h read back as signed).
//     dz = (short)(int)(target.z - P.z): the word at +100 becomes 0 when -9 <= dz <= 9, else +200.0 for a positive
//     dz and -200.0 otherwise.
//  5. When not homing, (+92, +96, +100) are overwritten with (icos(h) * 60, isin(h) * 60, 50.0).
//  6. A zero-velocity particle is added at P with P_AddParticle__FP11ParticleDefP6Point3T1f: the record at
//     0x007d7810 (oniRedParticle) when the signed byte at +116 is zero, else the record at 0x007d78b0
//     (oniBlueParticle) (both file-local objects of the original unit, imported here under address names).
//  7. move__10ProjectileG6Point3(O, the three floats at +92) runs (its result is ignored).
//  8. When frameNm is not below the signed word at +84, slot byte 8 of the table at +72 is called on O with the
//     argument 3 (a delete request, by the pattern of the other classes).
// Inferred: the ball chases the nearest player holding a button and otherwise wanders; the skill id 59 is the one the
// SkillLifeTap pickup in msg_collision__7OniBallP10GameObject also tests. Unknown: the meanings of the other fields
// and of the floats k0..k2 beyond their values. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern int nmPlayersAlive;
extern int playersAlive[];
extern unsigned blockButton[];
extern unsigned char pad[];
extern unsigned char player[];
extern unsigned char oniball_msg_run_particle_7d7810[];
extern unsigned char oniball_msg_run_particle_7d78b0[];
int playerGetSkillRank__Fii(int player, int skill);
int eRand__10GameObject(void* self);
int iatan2__Fff(float y, float x);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale);
int move__10ProjectileG6Point3(void* self, Point3 v);

void msg_run__7OniBall(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    float best = bits(0x4cbebc20u);
    int nearest = -1;
    if (nmPlayersAlive > 0) {
        int i = 0;
        do {
            const int p = playersAlive[i];
            if (playerGetSkillRank__Fii(p, 59) > 0) {
                unsigned char* const object = from_word(word(player + p * 23984, 21872));
                const float dx = real(object, 16) - position->x;
                const float dy = real(object, 20) - position->y;
                const float dz = real(object, 24) - position->z;
                const float d = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
                if (d < best) {
                    nearest = p;
                    best = d;
                }
            }
            ++i;
        } while (i < nmPlayersAlive);
    }
    bool homing = false;
    if (eRand__10GameObject(o) % 65536 < 5160) {
        real(o, 104) = real(o, 16) - 500.0f + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(o)) % 1000u));
        real(o, 108) = real(o, 20) - 500.0f + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(o)) % 1000u));
        real(o, 112) = real(o, 24) - static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(o)) % 500u)) + 500.0f;
    }
    int turn = 1638;
    int speed = 200;
    if (nearest != -1) {
        unsigned char* const record = player + nearest * 23984;
        if (0.0f < real(record, 172) && (blockButton[nearest] & word(pad + nearest * 88, 8)) != 0u) {
            unsigned char* const object = from_word(word(record, 21872));
            homing = true;
            real(o, 104) = real(object, 16);
            real(o, 108) = real(object, 20);
            real(o, 112) = real(object, 24) + 36.0f;
        }
    }
    if (homing) {
        const float dy = real(o, 108) - position->y;
        const float dx = real(o, 104) - position->x;
        const float d = __builtin_sqrtf(dx * dx + dy * dy);
        float scaled = d * bits(0x3c638e38u);
        if (1.0f < scaled) scaled = 1.0f;
        else if (scaled < bits(0x3f199999u)) scaled = bits(0x3f199999u);
        speed = static_cast<int>(scaled * static_cast<float>(speed));
    } else {
        turn = turn / 5;
    }
    const int angle = iatan2__Fff(real(o, 108) - real(o, 20), real(o, 104) - real(o, 16));
    const unsigned h = static_cast<unsigned short>(half(o, 56));
    const short diff = static_cast<short>(angle - static_cast<int>(h));
    const int magnitude = diff >= 0 ? diff : -diff;
    int updated;
    if (magnitude < turn) updated = diff + static_cast<int>(h);
    else if (diff > 0) updated = static_cast<int>(h) + turn;
    else updated = static_cast<int>(h) - turn;
    half(o, 56) = static_cast<short>(updated);
    const float speedf = static_cast<float>(speed);
    real(o, 92) = icos__Fi(half(o, 56)) * speedf;
    real(o, 96) = isin__Fi(half(o, 56)) * speedf;
    const short dz = static_cast<short>(static_cast<int>(real(o, 112) - real(o, 24)));
    if (static_cast<unsigned short>(dz + 9) < 19u) word(o, 100) = 0u;
    else if (dz > 0) real(o, 100) = 200.0f;
    else real(o, 100) = -200.0f;
    if (!homing) {
        const float sixty = 60.0f;
        const float vx = icos__Fi(half(o, 56)) * sixty;
        const float vy = isin__Fi(half(o, 56)) * sixty;
        real(o, 92) = vx;
        real(o, 96) = vy;
        real(o, 100) = 50.0f;
    }
    Point3 still;
    still.x = 0.0f;
    still.y = 0.0f;
    still.z = 0.0f;
    if (static_cast<signed char>(o[116]) == 0) P_AddParticle__FP11ParticleDefP6Point3T1f(oniball_msg_run_particle_7d7810, position, &still, 0.0f);
    else P_AddParticle__FP11ParticleDefP6Point3T1f(oniball_msg_run_particle_7d78b0, position, &still, 0.0f);
    move__10ProjectileG6Point3(o, *reinterpret_cast<Point3*>(o + 92));
    if (!(frameNm < sword(o, 84))) call_entry(o, 8, 3);
}
}
