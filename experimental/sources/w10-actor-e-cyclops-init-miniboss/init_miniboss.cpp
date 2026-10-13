#include "../vslots-b-bytes/bytes.hpp"
// Observed initMiniboss__7Cyclops at 0x203c60, 56 bytes.
// Calls eRand on the object, takes the result as an unsigned value modulo nine,
// adds one, and stores the 1..9 result as a byte at object+76.
// getMinibossName__7Cyclops reads the same byte. Unknown: what the byte selects beyond that name.
extern "C" int eRand__10GameObject(void* object);
extern "C" void initMiniboss__7Cyclops(void* self) {
    using namespace vslots_b;
    const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self));
    write_u8(address(self) + 76u, roll % 9u + 1u);
}
