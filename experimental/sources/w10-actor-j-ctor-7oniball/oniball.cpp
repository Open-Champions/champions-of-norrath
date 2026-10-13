// Observed __7OniBallG6Point3iP10GameObjectP10_vagHeader at 0x0021a420, 444 bytes (a constructor with
// args: position, int, owner object, vag header). The int and the vag header are not read. "this" offsets
// are bytes; "owner" is the GameObject argument; e1..e5 are successive eRand(this) results.
//  1. The Projectile constructor runs with (this, copy of the position, null, null).
//  2. The table pointer _vt$7OniBall is stored at +72, bytes +128 and +129 are cleared, word +28 is 0.
//  3. e1: halfword +56 = low 16 bits of e1; word +12 gains 0x4000; word +84 = frameNm + 600.
//  4. e2 (signed) % 65536 below 29491: byte +116 = 0 and owner word +928 = 30; otherwise byte +116 = 1 and
//     owner word +936 = 30. Owner word +252 = frameNm.
//  5. Word +88 = (int)(owner float +564 * the float with bits 0x3d4ccccc, just under 0.05). objectAddToRunList(this). Float +24 grows by 36.
//  6. e3: float +104 = float +16 - 50 + (float)(e3 % 100); e4: float +108 = float +20 - 50 + (float)(e4 % 100);
//     e5: float +112 = float +24 + (float)(e5 % 2000) + 36 (with the already increased +24).
// The meaning of the fields is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char vt_OniBall[] __asm__("_vt$7OniBall");
void projectile_construct(void* self, Point3 position, void* owner, void* map)
    __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
unsigned eRand__10GameObject(void* self);
void objectAddToRunList__FP10GameObject(void* object);

void oniball_construct(void* self, Point3 position, int, void* owner, void*) __asm__("__7OniBallG6Point3iP10GameObjectP10_vagHeader");
void oniball_construct(void* self, Point3 position, int, void* ownerObject, void*) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const owner = static_cast<unsigned char*>(ownerObject);
    projectile_construct(self, position, nullptr, nullptr);
    word(o, 72) = to_word(vt_OniBall);
    o[129] = 0;
    o[128] = 0;
    word(o, 28) = 0u;
    const unsigned e1 = eRand__10GameObject(self);
    word(o, 12) |= 0x4000u;
    half(o, 56) = static_cast<short>(e1);
    word(o, 84) = static_cast<unsigned>(frameNm) + 600u;
    const int e2 = static_cast<int>(eRand__10GameObject(self));
    if (e2 % 65536 < 29491) {
        o[116] = 0;
        word(owner, 928) = 30u;
    } else {
        o[116] = 1;
        word(owner, 936) = 30u;
    }
    word(owner, 252) = static_cast<unsigned>(frameNm);
    sword(o, 88) = static_cast<int>(real(owner, 564) * bits(0x3d4ccccc));
    objectAddToRunList__FP10GameObject(self);
    real(o, 24) = real(o, 24) + 36.0f;
    const unsigned e3 = eRand__10GameObject(self);
    real(o, 104) = real(o, 16) - 50.0f + static_cast<float>(static_cast<int>(e3 % 100u));
    const unsigned e4 = eRand__10GameObject(self);
    real(o, 108) = real(o, 20) - 50.0f + static_cast<float>(static_cast<int>(e4 % 100u));
    const unsigned e5 = eRand__10GameObject(self);
    real(o, 112) = real(o, 24) + static_cast<float>(static_cast<int>(e5 % 2000u)) + 36.0f;
}
}
