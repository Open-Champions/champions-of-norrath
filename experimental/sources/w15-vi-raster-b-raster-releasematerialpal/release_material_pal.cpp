// Observed: ReleaseMaterialPal__8VIRasteriR12VIDictionary at 0x0110e338 is 292 bytes in the pinned SLUS-20565 image. Arguments: the
// raster (this), a slot index and a dictionary. The material-palette pool is the sub-object at raster + 19428 (0x4be4); its table
// pointer is the pool's word at +8 and slots are 12 bytes with the palette pointer in the first word. The routine loads the palette,
// decrements its word at +4 and stores it back; a result above zero returns 0. Otherwise the palette's int array (a sub-object at
// palette +8 whose word at +0 is a count and word at +4 a pointer to ints) is walked from index 0: for each entry it calls
// ReleaseMaterial__8VIRasteriR12VIDictionary(raster, entry, dictionary) and returns -1 at the first negative result; the count and
// the pointer are re-read every iteration. If the palette's first word is non-zero it calls
// Remove__12VIDictionaryR10VIResource(dictionary, palette), returning -1 when negative. Then _$_t7VIArray1Zi(array, 2) (the array
// destructor), __builtin_delete(palette) and Erase__t6VIPool1ZP13VIMaterialPali(pool, index) run; the result is 0 when Erase is not
// negative and -1 otherwise.
// Inferred: +4 is a reference count, the array entries are material slot indexes of the raster, and the first word says whether the
// palette is registered in the dictionary.
// Unknown: the palette layout beyond those words, the array class, and the callees' behaviour (unresolved imports except
// ReleaseMaterial, which is defined elsewhere in the selected sources). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int ReleaseMaterial__8VIRasteriR12VIDictionary(unsigned char* raster, int index, unsigned char* dictionary);
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
void _$_t7VIArray1Zi(unsigned char* array, int flags);
void __builtin_delete(void* pointer);
int Erase__t6VIPool1ZP13VIMaterialPali(unsigned char* pool, int index);
int ReleaseMaterialPal__8VIRasteriR12VIDictionary(unsigned char* self, int index, unsigned char* dictionary) {
    unsigned char* pool = self + 19428;
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8));
    unsigned char* palette = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    int count = *reinterpret_cast<int*>(palette + 4) - 1;
    *reinterpret_cast<int*>(palette + 4) = count;
    if (count > 0) return 0;
    unsigned char* array = palette + 8;
    if (*reinterpret_cast<int*>(array) > 0) {
        int i = 0;
        do {
            unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4));
            int entry = *reinterpret_cast<int*>(entries + i * 4);
            i++;
            if (ReleaseMaterial__8VIRasteriR12VIDictionary(self, entry, dictionary) < 0) return -1;
        } while (i < *reinterpret_cast<int*>(array));
    }
    if (*reinterpret_cast<int*>(palette) != 0) {
        if (Remove__12VIDictionaryR10VIResource(dictionary, palette) < 0) return -1;
    }
    _$_t7VIArray1Zi(array, 2);
    __builtin_delete(palette);
    return Erase__t6VIPool1ZP13VIMaterialPali(pool, index) < 0 ? -1 : 0;
}
}
