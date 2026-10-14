// Observed msg_load__8RollDoorPc at 0x0021c5a0, 176 bytes. Reads four 32-bit words from the
// buffer: w0, w1, a float f and w3. Stores low halfword of w1 at object+76, low halfword of w0
// at +90, f at +80 and w3 at +84. Then worldSetDoorCollision__FP12_worldHeaderii is called with
// the global world pointer, the halfword at +76 (sign-extended) and 1; the halfword at +78 is
// set from worldGetDoorHeight__FP12_worldHeaderi(world, halfword at +76). When the halfword at
// +90 is nonzero, bit 20 (0x100000) of the word at +12 is cleared and the object is added to the
// run list with objectAddToRunList__FP10GameObject. Returns 16, the bytes consumed. Nothing
// here ran on the EE. What the fields mean is unknown.
extern "C" {
extern void* world;
void worldSetDoorCollision__FP12_worldHeaderii(void* header, int door, int value);
int worldGetDoorHeight__FP12_worldHeaderi(void* header, int door);
void objectAddToRunList__FP10GameObject(void* object);

int msg_load__8RollDoorPc(void* self, char* buffer) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int* in = reinterpret_cast<const int*>(buffer);
    const int w0 = in[0];
    const int w1 = in[1];
    const float f = reinterpret_cast<const float*>(in)[2];
    const int w3 = in[3];
    *reinterpret_cast<short*>(o + 76) = static_cast<short>(w1);
    *reinterpret_cast<short*>(o + 90) = static_cast<short>(w0);
    *reinterpret_cast<float*>(o + 80) = f;
    *reinterpret_cast<int*>(o + 84) = w3;
    worldSetDoorCollision__FP12_worldHeaderii(world, static_cast<short>(w1), 1);
    *reinterpret_cast<short*>(o + 78) = static_cast<short>(worldGetDoorHeight__FP12_worldHeaderi(world, *reinterpret_cast<short*>(o + 76)));
    if (*reinterpret_cast<short*>(o + 90) != 0) {
        *reinterpret_cast<unsigned*>(o + 12) &= 0xffefffffu;
        objectAddToRunList__FP10GameObject(self);
    }
    return 16;
}
}
