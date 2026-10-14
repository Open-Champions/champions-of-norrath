// Copy__13VIFloraSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x010060b0, 296 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is
// negative. Otherwise it fetches the new sprite N with Sprite__7VIScenei(scene, id) and copies the 44 bytes S+8..S+51 (eleven floats)
// and the word at S+52 to N. The base-class entry Copy__8VISprite only returns -1.
// Also copies the words at S+68 and S+64. When the raster is nonnull it calls ShareMaterialPal__8VIRasteri(raster, word at S+68) and,
// if that result is not negative, SharePrimBuffer__8VIRasteri(raster, word at S+64); a negative result from either makes it return -1.
// Otherwise it returns the id. Inferred: the words are a primitive-buffer handle and a material-palette handle shared with the new sprite.
// Unknown: their names.
extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned char* Sprite__7VIScenei(unsigned char* scene, int index);
int ShareMaterialPal__8VIRasteri(unsigned char* raster, int palette);
int SharePrimBuffer__8VIRasteri(unsigned char* raster, int buffer);
}

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

extern "C" int Copy__13VIFloraSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char* raster, unsigned char* sound, unsigned char* collide, int copyArgument) {
    (void)sound; (void)collide;
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = Sprite__7VIScenei(scene, id);
    copy_base(copy, self);
*reinterpret_cast<int*>(copy + 68) = *reinterpret_cast<int*>(self + 68);
    *reinterpret_cast<int*>(copy + 64) = *reinterpret_cast<int*>(self + 64);
    if (raster != 0) {
        if (ShareMaterialPal__8VIRasteri(raster, *reinterpret_cast<int*>(self + 68)) < 0) return -1;
        if (SharePrimBuffer__8VIRasteri(raster, *reinterpret_cast<int*>(self + 64)) < 0) return -1;
    }
    return id;
}
