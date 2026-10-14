// Copy__12VINameSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x010a0b60, 328 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is
// negative. Otherwise it fetches the new sprite N with Sprite__7VIScenei(scene, id) and copies the 44 bytes S+8..S+51 (eleven floats)
// and the word at S+52 to N. The base-class entry Copy__8VISprite only returns -1.
// Also copies the word at S+64, calls strcpy8__8VIStringPcUiPCc(N + 68, 32, S + 68), copies the 16 bytes S+100..S+115 to N+100.., and, when the
// raster is nonnull, calls ShareFont__8VIRasteri(raster, word at S+64), returning -1 if that result is negative. It returns the id otherwise.
// Inferred: S+68 begins a 32-byte text buffer (the name) and S+64 is a font handle. Unknown: the semantics of strcpy8 and the 16 bytes' layout.
extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned char* Sprite__7VIScenei(unsigned char* scene, int index);
char* strcpy8__8VIStringPcUiPCc(char* destination, unsigned size, const char* source);
int ShareFont__8VIRasteri(unsigned char* raster, int font);
}

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

extern "C" int Copy__12VINameSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char* raster, unsigned char* sound, unsigned char* collide, int copyArgument) {
    (void)sound; (void)collide;
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = Sprite__7VIScenei(scene, id);
    copy_base(copy, self);
*reinterpret_cast<int*>(copy + 64) = *reinterpret_cast<int*>(self + 64);
    strcpy8__8VIStringPcUiPCc(reinterpret_cast<char*>(copy + 68), 32, reinterpret_cast<const char*>(self + 68));
    typedef long long Unaligned __attribute__((aligned(1)));
    reinterpret_cast<Unaligned*>(copy + 100)[0] = reinterpret_cast<const Unaligned*>(self + 100)[0];
    reinterpret_cast<Unaligned*>(copy + 100)[1] = reinterpret_cast<const Unaligned*>(self + 100)[1];
    if (raster != 0 && ShareFont__8VIRasteri(raster, *reinterpret_cast<int*>(self + 64)) < 0) return -1;
    return id;
}
