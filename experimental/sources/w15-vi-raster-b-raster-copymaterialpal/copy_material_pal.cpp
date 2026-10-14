// Observed: CopyMaterialPal__8VIRasterii at 0x0110e1d0 is 360 bytes in the pinned SLUS-20565 image. Arguments: the raster (this), the
// index of a source palette and a flag. It allocates 16 bytes with __builtin_new; if that succeeds it zeroes the words at +4 then +0
// and constructs an int array (__t7VIArray1Zi) at +8, and an allocation failure gives a null palette, which returns -1. Then
// GetMaterialPalSize__8VIRasteri(raster, source index) sizes the new array through Init__t7VIArray1Zii(palette + 8, size)
// (negative -> -1) and Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(raster + 19428, &palette word) gives the slot index
// (negative -> -1). The source palette is the first word of the 12-byte slot at that index in the table whose pointer is the pool's
// word at +8. Entries of the source int array (a sub-object at palette +8: count at +0, data pointer at +4) are copied while
// index < GetMaterialPalSize(raster, source index) (called again before every test): CopyMaterial__8VIRasterii(raster, entry, flag) is
// called and its result is stored in the same slot of the new array's data (the pointer at new palette +12), then a negative result
// returns -1. After the loop a non-zero flag increments the new palette's word at +4. It returns the new slot index.
// Inferred: this duplicates a material palette and each of its materials; the flag is passed through to CopyMaterial and also asks for
// an initial reference on the copy.
// Unknown: what CopyMaterial's flag does, the array and pool classes, and the callees' behaviour (all are imports of this unit). A failed copy leaves the new palette in the pool. Nothing here ran on
// the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_POINTER_TO_WORD(pointer) norrath_host_address(pointer)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_POINTER_TO_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif
extern "C" {
void* __builtin_new(unsigned size);
void __t7VIArray1Zi(unsigned char* array);
int Init__t7VIArray1Zii(unsigned char* array, int count);
int Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(unsigned char* pool, const unsigned* element);
int GetMaterialPalSize__8VIRasteri(unsigned char* raster, int index);
int CopyMaterial__8VIRasterii(unsigned char* raster, int index, int flag);
int CopyMaterialPal__8VIRasterii(unsigned char* self, int source, int flag) {
    unsigned char* palette = static_cast<unsigned char*>(__builtin_new(16));
    if (palette) {
        *reinterpret_cast<int*>(palette + 4) = 0;
        *reinterpret_cast<int*>(palette) = 0;
        __t7VIArray1Zi(palette + 8);
    }
    unsigned paletteWord = palette ? NORRATH_POINTER_TO_WORD(palette) : 0;
    if (!palette) return -1;
    int size = GetMaterialPalSize__8VIRasteri(self, source);
    if (Init__t7VIArray1Zii(NORRATH_WORD_TO_POINTER(paletteWord) + 8, size) < 0) return -1;
    unsigned char* pool = self + 19428;
    int slot = Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(pool, &paletteWord);
    if (slot < 0) return -1;
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8));
    unsigned char* sourceArray = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + source * 12)) + 8;
    int i = 0;
    while (i < GetMaterialPalSize__8VIRasteri(self, source)) {
        int offset = i * 4;
        unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sourceArray + 4));
        i++;
        int copied = CopyMaterial__8VIRasterii(self, *reinterpret_cast<int*>(entries + offset), flag);
        unsigned char* held = NORRATH_WORD_TO_POINTER(paletteWord);
        *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(held + 12)) + offset) = copied;
        if (copied < 0) return -1;
    }
    if (flag != 0) {
        unsigned char* held = NORRATH_WORD_TO_POINTER(paletteWord);
        *reinterpret_cast<int*>(held + 4) += 1;
    }
    return slot;
}
}
