// Copy__13VISoundSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x0117c2c0, 264 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is
// negative. Otherwise it fetches the new sprite N with Sprite__7VIScenei(scene, id) and copies the 44 bytes S+8..S+51 (eleven floats)
// and the word at S+52 to N. The base-class entry Copy__8VISprite only returns -1.
// Also copies the word at S+64 and the float at S+84 and, when the sound device (the fourth argument) is nonnull, calls
// ShareSound__13VISoundDevicei(sound, word at S+64) without using its result. It returns the id.
// Inferred: S+64 is a sound handle shared with the new sprite. Unknown: the float's meaning and the names.
extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned char* Sprite__7VIScenei(unsigned char* scene, int index);
int ShareSound__13VISoundDevicei(unsigned char* device, int sound);
}

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

extern "C" int Copy__13VISoundSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char* raster, unsigned char* sound, unsigned char* collide, int copyArgument) {
    (void)raster; (void)collide;
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = Sprite__7VIScenei(scene, id);
    copy_base(copy, self);
*reinterpret_cast<int*>(copy + 64) = *reinterpret_cast<int*>(self + 64);
    *reinterpret_cast<float*>(copy + 84) = *reinterpret_cast<float*>(self + 84);
    if (sound != 0) ShareSound__13VISoundDevicei(sound, *reinterpret_cast<int*>(self + 64));
    return id;
}
