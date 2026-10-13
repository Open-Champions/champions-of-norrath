#include "../w10-actor-l-common/access.hpp"
// Observed msg_run__8RollDoor at 0x00215be8, 236 bytes. A per-frame step driven by the counter
// word at object+84. Steps, in order:
//  1. If the counter is at least 121, call worldSetDoorCollision(world, halfword at +76, 0). This
//     happens on every frame from 121 on, before the counter is advanced.
//  2. Add one to the counter; if the value before the increment was at least 151, remove the
//     object from the run list, set bit 1 (value 2) in the flag word at +12 and stop.
//  3. Otherwise, with a = halfword at +56 plus 16384: subtract icos(a) * speed from the float at
//     +16 and isin(a) * speed from the float at +20, where speed is the float at +80; then add
//     the global constant 0.01 (observed value, gp-relative float pool) to the speed, and
//     replace the halfword at +88 by itself minus speed * 200.0, converted to an integer by
//     truncation (the speed used is the one before the 0.01 addition).
// Unknown: what the fields mean (a door that rolls on a heading). Nothing here ran on the EE.
extern "C" {
extern void* world;
void worldSetDoorCollision__FP12_worldHeaderii(void* world, int door, int value);
void objectRemoveFromRunList__FP10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);

void msg_run__8RollDoor(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (w10l::sword(o, 84) >= 121) worldSetDoorCollision__FP12_worldHeaderii(world, w10l::half(o, 76), 0);
    const int counter = w10l::sword(o, 84);
    w10l::sword(o, 84) = counter + 1;
    if (counter >= 151) {
        objectRemoveFromRunList__FP10GameObject(self);
        w10l::word(o, 12) |= 2u;
        return;
    }
    w10l::real(o, 16) = w10l::real(o, 16) - icos__Fi(w10l::half(o, 56) + 16384) * w10l::real(o, 80);
    const float sine = isin__Fi(w10l::half(o, 56) + 16384);
    const float speed = w10l::real(o, 80);
    w10l::real(o, 20) = w10l::real(o, 20) - sine * speed;
    w10l::real(o, 80) = speed + 0.01f;
    w10l::half(o, 88) = static_cast<short>(static_cast<int>(static_cast<float>(w10l::half(o, 88)) - speed * 200.0f));
}
}
