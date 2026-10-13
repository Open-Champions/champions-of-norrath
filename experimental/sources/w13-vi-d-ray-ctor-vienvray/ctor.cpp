// Observed: __8VIEnvRay at 0x0116d268 is 84 bytes in the pinned SLUS-20565 image and returns nothing. With the object as
// this it stores float 0.0 at +0, +4, +8, +12, +16, +20, +24, +28 and +32, the word 15 at +44, the word 0 at +48, the word 1 at +52
// and the address of _vt$8VIEnvRay at +60. It calls nothing and reads nothing from the object.
// Inferred: three consecutive vectors of three floats start at +0, +12 and +24 (the stores come in three groups of three).
// Unknown: what the words at +44, +48 and +52 select, and the bytes at +36..43 and +56..59, which are left untouched;
// the class table is an import. Nothing here ran on the EE.
extern "C" {
extern unsigned char _vt$8VIEnvRay[] __asm__("_vt$8VIEnvRay");

void __8VIEnvRay(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    float* v = reinterpret_cast<float*>(bytes);
    for (int i = 0; i < 9; ++i) v[i] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 44) = 15;
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$8VIEnvRay));
    *reinterpret_cast<unsigned*>(bytes + 52) = 1;
    *reinterpret_cast<unsigned*>(bytes + 48) = 0;
}
}
