// Observed msg_draw__5Torch at 0x00230030, 372 bytes.
//  1. flicker = (int)(ef1Noise(0x7efe41 + (int)object * 0x19660d + frameNm * 16) * 22.0f), with the
//     argument computed in wrapping 32-bit arithmetic.
//  2. p = {object+16, object+20, object+24 - 6.0f}; when the unsigned byte at +138 is 3 or 4,
//     p.z += 12.0f.
//  3. The byte at +137 is set to 0 on every call. When the signed byte at +136 is zero,
//     objectAddToRunList(object) is called and +136 becomes 1.
//  4. The byte at +138 is read again. When it is 2 or 3, P_FaceSprite is called with the unmodified
//     position copied from object+16..27 (by address), 26.0f, (float)(flicker * 64 + 20000), the
//     ints 124, 81, 62, 125, haloTexture2, short 0 and int 0. Otherwise P_FaceSprite is called with
//     the address of p, 25.0f, (float)(flicker * 64 + 13996), the ints 250, 150, 0, 40, haloTexture2,
//     short 0 and int 2.
// Unknown: what the sprite constants and the +136/+137/+138 bytes mean. The image calls the first
// P_FaceSprite from the "2 or 3" branch with the raw position, as here. Nothing here ran on the EE.
extern "C" {
struct Point3 { float x, y, z; };
float ef1Noise__Fi(int seed);
void objectAddToRunList__FP10GameObject(void* object);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* position, float a, float b, int c, int d, int e, int f,
                                               void* texture, short g, int h);
extern int frameNm;
extern void* haloTexture2;
}

typedef float __attribute__((may_alias)) Single;

extern "C" void msg_draw__5Torch(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const int seed = 0x7efe41 + static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self)) * 0x19660du) + frameNm * 16;
    const int flicker = static_cast<int>(ef1Noise__Fi(seed) * 22.0f);
    Point3 p;
    p.x = *reinterpret_cast<Single*>(object + 16);
    p.y = *reinterpret_cast<Single*>(object + 20);
    p.z = *reinterpret_cast<Single*>(object + 24) - 6.0f;
    if (static_cast<unsigned>(object[138]) - 3u < 2u) p.z += 12.0f;
    object[137] = 0;
    if (static_cast<signed char>(object[136]) == 0) {
        objectAddToRunList__FP10GameObject(self);
        object[136] = 1;
    }
    if (static_cast<unsigned>(object[138]) - 2u < 2u) {
        Point3 at;
        at.x = *reinterpret_cast<Single*>(object + 16);
        at.y = *reinterpret_cast<Single*>(object + 20);
        at.z = *reinterpret_cast<Single*>(object + 24);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&at, 26.0f, static_cast<float>(flicker * 64 + 20000), 124, 81, 62, 125, haloTexture2, 0, 0);
    } else {
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&p, 25.0f, static_cast<float>(flicker * 64 + 13996), 250, 150, 0, 40, haloTexture2, 0, 2);
    }
}
