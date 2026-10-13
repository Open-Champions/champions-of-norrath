// Copy__11VILODSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x010765e0, 388 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is
// negative. Otherwise it fetches the new sprite N with Sprite__7VIScenei(scene, id) and copies the 44 bytes S+8..S+51 (eleven floats)
// and the word at S+52 to N. The base-class entry Copy__8VISprite only returns -1.
// Also copies the words at S+64 and S+68 and the 40 bytes S+72..S+111 to N (the image copies them as five unaligned doublewords). Then, for i from 0 while i is less
// than the word at S+68 (reread after every call), it calls ShareSprite__7VIScenei(scene, word at N+76+8*i). It returns the id. The raster, sound
// device and collide arguments are not used.
// Inferred: the word at S+68 is a child count and the words at N+76+8*i are child sprite ids, which the copy now references a second time. Unknown: the other
// halves of each 8-byte child record and the names of S+64 and S+68.
extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned char* Sprite__7VIScenei(unsigned char* scene, int index);
void ShareSprite__7VIScenei(unsigned char* scene, int index);
}

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

extern "C" int Copy__11VILODSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char*, unsigned char*, unsigned char*, int copyArgument) {
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = Sprite__7VIScenei(scene, id);
    typedef long long Unaligned __attribute__((aligned(1)));
    *reinterpret_cast<int*>(copy + 64) = *reinterpret_cast<int*>(self + 64);
    *reinterpret_cast<int*>(copy + 68) = *reinterpret_cast<int*>(self + 68);
    copy_base(copy, self);
    reinterpret_cast<Unaligned*>(copy + 72)[0] = reinterpret_cast<const Unaligned*>(self + 72)[0];
    reinterpret_cast<Unaligned*>(copy + 72)[1] = reinterpret_cast<const Unaligned*>(self + 72)[1];
    reinterpret_cast<Unaligned*>(copy + 72)[2] = reinterpret_cast<const Unaligned*>(self + 72)[2];
    reinterpret_cast<Unaligned*>(copy + 72)[3] = reinterpret_cast<const Unaligned*>(self + 72)[3];
    reinterpret_cast<Unaligned*>(copy + 72)[4] = reinterpret_cast<const Unaligned*>(self + 72)[4];
    for (int i = 0; i < *reinterpret_cast<int*>(self + 68); ++i)
        ShareSprite__7VIScenei(scene, *reinterpret_cast<int*>(copy + 76 + 8 * i));
    return id;
}
