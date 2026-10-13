// Observed VITrailFxClear__FR8VIRasterR12VIDictionary at 0x01026fd8, 180 bytes. The result is zero.
// File-local words of the trail effect (the image's local symbols, published here as
// `VITrailFx$<name>` the way the splash effect words are): the pool word sTrailFxPool at
// 0x011e5214 and the material handle sMaterialHandle at 0x0118e7d8.
//   1. If the pool word is nonzero it points just past an eight-byte header whose first word
//      is an element count; the elements are 56 bytes each. Elements are destroyed from the
//      last to the first with _$_9VITrailFx(element, 2), the pool word being re-read to
//      decide whether to continue. Then __builtin_vec_delete(pool - 8) is called with the pool
//      word re-read. The pool word itself is not cleared.
//   2. If the material handle is not -1: ReleaseMaterial__8VIRasteriR12VIDictionary(raster,
//      handle, dictionary) and the handle is set to -1.
// Unknown: the layout of a 56-byte trail element.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIRaster;
struct VIDictionary;

extern unsigned norrath_trail_pool __asm__("VITrailFx$sTrailFxPool");
extern int norrath_trail_material __asm__("VITrailFx$sMaterialHandle");

void _$_9VITrailFx(void* element, int flags);
void __builtin_vec_delete(void* block);
int ReleaseMaterial__8VIRasteriR12VIDictionary(VIRaster* raster, int handle, VIDictionary* dictionary);

int VITrailFxClear__FR8VIRasterR12VIDictionary(VIRaster* raster, VIDictionary* dictionary) {
    unsigned pool = norrath_trail_pool;
    if (pool) {
        unsigned end = pool + 56u * *reinterpret_cast<unsigned*>(NORRATH_TRANSLATE(pool - 8));
        if (pool != end) {
            do {
                end -= 56;
                _$_9VITrailFx(NORRATH_TRANSLATE(end), 2);
            } while (norrath_trail_pool != end);
        }
        __builtin_vec_delete(NORRATH_TRANSLATE(norrath_trail_pool - 8));
    }
    if (norrath_trail_material != -1) {
        ReleaseMaterial__8VIRasteriR12VIDictionary(raster, norrath_trail_material, dictionary);
        norrath_trail_material = -1;
    }
    return 0;
}
}
