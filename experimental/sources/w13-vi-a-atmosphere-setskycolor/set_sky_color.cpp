// Observed: SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32 at 0x01067bc8 is 352 bytes. Arguments:
// atmosphere (a0), scene (a1) and a pointer to a 4-byte colour (a2).
//   - If the int at atmosphere+1228 is zero it returns -1.
//   - Otherwise: raster = the word at scene+19356; sprite = Sprite__7VIScenei(scene, the int at
//     atmosphere+1248); pal = GetMaterialPal__C14VISimpleSprite(sprite); count =
//     GetMaterialPalSize__8VIRasteri(raster, pal). With count <= 0 it returns 0.
//   - For each j in 0..count it gets material = GetMaterialPalEntry__8VIRasterii(raster, pal, j),
//     and for each layer i in 0..(int at material+8) (re-read after every layer): local = the 4
//     colour bytes; GetLayerColor__C10VIMateriali(&tmp, material, i) (the colour is returned
//     through a hidden pointer); local[3] = tmp[3]; then SetLayerColor__10VIMaterialiRC9VIColor32(
//     material, i, &local). A negative SetLayerColor result returns -1 at once.
//   - After all materials it returns 0.
// Inferred: tints every layer of every material of the sky sprite with the given colour but
// keeps the layer's own alpha byte (byte 3); the local colour starts as (0, 0, 0, 255).
// Unknown: the byte order of VIColor32 beyond "byte 3 is kept", the meaning of
// atmosphere+1228 and atmosphere+1248 (the latter looks like a sprite index), and the scene
// layout around +19356 (the word is the raster).
extern "C" {
void* Sprite__7VIScenei(void* scene, int index);
int GetMaterialPal__C14VISimpleSprite(const void* sprite);
int GetMaterialPalSize__8VIRasteri(void* raster, int pal);
unsigned char* GetMaterialPalEntry__8VIRasterii(void* raster, int pal, int index);
void GetLayerColor__C10VIMateriali(unsigned char* result, const void* material, int layer);
int SetLayerColor__10VIMaterialiRC9VIColor32(void* material, int layer, const unsigned char* color);

// The word at scene+19356 is a 32-bit target address; a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(unsigned char* atmosphere, unsigned char* scene, const unsigned char* color) {
    if (*reinterpret_cast<int*>(atmosphere + 1228) == 0) {
        return -1;
    }
    unsigned char local[4] = {0, 0, 0, 0xff};
    void* raster = NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(scene + 19356));
    int pal = GetMaterialPal__C14VISimpleSprite(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(atmosphere + 1248)));
    int count = GetMaterialPalSize__8VIRasteri(raster, pal);
    for (int j = 0; j < count; ++j) {
        unsigned char* material = GetMaterialPalEntry__8VIRasterii(raster, pal, j);
        for (int i = 0; i < *reinterpret_cast<int*>(material + 8); ) {
            unsigned char tmp[4];
            local[0] = color[0];
            local[1] = color[1];
            local[2] = color[2];
            local[3] = color[3];
            GetLayerColor__C10VIMateriali(tmp, material, i);
            local[3] = tmp[3];
            if (SetLayerColor__10VIMaterialiRC9VIColor32(material, i++, local) < 0) {
                return -1;
            }
        }
    }
    return 0;
}
}
