// Observed RenderDeferredList__7VISceneR8VICameraii at 0x01142e80, 692 bytes. Arguments: scene,
// camera, and two integers, called flagA (the third argument) and flagB (the fourth). The result is
// zero. Offsets are bytes. The display list is the vector at scene + 1112: its count is the word at
// +0 and its data pointer the word at +8; elements are 12 bytes: word 0 (a zone index, -1 for none),
// word 1 (a pointer to a matrix-and-lighting block, or -1) and word 2 (an index into the sprite pool
// at scene + 996, whose data pointer is at +8 and whose 12-byte entries start with a sprite pointer).
// The raster is the word at scene + 19356; the camera's matrix is at camera + 48.
//   A local 4x4 float matrix (VIMatrix44) is zero-filled first, and `last` = -1.
//   For each element i in order (the count is re-read after each element):
//     if word 0 is not -1 and not `last`: SetPretranslations__6VIZoneR8VIRasterR8VICamera(zone, raster,
//        camera) with zone = *(scene + 80) + 592 * word0.  `last` becomes word 0 either way.
//     if word 1 == -1: only when flagB != 0:
//        sceVu0CopyMatrix(m, camera + 48); m[12..14] = 0 and m[15] = 1.0f; SetModelView(raster, m);
//        a colour (0, 0, 0, 1.0f) is passed to SetStaticLighting(raster, colour); then the sprite is drawn.
//     otherwise: only when flagA != 0:
//        Mul__10VIMatrix44RC10VIMatrix44T1(m, block, camera + 48); SetModelView(raster, m);
//        scene + 1096 = -1 and scene + 1092 = the word at block + 108 are stored; then
//        SetStaticLighting(raster, block + 112); the sprite is drawn; then scene + 1092 and
//        scene + 1096 are set to -1.
//     Drawing the sprite = calling through the sprite's vtable (the word at sprite + 60, slot at +24: a
//        signed 16-bit adjustment then a function address) with (sprite + adjust, camera, scene, raster, 2).
//   Finally _$_10VIMatrix44(m, 2) runs on the local matrix.
// Inferred: the elements are deferred draws (by the function name). Unknown: the meaning of flagA and
// flagB, of the words at block + 108 and the lighting that follows, and of the stored scene words.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
extern "C" void norrath_host_call_draw(unsigned function, char* sprite, void* camera, void* scene, unsigned raster);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#define NORRATH_CALL_DRAW(function, sprite, camera, scene, raster) \
    norrath_host_call_draw((function), (sprite), (camera), (scene), (raster))
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#define NORRATH_CALL_DRAW(function, sprite, camera, scene, raster)                                  \
    reinterpret_cast<void (*)(void*, void*, void*, unsigned, int)>(static_cast<unsigned long>(function))( \
        (sprite), (camera), (scene), (raster), 2)
#endif

extern "C" {
struct VIScene;
struct VICamera;

int SetPretranslations__6VIZoneR8VIRasterR8VICamera(void* zone, unsigned raster, void* camera);
void sceVu0CopyMatrix(void* destination, const void* source);
void SetModelView__8VIRasterRC10VIMatrix44(unsigned raster, const void* matrix);
void SetStaticLighting__8VIRasterRC10VIColor32F(unsigned raster, const void* colour);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* a, const void* b);
void _$_10VIMatrix44(void* matrix, int flags);

static unsigned word_at(const char* address) { return *reinterpret_cast<const unsigned*>(address); }

static void draw_sprite(char* scene, void* camera, int index) {
    const char* sprites = NORRATH_TRANSLATE(word_at(scene + 996 + 8));
    char* sprite = NORRATH_TRANSLATE(word_at(sprites + 12 * index));
    const char* slot = NORRATH_TRANSLATE(word_at(sprite + 60)) + 24;
    short adjust = *reinterpret_cast<const short*>(slot);
    NORRATH_CALL_DRAW(word_at(slot + 4), sprite + adjust, camera, scene, word_at(scene + 19356));
}

int RenderDeferredList__7VISceneR8VICameraii(VIScene* scene_, VICamera* camera_, int flagA, int flagB) {
    char* scene = reinterpret_cast<char*>(scene_);
    char* camera = reinterpret_cast<char*>(camera_);
    alignas(16) float m[16];
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    int last = -1;
    const char* list = scene + 1112;
    if (*reinterpret_cast<const int*>(list) > 0) {
        int i = 0;
        do {
            int zone = *reinterpret_cast<const int*>(NORRATH_TRANSLATE(word_at(list + 8)) + 12 * i);
            if (zone != -1 && zone != last)
                SetPretranslations__6VIZoneR8VIRasterR8VICamera(NORRATH_TRANSLATE(word_at(scene + 80)) + 592 * zone,
                                                                word_at(scene + 19356), camera);
            last = zone;
            const char* element = NORRATH_TRANSLATE(word_at(list + 8)) + 12 * i;
            int block_word = *reinterpret_cast<const int*>(element + 4);
            int sprite_index = *reinterpret_cast<const int*>(element + 8);
            if (block_word == -1) {
                if (flagB != 0) {
                    sceVu0CopyMatrix(m, camera + 48);
                    m[12] = 0.0f;
                    m[13] = 0.0f;
                    m[14] = 0.0f;
                    m[15] = 1.0f;
                    SetModelView__8VIRasterRC10VIMatrix44(word_at(scene + 19356), m);
                    alignas(16) float colour[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                    SetStaticLighting__8VIRasterRC10VIColor32F(word_at(scene + 19356), colour);
                    draw_sprite(scene, camera, sprite_index);
                }
            } else if (flagA != 0) {
                const char* block = NORRATH_TRANSLATE(static_cast<unsigned>(block_word));
                Mul__10VIMatrix44RC10VIMatrix44T1(m, block, camera + 48);
                SetModelView__8VIRasterRC10VIMatrix44(word_at(scene + 19356), m);
                unsigned lighting_word = word_at(block + 108);
                *reinterpret_cast<int*>(scene + 1096) = -1;
                *reinterpret_cast<unsigned*>(scene + 1092) = lighting_word;
                SetStaticLighting__8VIRasterRC10VIColor32F(word_at(scene + 19356), block + 112);
                draw_sprite(scene, camera, sprite_index);
                *reinterpret_cast<int*>(scene + 1092) = -1;
                *reinterpret_cast<int*>(scene + 1096) = -1;
            }
            ++i;
        } while (i < *reinterpret_cast<const int*>(list));
    }
    _$_10VIMatrix44(m, 2);
    return 0;
}
}
