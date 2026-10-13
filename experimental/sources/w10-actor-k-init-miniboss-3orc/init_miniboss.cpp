// Observed initMiniboss__3Orc at 0x0022b960, 56 bytes. Draws one value from the object's random
// source (eRand__10GameObject, called with the object), reduces it modulo 18 as an unsigned
// number, adds one and stores the result as a byte at object+76. The byte therefore lands in
// 1..18. Nothing is returned. Nothing here ran on the EE.
extern "C" {
unsigned eRand__10GameObject(void* self);

void initMiniboss__3Orc(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    o[76] = static_cast<unsigned char>(eRand__10GameObject(self) % 18u + 1u);
}
}
