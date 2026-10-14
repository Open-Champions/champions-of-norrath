// Observed initMiniboss__6Goblin at 0x0022f4f8, 56 bytes.
// Stores 1 + (eRand__10GameObject() % 15, unsigned remainder) as a byte at object+76.
// Inferred: a random miniboss number from 1 to 15. Nothing here ran on the EE.
extern "C" unsigned eRand__10GameObject(void* self);

extern "C" void initMiniboss__6Goblin(void* self) {
    const unsigned roll = eRand__10GameObject(self);
    *(static_cast<unsigned char*>(self) + 76) = static_cast<unsigned char>(roll % 15u + 1u);
}
