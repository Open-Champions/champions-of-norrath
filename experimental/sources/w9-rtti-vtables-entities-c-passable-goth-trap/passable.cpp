#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setPassableState__8GothTrapb at 0x225cc0, 300 bytes. The world pointer is the word at
// 0x4844a0 (norrath_factory_world_pointer_word); the door index is the word at
// object+396 and the door's resting height the single at +400. The flag
// argument is not used. Steps:
// 1. worldSetDoorCollision(world, index, 1).
// 2. State word at object+376 equal to 0: worldMoveDoor with the point
//    (0, 0, 0) and shift 0, then worldSetDoorHeight with the single at +400.
// 3. The state is read again after those calls. State 1: worldMoveDoor with the
//    point (0, 0, -350.0) and shift 0, then worldSetDoorHeight with the +400
//    single minus 350.0.
// 4. State 2: worldMoveDoor with the point (0, 0, -350.0) and shift 0, then worldSetDoorHeight with the +400 single plus 256.0. Other states do nothing more.
// The Point3 argument is passed by address of a stack copy. Return values of
// the world calls are not read.
// Unknown: the meaning of the states, and the unit of the shift argument.
extern "C" {
extern unsigned char norrath_factory_world_pointer_word[];
void worldSetDoorCollision__FP12_worldHeaderii(void* world, int index, int value);
void worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int index, const void* point, short shift);
void worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height);
}

namespace {
void* world() { return rtti_c::pointer(rtti_c::read_word(rtti_c::address(norrath_factory_world_pointer_word))); }
int door(rtti_c::Address object) { return static_cast<int>(rtti_c::read_word(object + 396u)); }
}

extern "C" void setPassableState__8GothTrapb(void* self, int) {
    using namespace rtti_c;
    const Address object = address(self);
    worldSetDoorCollision__FP12_worldHeaderii(world(), door(object), 1);
    if (read_word(object + 376u) == 0u) {
        const float point[3] = {0.0f, 0.0f, 0.0f};
        worldMoveDoor__FP12_worldHeaderiG6Point3s(world(), door(object), point, 0);
        worldSetDoorHeight__FP12_worldHeaderif(world(), door(object), read_single(object + 400u));
    }
    const unsigned state = read_word(object + 376u);
    if (state == 1u) {
        const float point[3] = {0.0f, 0.0f, -350.0f};
        worldMoveDoor__FP12_worldHeaderiG6Point3s(world(), door(object), point, 0);
        worldSetDoorHeight__FP12_worldHeaderif(world(), door(object), read_single(object + 400u) - 350.0f);
    } else if (state == 2u) {
        const float point[3] = {0.0f, 0.0f, -350.0f};
        worldMoveDoor__FP12_worldHeaderiG6Point3s(world(), door(object), point, 0);
        worldSetDoorHeight__FP12_worldHeaderif(world(), door(object), read_single(object + 400u) + 256.0f);
    }
}
