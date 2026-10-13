// Observed: Clear__8VIRaster at 0x0110a5c8 is 872 bytes. In order it:
//   1. stores -1 at offsets 17852 and 17828, 17832, 17836, 17840, 17844, 17848, and zero at 17856;
//   2. calls ClearDMADoubleBuffer__8VIRaster(raster) and then ClearMaterialLRU__8VIRaster(raster);
//   3. empties nine containers of the raster in this order. A "pool walk" runs Begin, then
//      while the position differs from End takes the element pointer in the first word of the
//      12-byte entry at index position in the array whose pointer is at pool+8, runs the element's
//      destructor with the given flag when that pointer is nonnull, and moves to Next(position).
//      A "list walk" is the same except that the position is itself the address of the pointer
//      slot. After each walk the container's Clear member is called. Containers (offset, kind,
//      element type, destructor flag):
//        19356 pool VIColorBuffer 3;  19320 pool VIPrimBuffer 3;  19344 list VIPrimBuffer 3;
//        19380 pool VISurface 3;  19404 list VISurface 3;
//      then Clear__t6VIList1Z10VIMaterial(raster+19416) with no walk;
//        19428 pool VIMaterialPal: for a nonnull element the VIArray<int> at element+8 is
//        destroyed with flag 2 and the element is freed with __builtin_delete;
//        19508 pool VIRFont 3;
//   4. calls Clear__13VIEESurfCache(&SurfCache) last and returns -1 if that is negative, else zero.
// Any Clear step or either of the first two calls returning a negative value makes the function
// return -1 at once, without the later steps.
// Inferred: this is the raster's full teardown of its cached objects; 17828 to 17852 hold "none"
// handles (-1) afterwards. The pool and list element sizes are 12 bytes.
// Unknown: the names of the words at 17828 to 17856.
extern "C" {
struct VIRaster;

extern unsigned char norrath_surf_cache[] __asm__("SurfCache");
int ClearDMADoubleBuffer__8VIRaster(VIRaster* raster);
int ClearMaterialLRU__8VIRaster(VIRaster* raster);
void __builtin_delete(void* pointer);
int Clear__13VIEESurfCache(void* cache);

int Begin__Ct6VIPool1ZP13VIColorBuffer(void* pool);
int End__Ct6VIPool1ZP13VIColorBuffer(void* pool);
int Next__Ct6VIPool1ZP13VIColorBufferi(void* pool, int position);
int Clear__t6VIPool1ZP13VIColorBuffer(void* pool);
void _$_13VIColorBuffer(void* element, int flags);

int Begin__Ct6VIPool1ZP12VIPrimBuffer(void* pool);
int End__Ct6VIPool1ZP12VIPrimBuffer(void* pool);
int Next__Ct6VIPool1ZP12VIPrimBufferi(void* pool, int position);
int Clear__t6VIPool1ZP12VIPrimBuffer(void* pool);
int Begin__Ct6VIList1ZP12VIPrimBuffer(void* list);
int End__Ct6VIList1ZP12VIPrimBuffer(void* list);
int Next__Ct6VIList1ZP12VIPrimBufferi(void* list, int position);
int Clear__t6VIList1ZP12VIPrimBuffer(void* list);
void _$_12VIPrimBuffer(void* element, int flags);

int Begin__Ct6VIPool1ZP9VISurface(void* pool);
int End__Ct6VIPool1ZP9VISurface(void* pool);
int Next__Ct6VIPool1ZP9VISurfacei(void* pool, int position);
int Clear__t6VIPool1ZP9VISurface(void* pool);
int Begin__Ct6VIList1ZP9VISurface(void* list);
int End__Ct6VIList1ZP9VISurface(void* list);
int Next__Ct6VIList1ZP9VISurfacei(void* list, int position);
int Clear__t6VIList1ZP9VISurface(void* list);
void _$_9VISurface(void* element, int flags);

int Clear__t6VIList1Z10VIMaterial(void* list);

int Begin__Ct6VIPool1ZP13VIMaterialPal(void* pool);
int End__Ct6VIPool1ZP13VIMaterialPal(void* pool);
int Next__Ct6VIPool1ZP13VIMaterialPali(void* pool, int position);
int Clear__t6VIPool1ZP13VIMaterialPal(void* pool);
void _$_t7VIArray1Zi(void* array, int flags);

int Begin__Ct6VIPool1ZP7VIRFont(void* pool);
int End__Ct6VIPool1ZP7VIRFont(void* pool);
int Next__Ct6VIPool1ZP7VIRFonti(void* pool, int position);
int Clear__t6VIPool1ZP7VIRFont(void* pool);
void _$_7VIRFont(void* element, int flags);

static void* pool_element(void* pool, int position) {
    return *reinterpret_cast<void**>(*reinterpret_cast<char**>(static_cast<char*>(pool) + 8) + position * 12);
}

#define POOL_WALK(pool, SUFFIX, DESTROY)                                              \
    for (int position = Begin__Ct6VIPool1##SUFFIX(pool); position != End__Ct6VIPool1##SUFFIX(pool); \
         position = Next__Ct6VIPool1##SUFFIX##i(pool, position)) {                    \
        void* element = pool_element(pool, position);                                  \
        if (element != 0) DESTROY;                                                     \
    }

#define LIST_WALK(list, SUFFIX, DESTROY)                                              \
    for (int position = Begin__Ct6VIList1##SUFFIX(list); position != End__Ct6VIList1##SUFFIX(list); \
         position = Next__Ct6VIList1##SUFFIX##i(list, position)) {                    \
        void* element = *reinterpret_cast<void**>(position);                           \
        if (element != 0) DESTROY;                                                     \
    }

int Clear__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    int* words = reinterpret_cast<int*>(base);
    words[17852 / 4] = -1;
    words[17828 / 4] = -1;
    words[17832 / 4] = -1;
    words[17836 / 4] = -1;
    words[17840 / 4] = -1;
    words[17844 / 4] = -1;
    words[17848 / 4] = -1;
    words[17856 / 4] = 0;
    if (ClearDMADoubleBuffer__8VIRaster(raster) < 0) return -1;
    if (ClearMaterialLRU__8VIRaster(raster) < 0) return -1;

    void* colorBuffers = base + 19356;
    POOL_WALK(colorBuffers, ZP13VIColorBuffer, _$_13VIColorBuffer(element, 3))
    if (Clear__t6VIPool1ZP13VIColorBuffer(colorBuffers) < 0) return -1;

    void* primPool = base + 19320;
    POOL_WALK(primPool, ZP12VIPrimBuffer, _$_12VIPrimBuffer(element, 3))
    if (Clear__t6VIPool1ZP12VIPrimBuffer(primPool) < 0) return -1;

    void* primList = base + 19344;
    LIST_WALK(primList, ZP12VIPrimBuffer, _$_12VIPrimBuffer(element, 3))
    if (Clear__t6VIList1ZP12VIPrimBuffer(primList) < 0) return -1;

    void* surfacePool = base + 19380;
    POOL_WALK(surfacePool, ZP9VISurface, _$_9VISurface(element, 3))
    if (Clear__t6VIPool1ZP9VISurface(surfacePool) < 0) return -1;

    void* surfaceList = base + 19404;
    LIST_WALK(surfaceList, ZP9VISurface, _$_9VISurface(element, 3))
    if (Clear__t6VIList1ZP9VISurface(surfaceList) < 0) return -1;

    if (Clear__t6VIList1Z10VIMaterial(base + 19416) < 0) return -1;

    void* palettes = base + 19428;
    POOL_WALK(palettes, ZP13VIMaterialPal,
              (_$_t7VIArray1Zi(static_cast<char*>(element) + 8, 2), __builtin_delete(element)))
    if (Clear__t6VIPool1ZP13VIMaterialPal(palettes) < 0) return -1;

    void* fonts = base + 19508;
    POOL_WALK(fonts, ZP7VIRFont, _$_7VIRFont(element, 3))
    if (Clear__t6VIPool1ZP7VIRFont(fonts) < 0) return -1;

    return Clear__13VIEESurfCache(norrath_surf_cache) < 0 ? -1 : 0;
}
}
