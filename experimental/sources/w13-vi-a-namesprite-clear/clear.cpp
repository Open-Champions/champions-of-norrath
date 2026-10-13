// Observed: Clear__12VINameSprite at 0x010a1160 is 68 bytes. It stores into the object:
// byte 0 at +68; float 22.0f (0x41b0 high half) at +48; word 6 at +52; word -1 at +64; and
// 1.0f (0x3f80) at +100, +104, +108 and +112. It returns 0. No other bytes are written and
// nothing is read.
// Inferred: a reset of a name label sprite to defaults; the four consecutive floats at
// +100..+112 look like an RGBA colour (opaque white) and +64 an invalid-index sentinel.
// Unknown: field names, and the meaning of 22.0 and 6.
extern "C" {
int Clear__12VINameSprite(unsigned char* sprite) {
    sprite[68] = 0;
    *reinterpret_cast<float*>(sprite + 48) = 22.0f;
    *reinterpret_cast<int*>(sprite + 52) = 6;
    *reinterpret_cast<int*>(sprite + 64) = -1;
    float* color = reinterpret_cast<float*>(sprite + 100);
    color[3] = 1.0f;
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    return 0;
}
}
