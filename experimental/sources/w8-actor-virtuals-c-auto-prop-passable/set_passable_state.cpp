#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed setPassableState__8AutoPropb at 0x221040, 212 bytes.
// The door table pointer is the word `world` at 0x4844a0 (here the existing
// view norrath_actors_b_view_4844a0), reloaded for every call. The door index is
// the word at object+396. Nothing is done to the door unless bit 0x1 of the word
// at object+284 is set.
//  Nonzero argument: with that bit set, worldSetDoorCollision(world, index, 0),
//    then worldMoveDoor(world, index, {0, 0, -256}, 0); in every case bit 0x1 of
//    the word at object+12 is then set.
//  Zero argument: with that bit set, worldSetDoorCollision(world, index, 1),
//    then worldMoveDoor(world, index, {0, 0, 0}, 0); in every case bit 0x1 of the
//    word at object+12 is then cleared.
// The point is passed by address, as in the GothTrap and HateBridge bodies. No
// value is returned. Unknown: the meaning of bit 0x1 at +284 and at +12, and the
// units of the door move.
extern "C" int worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int flag);
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short extra);
extern "C" unsigned char norrath_actors_b_view_4844a0[];

extern "C" void setPassableState__8AutoPropb(void* self, int passable) {
    using namespace w8c;
    const Address base = address(self);
    unsigned flags;
    if (passable != 0) {
        if ((read_word(base + 284u) & 1u) != 0u) {
            worldSetDoorCollision__FP12_worldHeaderii(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                                      signed_word(read_word(base + 396u)), 0);
            const float point[3] = {0.0f, 0.0f, single_from_bits(0xc3800000u)};
            worldMoveDoor__FP12_worldHeaderiG6Point3s(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                                      signed_word(read_word(base + 396u)), point, 0);
        }
        flags = read_word(base + 12u) | 1u;
    } else {
        if ((read_word(base + 284u) & 1u) != 0u) {
            worldSetDoorCollision__FP12_worldHeaderii(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                                      signed_word(read_word(base + 396u)), 1);
            const float point[3] = {0.0f, 0.0f, 0.0f};
            worldMoveDoor__FP12_worldHeaderiG6Point3s(pointer(read_word(address(norrath_actors_b_view_4844a0))),
                                                      signed_word(read_word(base + 396u)), point, 0);
        }
        flags = read_word(base + 12u) & ~1u;
    }
    write_word(base + 12u, flags);
}
