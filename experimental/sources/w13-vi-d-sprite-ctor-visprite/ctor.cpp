// Observed: __8VISprite at 0x01007ed0 is 128 bytes in the pinned SLUS-20565 image and returns nothing. With the object as this it
// stores: the address of _vt$8VISprite at +60; the word -1 at +56; zero words at +0, +4 and +52; float 0.0 at +8, +12, +16 and
// at +20, +24, +28 (two three-float vectors); float 0.0 at +32, +36, +40 and +44 (four floats); and float 500.0 at +48.
// It calls nothing and reads nothing from the object. The image stores the two vectors twice (an inlined member constructor
// followed by an explicit zeroing); the final bytes are the same as storing once.
// Inferred: the word at +60 is the class table, as for the other VISprite subclasses; +32..47 is a four-float value.
// Unknown: what each field means and the object's size (subclasses extend it past +84); the class table is an import.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char _vt$8VISprite[] __asm__("_vt$8VISprite");

void __8VISprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    float* f = reinterpret_cast<float*>(bytes);
    for (int i = 2; i <= 4; ++i) f[i] = 0.0f;
    for (int i = 5; i <= 7; ++i) f[i] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$8VISprite));
    *reinterpret_cast<unsigned*>(bytes + 56) = static_cast<unsigned>(-1);
    *reinterpret_cast<unsigned*>(bytes + 4) = 0;
    *reinterpret_cast<unsigned*>(bytes + 0) = 0;
    for (int i = 8; i <= 11; ++i) f[i] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 52) = 0;
    f[12] = 500.0f;
}
}
