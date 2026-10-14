// Observed: __9VISurface at 0x0112c750 is 116 bytes and returns nothing. Offsets
// written: +0, +4, +8, +12 = 0; +16 = 2; +20, +24, +28 = -1; +32 = 1; +36, +56, +64,
// +308, +312 = 0; then ten words at +300, +276, ... , +84 (stride -24) = 0.
// Inferred: a 24-byte record array of at least ten entries starting at +60; the
// -1 words look like "unset" ids. Unknown: field meanings and the object's size.
extern "C" {
void __9VISurface(int* self) {
    self[7] = -1;
    self[8] = 1;
    self[1] = 0;
    self[0] = 0;
    self[2] = 0;
    self[3] = 0;
    self[5] = -1;
    self[6] = -1;
    self[9] = 0;
    self[16] = 0;
    self[77] = 0;
    self[78] = 0;
    self[14] = 0;
    self[4] = 2;
    for (int i = 0; i < 10; ++i)
        self[75 - i * 6] = 0;
}
}
