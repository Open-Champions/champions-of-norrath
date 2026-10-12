#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed setPassableState__8HateHoleb at 0x226338, 172 bytes.
// The state word at object+376 selects the work; the bool argument is not read.
// The door table pointer is the word `world` (here the existing view
// norrath_actors_b_view_4844a0), reloaded for each call, and the door index is
// the word at object+396.
//  State 1: worldMoveDoor(world, index, {0, 0, -350.0}, 0), then
//    worldSetDoorHeight(world, index, float at object+408 minus 350.0).
//  State 2: worldSetDoorHeight(world, index, float at object+408 plus 256.0).
//  Any other state: nothing.
// No value is returned. The point is passed by address, as in the GothTrap and
// HateBridge bodies. Unknown: the meaning of the states and of the float at +408.
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short extra);
extern "C" int worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height);
extern "C" unsigned char norrath_actors_b_view_4844a0[];

extern "C" void setPassableState__8HateHoleb(void* self, int passable) {
    (void)passable;
    using namespace w8c;
    const Address base = address(self);
    const unsigned state = read_word(base + 376u);
    if (state == 1u) {
        const float point[3] = {0.0f, 0.0f, single_from_bits(0xc3af0000u)};
        worldMoveDoor__FP12_worldHeaderiG6Point3s(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                                  signed_word(read_word(base + 396u)), point, 0);
        worldSetDoorHeight__FP12_worldHeaderif(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                               signed_word(read_word(base + 396u)),
                                               read_single(base + 408u) - single_from_bits(0x43af0000u));
    } else if (state == 2u) {
        worldSetDoorHeight__FP12_worldHeaderif(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                               signed_word(read_word(base + 396u)),
                                               read_single(base + 408u) + single_from_bits(0x43800000u));
    }
}
