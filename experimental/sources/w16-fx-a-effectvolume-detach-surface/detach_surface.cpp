// Observed: DetachSurface__14VIEffectVolumei at 0x01147180, 192 bytes. With (this, i) let raster be the word at scene+19356 and dictionary the
// word at scene+19352, scene being the pointer in _14VIEffectVolume$mScene (read again after each call). The material number is the word at
// this+76+4*i. When it is not -1 it calls Material__8VIRasteri(raster, number), then
// SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, -1, raster, dictionary), then
// ReleaseMaterial__8VIRasteriR12VIDictionary(raster, number, dictionary), and stores -1 at this+76+4*i. In every case it stores -1 at
// this+64+4*i and returns 0.
// Inferred: this+64+4*i is a surface id slot and this+76+4*i the material made for it by AttachSurface__14VIEffectVolumeii; this
// undoes that: the material's layer-0 texture is cleared, the material released, and both slots reset.
// Unknown: the slot count (the index is not range checked) and what the dictionary does on release.
extern "C" {
extern unsigned char* _14VIEffectVolume$mScene;
void* Material__8VIRasteri(void* raster, int index);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* material, int layer, int surface, void* raster, void* dictionary);
void ReleaseMaterial__8VIRasteriR12VIDictionary(void* raster, int index, void* dictionary);

#ifdef NORRATH_HOST_ARENA
void* norrath_host_pointer(unsigned address);
#define POINTER_OF(word) norrath_host_pointer(word)
#else
#define POINTER_OF(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int DetachSurface__14VIEffectVolumei(unsigned char* self, int index) {
    void* raster = POINTER_OF(*reinterpret_cast<unsigned*>(_14VIEffectVolume$mScene + 19356));
    int* slot = reinterpret_cast<int*>(self + 64 + 4 * index);
    if (slot[3] != -1) {
        void* material = Material__8VIRasteri(raster, slot[3]);
        SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, -1, raster,
                                                                 POINTER_OF(*reinterpret_cast<unsigned*>(_14VIEffectVolume$mScene + 19352)));
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, slot[3],
                                                   POINTER_OF(*reinterpret_cast<unsigned*>(_14VIEffectVolume$mScene + 19352)));
        slot[3] = -1;
    }
    *slot = -1;
    return 0;
}
}
