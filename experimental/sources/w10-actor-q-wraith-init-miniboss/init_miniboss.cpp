#include "../w10-actor-q-common/q.hpp"
// Observed initMiniboss__6Wraith at 0x002052b0, 56 bytes.
// Draws one value with eRand__10GameObject on the object, takes it modulo 20 as an unsigned
// number, adds one and stores the low byte at object+76. The result is 1 to 20.
// Inferred: this selects the variant that getMinibossName__6Wraith turns into a name.
// Nothing here ran on the EE.
extern "C" {
int eRand__10GameObject(void* self);
}

extern "C" void initMiniboss__6Wraith(void* self) {
    const unsigned draw = static_cast<unsigned>(eRand__10GameObject(self));
    w10q::ubyte(self, 76) = static_cast<unsigned char>(draw % 20u + 1u);
}
