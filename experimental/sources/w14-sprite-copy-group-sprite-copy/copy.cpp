// Copy__13VIGroupSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x0101d830, 644 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is negative. Otherwise it fetches the new
// sprite N with Sprite__7VIScenei(scene, id), copies the 44 bytes S+8..S+51 (eleven floats), the word at S+52 and the word at S+64, and the 160 bytes S+68..S+227 to N
// (the image copies them as twenty doublewords, with an alignment test choosing aligned or unaligned accesses). Then for k from 0 while k is less than the word at S+64
// (reread each time) it takes child = Sprite__7VIScenei(scene, word at S+68+32*k), reads the word at child+60 (a table), and calls the function whose address is the word at
// table+20 with (child + the signed halfword at table+16, scene). If that call returns exactly 1 it calls CopySprite__7VISceneii(scene, word at S+68+32*k, i), stores the
// result at N+68+32*k, and returns -1 if the result is -1. After the loop it calls ShareSprite__7VIScenei(scene, word at N+68+32*k) for k below the word at S+64 (reread each
// time). It returns the id. The raster, sound device and collide arguments are not used.
// Inferred: the sprite is a group of up to five children in 32-byte records whose first word is the child sprite id, and the call through the table slot asks the child
// whether it must be duplicated (result 1) rather than shared. Unknown: the name of that virtual function, the other 28 bytes of each child record, and the names of S+64
// and the table layout beyond this slot.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
int CopySprite__7VISceneii(unsigned char* scene, int index, int copyArgument);
void ShareSprite__7VIScenei(unsigned char* scene, int index);

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

int Copy__13VIGroupSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char*, unsigned char*, unsigned char*, int copyArgument) {
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, id));
    typedef long long Unaligned __attribute__((aligned(1)));
    copy_base(copy, self);
    *reinterpret_cast<int*>(copy + 64) = *reinterpret_cast<int*>(self + 64);
    for (int k = 0; k < 20; ++k)
        reinterpret_cast<Unaligned*>(copy + 68)[k] = reinterpret_cast<const Unaligned*>(self + 68)[k];
    for (int k = 0; k < *reinterpret_cast<int*>(self + 64); ++k) {
        int childId = *reinterpret_cast<int*>(self + 68 + 32 * k);
        unsigned char* child = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, childId));
        unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(child + 60)) + 16;
        typedef int (*Slot)(unsigned char*, unsigned char*);
        Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
        if (function(child + *reinterpret_cast<short*>(entry), scene) == 1) {
            int duplicate = CopySprite__7VISceneii(scene, *reinterpret_cast<int*>(self + 68 + 32 * k), copyArgument);
            *reinterpret_cast<int*>(copy + 68 + 32 * k) = duplicate;
            if (duplicate == -1) return -1;
        }
    }
    for (int k = 0; k < *reinterpret_cast<int*>(self + 64); ++k)
        ShareSprite__7VIScenei(scene, *reinterpret_cast<int*>(copy + 68 + 32 * k));
    return id;
}
}
