// Copy__20VIEffectVolumeSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x0114e5a8, 324 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is negative.
// Otherwise it fetches the new sprite N with Sprite__7VIScenei(scene, id) and then, in this order: calls Init__20VIEffectVolumeSpriteRC6VIBBox(N, S + 8); copies the
// word at S+52 and the float at S+48 to N; calls Init__14VIEffectVolumeRC6VIBBox(N + 88, S + 8); sets the type and density of the embedded volume
// at N+88 with SetType__14VIEffectVolume18VIEffectVolumeTypef(N + 88, Type__C14VIEffectVolume(S + 88), Density__C14VIEffectVolume(S + 88)); copies the floats at
// S+140 and S+144 to N+140 and N+144; calls SetFlags__14VIEffectVolumeUi(N + 88, Flags__C14VIEffectVolume(S + 88)); copies the floats at S+136 and S+128 to
// N+136 and N+128; then attaches surfaces 0 and 1 with AttachSurface__14VIEffectVolumeii(N + 88, k, Surface__C14VIEffectVolumei(S + 88, k)). It returns the id.
// The raster, sound device and collide arguments are not used. Unlike the other copies it does not copy the S+8..S+47 floats directly; the two initializers
// receive the box at S+8 instead.
// Inferred: the object at S+88 is an embedded effect-volume record whose fields are read through accessors (Type reads +36, Density +44, Flags +60,
// Surface(k) +64+4k, as observed in the image). Unknown: the layout of the box and the record beyond those offsets, and the type/flag semantics.
extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned char* Sprite__7VIScenei(unsigned char* scene, int index);
void Init__20VIEffectVolumeSpriteRC6VIBBox(unsigned char* sprite, const unsigned char* box);
void Init__14VIEffectVolumeRC6VIBBox(unsigned char* volume, const unsigned char* box);
int Type__C14VIEffectVolume(const unsigned char* volume);
float Density__C14VIEffectVolume(const unsigned char* volume);
void SetType__14VIEffectVolume18VIEffectVolumeTypef(unsigned char* volume, int type, float density);
unsigned Flags__C14VIEffectVolume(const unsigned char* volume);
void SetFlags__14VIEffectVolumeUi(unsigned char* volume, unsigned flags);
int Surface__C14VIEffectVolumei(const unsigned char* volume, int index);
void AttachSurface__14VIEffectVolumeii(unsigned char* volume, int index, int surface);

int Copy__20VIEffectVolumeSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char*, unsigned char*, unsigned char*, int copyArgument) {
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = Sprite__7VIScenei(scene, id);
    unsigned char* volume = copy + 88;
    const unsigned char* source = self + 88;
    Init__20VIEffectVolumeSpriteRC6VIBBox(copy, self + 8);
    *reinterpret_cast<int*>(copy + 52) = *reinterpret_cast<int*>(self + 52);
    *reinterpret_cast<float*>(copy + 48) = *reinterpret_cast<float*>(self + 48);
    Init__14VIEffectVolumeRC6VIBBox(volume, self + 8);
    int type = Type__C14VIEffectVolume(source);
    SetType__14VIEffectVolume18VIEffectVolumeTypef(volume, type, Density__C14VIEffectVolume(source));
    *reinterpret_cast<float*>(volume + 52) = *reinterpret_cast<const float*>(source + 52);
    *reinterpret_cast<float*>(volume + 56) = *reinterpret_cast<const float*>(source + 56);
    SetFlags__14VIEffectVolumeUi(volume, Flags__C14VIEffectVolume(source));
    *reinterpret_cast<float*>(volume + 48) = *reinterpret_cast<const float*>(source + 48);
    *reinterpret_cast<float*>(volume + 40) = *reinterpret_cast<const float*>(source + 40);
    AttachSurface__14VIEffectVolumeii(volume, 0, Surface__C14VIEffectVolumei(source, 0));
    AttachSurface__14VIEffectVolumeii(volume, 1, Surface__C14VIEffectVolumei(source, 1));
    return id;
}
}
