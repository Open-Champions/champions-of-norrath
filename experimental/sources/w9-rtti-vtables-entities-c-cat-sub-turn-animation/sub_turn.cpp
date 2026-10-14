#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed subTurnAnimation__3Cat at 0x1eef20, 16 bytes. Loads one single from
// the small-data constant at gp-27948 (0x480cc4, bit pattern 0xbd088888,
// about -0.0333) and stores it to object+404 and object+372. Nothing is read
// from the object and nothing is returned.
// Unknown: what the two fields hold; the same constant is also stored at +436
// by switchToWalk__3Cat.
extern "C" void subTurnAnimation__3Cat(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    const float value = single_from_bits(0xbd088888u);
    write_single(object + 404u, value);
    write_single(object + 372u, value);
}
