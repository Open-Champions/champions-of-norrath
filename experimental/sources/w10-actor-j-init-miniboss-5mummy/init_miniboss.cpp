// Observed initMiniboss__5Mummy at 0x0023be40, 56 bytes. Stores (eRand(this) % 18) + 1 as a byte
// at object+76; the random value is treated as unsigned. What the byte selects is inferred from
// getMinibossName__5Mummy, which formats it into a name. Nothing here ran on the EE.
extern "C" {
unsigned eRand__10GameObject(void* self);

void initMiniboss__5Mummy(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    o[76] = static_cast<unsigned char>(eRand__10GameObject(self) % 18u + 1u);
}
}
