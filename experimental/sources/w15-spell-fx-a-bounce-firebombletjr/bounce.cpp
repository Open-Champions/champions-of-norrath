// Observed: bounce__13FireBombletJr at 0x0028b540 is 472 bytes in the pinned SLUS-20565 image. Argument: the object o. Nothing
// is returned. Names: pos = floats at o+16, +20, +24; vel = floats at +76, +80, +84; the image globals are cutMode (signed byte),
// frameNm (int) and syncRandState (32-bit word). Float constants held in registers are immediates: 0.5, 40.0, 10.0, 240.0 and
// 2^-16 (0x37800000).
//  1. The unsigned byte at o+88 is incremented.
//  2. vel is halved. When the new vz is below zero its sign is flipped.
//  3. When the signed byte at o+89 is nonzero and (cutMode != 0 or frameNm % 37 == 0, signed remainder), a StaticFire is spawned. The
//     state steps s = s * 0x19660d + 0x3c6ef35f. Four draws u = (float)(s >> 16) * 2^-16 are taken in order and the state is stored
//     after the fourth: x = pos.x + (u1 - 0.5) * (u2 * 40 + 10), y = pos.y + (u3 - 0.5) * (u4 * 40 + 10), z = pos.z (as read after
//     the vz step). blockAlloc__Fi(1192) gives the storage; when it is non-null a fifth step is taken from the stored state
//     (and stored), r = s >> 16, and the constructor __10StaticFireG6Point3fif runs on the storage with (Point3 {x, y, z}, 240.0,
//     120 + 60 * (r % 3), 0.5).
// Inferred: the bounce of a small FireBomblet fireball, spawning a ground fire at a random spot near it now and then. The
// constructor's argument roles are not established. Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" {
extern signed char cutMode;
extern int frameNm;
extern unsigned syncRandState;
void* blockAlloc__Fi(int size);
void __10StaticFireG6Point3fif(void* self, Point3 position, float a, int b, float c);

void bounce__13FireBombletJr(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    o[88] = static_cast<unsigned char>(o[88] + 1);
    real(o, 76) = real(o, 76) * 0.5f;
    real(o, 80) = real(o, 80) * 0.5f;
    real(o, 84) = real(o, 84) * 0.5f;
    if (real(o, 84) < 0.0f) real(o, 84) = -real(o, 84);
    Point3 spot;
    spot.z = real(o, 24);
    if (static_cast<signed char>(o[89]) == 0) return;
    if (cutMode == 0 && frameNm % 37 != 0) return;
    const float scale = bits(0x37800000u);
    unsigned s = syncRandState;
    s = s * 0x19660du + 0x3c6ef35fu;
    const float u1 = static_cast<float>(static_cast<int>(s >> 16)) * scale;
    s = s * 0x19660du + 0x3c6ef35fu;
    const float u2 = static_cast<float>(static_cast<int>(s >> 16)) * scale;
    s = s * 0x19660du + 0x3c6ef35fu;
    const float u3 = static_cast<float>(static_cast<int>(s >> 16)) * scale;
    s = s * 0x19660du + 0x3c6ef35fu;
    const float u4 = static_cast<float>(static_cast<int>(s >> 16)) * scale;
    syncRandState = s;
    spot.x = real(o, 16) + (u1 - 0.5f) * (u2 * 40.0f + 10.0f);
    spot.y = real(o, 20) + (u3 - 0.5f) * (u4 * 40.0f + 10.0f);
    void* const storage = blockAlloc__Fi(1192);
    if (storage == nullptr) return;
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    const unsigned r = syncRandState >> 16;
    __10StaticFireG6Point3fif(storage, spot, 240.0f, static_cast<int>(120u + 60u * (r % 3u)), 0.5f);
}
}
