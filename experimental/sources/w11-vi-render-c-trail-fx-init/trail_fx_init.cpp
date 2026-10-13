// Observed VITrailFxInit__FR8VIRasterR12VIDictionary at 0x01026988, 372 bytes. The dictionary
// argument is not used. The file-local words are the image's local symbols, published as
// `VITrailFx$<name>`: sMaterialHandle (0x0118e7d8), the lists sFreeTrailFx (0x011e51e8),
// sActiveTrailFx (0x011e51f8) and sInactiveTrailFx (0x011e5208), and the pool word sTrailFxPool
// (0x011e5214).
//   1. h = CreateMaterial__8VIRasteri(raster, 0); sMaterialHandle = h (stored even when h is -1).
//      If h is -1 the result is -1 and nothing else happens.
//   2. m = Material__8VIRasteri(raster, h), then on m: Init__10VIMateriali(m, 1),
//      SetLayerFillType__10VIMateriali18VIMaterialFillType(m, 0, 0), SetLayerModulate__10VIMaterialii(m, 0, 1),
//      SetDither__10VIMateriali(m, 0), SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(m, 0, 1),
//      SetZWrite__10VIMateriali(m, 0), SetZTest__10VIMateriali(m, 1).
//   3. Init__7VILList on the free, active and inactive lists, in that order.
//   4. block = __builtin_vec_new(904). If nonzero: the word 16 is stored at block, __9VITrailFx runs on
//      the sixteen 56-byte elements from block + 8 in order, and the pool word becomes block + 8;
//      otherwise the pool word becomes zero.
//   5. For i = 0 .. 15: AddTail__7VILListP7VILNode(free list, pool word + 56 * i) with the pool word
//      re-read each time (no null check).
//   The result is zero. The meaning of the material settings is not established here.
extern "C" {
struct VIRaster;
struct VIDictionary;

extern int norrath_trail_material __asm__("VITrailFx$sMaterialHandle");
extern unsigned norrath_trail_free[4] __asm__("VITrailFx$sFreeTrailFx");
extern unsigned norrath_trail_active[4] __asm__("VITrailFx$sActiveTrailFx");
extern unsigned norrath_trail_inactive[4] __asm__("VITrailFx$sInactiveTrailFx");
extern unsigned norrath_trail_pool __asm__("VITrailFx$sTrailFxPool");

int CreateMaterial__8VIRasteri(VIRaster* raster, int arg);
void* Material__8VIRasteri(VIRaster* raster, int handle);
void Init__10VIMateriali(void* material, int arg);
void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* material, int layer, int type);
void SetLayerModulate__10VIMaterialii(void* material, int layer, int on);
void SetDither__10VIMateriali(void* material, int on);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode);
void SetZWrite__10VIMateriali(void* material, int on);
void SetZTest__10VIMateriali(void* material, int on);
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);
void* __builtin_vec_new(unsigned size);
void __9VITrailFx(void* element);

int VITrailFxInit__FR8VIRasterR12VIDictionary(VIRaster* raster, VIDictionary* dictionary) {
    (void)dictionary;
    int handle = CreateMaterial__8VIRasteri(raster, 0);
    norrath_trail_material = handle;
    if (handle == -1) return -1;
    void* material = Material__8VIRasteri(raster, handle);
    Init__10VIMateriali(material, 1);
    SetLayerFillType__10VIMateriali18VIMaterialFillType(material, 0, 0);
    SetLayerModulate__10VIMaterialii(material, 0, 1);
    SetDither__10VIMateriali(material, 0);
    SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 1);
    SetZWrite__10VIMateriali(material, 0);
    SetZTest__10VIMateriali(material, 1);
    Init__7VILList(norrath_trail_free);
    Init__7VILList(norrath_trail_active);
    Init__7VILList(norrath_trail_inactive);
    char* block = static_cast<char*>(__builtin_vec_new(904));
    unsigned pool = 0;
    if (block) {
        *reinterpret_cast<int*>(block) = 16;
        for (int i = 0; i < 16; ++i) __9VITrailFx(block + 8 + 56 * i);
        pool = static_cast<unsigned>(reinterpret_cast<unsigned long>(block + 8));
    }
    norrath_trail_pool = pool;
    for (int i = 0; i < 16; ++i)
        AddTail__7VILListP7VILNode(norrath_trail_free,
                                   reinterpret_cast<void*>(static_cast<unsigned long>(norrath_trail_pool + 56u * i)));
    return 0;
}
}
