// Observed: AttachSurface__14VIEffectVolumeii at 0x01146fd0 is 428 bytes. Arguments: the object (V), a slot index i and a surface id s.
// Let scene be the pointer at _14VIEffectVolume$mScene, raster the word at scene+19356 and dictionary the word at scene+19352.
//  - If s is -1 it returns -1 at once.
//  - Otherwise slot = V + 64 + 4*i. When the word at slot is not -1 it calls DetachSurface__14VIEffectVolumei(V, i) first. It stores s in
//    the slot, calls CreateMaterial__8VIRasteri(raster, 0) and stores the result in the word at slot+12 (V + 76 + 4*i). When that result is
//    negative it stores -1 in the slot and returns -1.
//  - Otherwise it takes m = Material__8VIRasteri(raster, result) and calls, in order: Init__10VIMateriali(m, 1);
//    SetLayerFillType__10VIMateriali18VIMaterialFillType(m, 0, 0); SetLayerModulate__10VIMaterialii(m, 0, 1);
//    when the word at V+36 is below 5, SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(m, 0, mode) with mode 0 for type 0, 1 for type
//    2 and 3 for types 1, 3 and 4 (no call for 5 or more); SetZWrite__10VIMateriali(m, 0); SetZTest__10VIMateriali(m, 1);
//    SetDither__10VIMateriali(m, 0); and SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(m, 0, the word now in the slot, raster,
//    dictionary). It returns 0.
// Inferred: the slots at V+64 hold surface ids and those at V+76 the matching material ids; the type at V+36 picks the blend mode
// (the same type numbering Process__14VIEffectVolumef dispatches on). Mode values are the integers passed, not names.
// Unknown: the number of slots (the index is not range checked), the names of the blend modes and what Material's return is beyond a
// material pointer.
extern "C" {
extern unsigned char* _14VIEffectVolume$mScene;
void DetachSurface__14VIEffectVolumei(void* self, int index);
int CreateMaterial__8VIRasteri(void* raster, int flags);
void* Material__8VIRasteri(void* raster, int index);
void Init__10VIMateriali(void* material, int value);
void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* material, int layer, int type);
void SetLayerModulate__10VIMaterialii(void* material, int layer, int value);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode);
void SetZWrite__10VIMateriali(void* material, int value);
void SetZTest__10VIMateriali(void* material, int value);
void SetDither__10VIMateriali(void* material, int value);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* material, int layer, int surface, void* raster, void* dictionary);

#ifdef NORRATH_HOST_ARENA
void* norrath_host_pointer(unsigned address);
#define POINTER_OF(word) norrath_host_pointer(word)
#else
#define POINTER_OF(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int AttachSurface__14VIEffectVolumeii(unsigned char* self, int index, int surface) {
    void* raster = POINTER_OF(*reinterpret_cast<unsigned*>(_14VIEffectVolume$mScene + 19356));
    if (surface == -1) return -1;
    int* slot = reinterpret_cast<int*>(self + 64 + 4 * index);
    if (*slot != -1) DetachSurface__14VIEffectVolumei(self, index);
    *slot = surface;
    int created = CreateMaterial__8VIRasteri(raster, 0);
    slot[3] = created;
    if (created < 0) {
        *slot = -1;
        return -1;
    }
    void* material = Material__8VIRasteri(raster, created);
    Init__10VIMateriali(material, 1);
    SetLayerFillType__10VIMateriali18VIMaterialFillType(material, 0, 0);
    SetLayerModulate__10VIMaterialii(material, 0, 1);
    switch (*reinterpret_cast<unsigned*>(self + 36)) {
    case 0: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 0); break;
    case 2: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 1); break;
    case 1:
    case 3:
    case 4: SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 3); break;
    default: break;
    }
    SetZWrite__10VIMateriali(material, 0);
    SetZTest__10VIMateriali(material, 1);
    SetDither__10VIMateriali(material, 0);
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, *slot, raster, POINTER_OF(*reinterpret_cast<unsigned*>(_14VIEffectVolume$mScene + 19352)));
    return 0;
}
}
