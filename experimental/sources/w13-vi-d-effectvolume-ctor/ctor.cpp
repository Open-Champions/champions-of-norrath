// Observed: __14VIEffectVolume at 0x01146f38 is 148 bytes in the pinned SLUS-20565 image and returns nothing. With the object as
// this it stores: zero words at +0, +4, +36, +60 and +88; float 0.0 at +8, +12, +16 and +20, +24, +28 (two three-float vectors),
// at +52 and +56; float 1.0 at +40 and +44; float 20.0 at +48; the word 1 at +32 and +72; and the word -1 at +64, +68, +76 and
// +80. The bytes at +84 and after +91 are untouched. It calls nothing and reads nothing from the object. The image stores the two
// vectors twice (an inlined member constructor followed by an explicit zeroing); the final bytes are the same as storing once.
// Inferred: the layout of the stores only; the types are from the stored constants.
// Unknown: what each field means and the object size. Nothing here ran on the EE.
extern "C" {
void __14VIEffectVolume(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    float* f = reinterpret_cast<float*>(bytes);
    unsigned* w = reinterpret_cast<unsigned*>(bytes);
    for (int i = 2; i <= 7; ++i) f[i] = 0.0f;
    w[20] = static_cast<unsigned>(-1);
    w[18] = 1;
    w[0] = 0;
    w[1] = 0;
    w[9] = 0;
    f[11] = 1.0f;
    f[12] = 20.0f;
    f[14] = 0.0f;
    f[10] = 1.0f;
    f[13] = 0.0f;
    w[15] = 0;
    w[16] = static_cast<unsigned>(-1);
    w[17] = static_cast<unsigned>(-1);
    w[19] = static_cast<unsigned>(-1);
    w[22] = 0;
    w[8] = 1;
}
}
