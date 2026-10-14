// Observed initMiniboss__10SeaMonster at 0x001f8b90, 56 bytes. Draws one value from the object's
// random source (eRand__10GameObject, called with the object), reduces it modulo 18 as an
// unsigned number, adds one and stores the result as a byte at object+76, giving 1..18. Nothing
// is returned. Same shape as the Orc routine. Nothing here ran on the EE.
extern "C" {
unsigned eRand__10GameObject(void* self);

void initMiniboss__10SeaMonster(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    o[76] = static_cast<unsigned char>(eRand__10GameObject(self) % 18u + 1u);
}
}
