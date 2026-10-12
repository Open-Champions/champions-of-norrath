// Observed cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture at 0x001959a0,
// 536 bytes (a local symbol), in the pinned SLUS_205.65. Arguments: a name, a cache element
// (88 bytes; the elements are the table walked by preCacheArmor__6Player and
// playerDeallocateArmorCaches__Fv) and two output words (model, texture).
// Fields of the element as used here (offsets in bytes): +0 buffer word, +8 halfword
// (set to 0 when a load starts, 1 when its read is issued), +10 halfword (nonzero when
// the buffer holds a loaded entry), +12 the name of the loaded entry (a string, up to
// 64 bytes), +76 word (set when the read is issued), +80 word (a delay counter), +84 and
// +86 signed bytes (flags).
// Steps, in order:
//  1. If +10 is nonzero and strcmp(name, element+12) is 0: *texture = +0, *model = +0 plus
//     +76; return. Otherwise both outputs are set to 0 and the steps continue.
//  2. If the word cacheLoadCache is 0 (no load in flight): when +80 is nonzero it is
//     decremented and the call returns. Otherwise sprintf(text, "%s", name) and
//     eqFind__FPci(text, +84 as a signed byte) decide: a nonzero result stores +10 in
//     cacheLoadValid, drawFrameNm in cacheLoadDelayTime, copies the name into
//     cacheLoadName, makes this element the in-flight one (cacheLoadCache = element) and
//     clears the halfwords +8 and +10; a zero result sets +80 to 600.
//  3. Otherwise (a load is in flight, taken as the element at cacheLoadCache), nothing
//     happens until drawFrameNm - cacheLoadDelayTime is at least 4. Then with the
//     in-flight halfword +8 clear: return while cdIsBusy__Fv() is nonzero; free the old
//     texture (textureFreeTexture__FP8_texture(+0)) when cacheLoadValid is nonzero; set +8
//     to 1; look up cacheLoadName with eqFind__FPci(.., +84) and issue
//     cdLoadAsync__FPcPvT0ii(file, +0, &cacheCdDone, size + (half10 << 11), word4 << 11)
//     with size = (word12 << 11) - (word4 << 11) (found entry's words at +4 and +12 and
//     halfword at +10), storing size at +76. The file is "wcache.dat" when +84 is nonzero,
//     else "eqcache2.dat" when +86 is nonzero, else "eqcache.dat".
//     With +8 already set: return while cacheCdDone is 0; otherwise call
//     textureInitTexture__FP8_texture(+0), modelInitModel__FPv(+0 plus +76), copy
//     cacheLoadName into element+12, set +10 to 1 and clear cacheLoadCache.
// Pointers kept in memory are 32-bit words on the target.
// Inferred: element+0 is a heap block address and the file names are the three listed;
// the owner of eqFind, the file layout, and the meaning of the flags are unknown.
// Not executed on hardware; callees and the four globals are defined elsewhere.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#define NORRATH_WORD(p) static_cast<unsigned>(reinterpret_cast<unsigned char*>(p) - norrath_host_base)
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#define NORRATH_WORD(p) static_cast<unsigned>(reinterpret_cast<unsigned long>(p))
#endif
extern "C" {
struct ArmorCache {
    unsigned buffer;
    unsigned word4;
    short loaded;
    short valid;
    char name[64];
    unsigned offset;
    int delay;
    signed char flag84;
    signed char pad85;
    signed char flag86;
    signed char pad87;
};
struct CacheFileEntry {
    unsigned word0;
    unsigned word4;
    short half8;
    short half10;
    unsigned word12;
};
extern int drawFrameNm;
extern unsigned cacheLoadCache;
extern int cacheLoadValid;
extern int cacheLoadDelayTime;
extern signed char cacheCdDone;
extern char cacheLoadName[];
int strcmp(const char* a, const char* b);
char* strcpy(char* dest, const char* src);
int sprintf(char* out, const char* format, ...);
CacheFileEntry* eqFind__FPci(char* name, int flag);
int cdIsBusy__Fv();
void textureFreeTexture__FP8_texture(unsigned texture);
void textureInitTexture__FP8_texture(unsigned texture);
void modelInitModel__FPv(unsigned model);
void cdLoadAsync__FPcPvT0ii(const char* file, unsigned buffer, signed char* done, int length, int start);

void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* name, ArmorCache* element, unsigned* model,
                                                                 unsigned* texture) {
    if (element->valid != 0) {
        if (strcmp(name, element->name) == 0) {
            *texture = element->buffer;
            *model = element->buffer + element->offset;
            return;
        }
    }
    *model = 0;
    *texture = 0;
    if (cacheLoadCache == 0) {
        if (element->delay != 0) {
            element->delay = element->delay - 1;
            return;
        }
        char text[64];
        sprintf(text, "%s", name);
        if (eqFind__FPci(text, element->flag84) != 0) {
            cacheLoadValid = element->valid;
            cacheLoadDelayTime = drawFrameNm;
            strcpy(cacheLoadName, name);
            cacheLoadCache = NORRATH_WORD(element);
            element->loaded = 0;
            element->valid = 0;
        } else {
            element->delay = 600;
        }
        return;
    }
    ArmorCache* loading = reinterpret_cast<ArmorCache*>(NORRATH_PTR(cacheLoadCache));
    if (drawFrameNm - cacheLoadDelayTime < 4)
        return;
    if (loading->loaded == 0) {
        if (cdIsBusy__Fv() != 0)
            return;
        if (cacheLoadValid != 0)
            textureFreeTexture__FP8_texture(loading->buffer);
        loading->loaded = 1;
        CacheFileEntry* found = eqFind__FPci(cacheLoadName, loading->flag84);
        const unsigned start = found->word4 << 11;
        const unsigned size = (found->word12 << 11) - start;
        const int length = static_cast<int>(size + (static_cast<int>(found->half10) << 11));
        if (loading->flag84 != 0) {
            cdLoadAsync__FPcPvT0ii("wcache.dat", loading->buffer, &cacheCdDone, length, static_cast<int>(start));
        } else if (loading->flag86 != 0) {
            cdLoadAsync__FPcPvT0ii("eqcache2.dat", loading->buffer, &cacheCdDone, length, static_cast<int>(start));
        } else {
            cdLoadAsync__FPcPvT0ii("eqcache.dat", loading->buffer, &cacheCdDone, length, static_cast<int>(start));
        }
        loading->offset = size;
        return;
    }
    if (cacheCdDone == 0)
        return;
    textureInitTexture__FP8_texture(loading->buffer);
    modelInitModel__FPv(loading->buffer + loading->offset);
    strcpy(loading->name, cacheLoadName);
    loading->valid = 1;
    cacheLoadCache = 0;
}
}
