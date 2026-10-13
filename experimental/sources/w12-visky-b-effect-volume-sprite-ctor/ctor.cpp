// Observed: __20VIEffectVolumeSprite at 0x0114df60, 124 bytes. With the object as this it calls
// __8VISprite(this); stores the address of _vt$20VIEffectVolumeSprite (0x01226580) at this+60;
// stores 0.0f in the three floats at this+64, +68, +72 and in the three at this+76, +80, +84;
// calls __14VIEffectVolume(this + 88); then stores 14 at this+52. Nothing is returned.
// Inferred: the embedded VIEffectVolume sits at +88; two three-float vectors precede it; the
// value 14 at +52 is the sprite type (CreateSprite builds this class for type 14).
// Unknown: what the two vectors represent.
extern "C" {
void __8VISprite(void* self);
void __14VIEffectVolume(void* self);
extern unsigned char _vt$20VIEffectVolumeSprite[];

void __20VIEffectVolumeSprite(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __8VISprite(self);
    float* first = reinterpret_cast<float*>(bytes + 64);
    float* second = reinterpret_cast<float*>(bytes + 76);
    first[0] = 0.0f;
    first[1] = 0.0f;
    first[2] = 0.0f;
    *reinterpret_cast<unsigned*>(bytes + 60) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$20VIEffectVolumeSprite));
    second[0] = 0.0f;
    second[1] = 0.0f;
    second[2] = 0.0f;
    __14VIEffectVolume(bytes + 88);
    *reinterpret_cast<int*>(bytes + 52) = 14;
}
}
