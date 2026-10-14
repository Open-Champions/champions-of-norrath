// Observed: ReleaseStaticLighting__7VIScenei at 0x0113cf10, 216 bytes. Arguments: the scene (S) and an index (I). The pool is at S+1068; its record array
// pointer is 8 bytes in and the lighting L is the first word of 12-byte record I. It decrements the word at L+4 (stored at once); when the result is
// still above zero it returns 0. Otherwise it calls ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary(L, the raster word at S+19356,
// the dictionary word at S+19352) and returns -1 for a negative result. If the word at L+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(dictionary, L) and returns -1 for a negative result. It then calls _$_t7VIArray1Zi(L+8, 2), __builtin_delete(L)
// and Erase__t6VIPool1ZP16VIStaticLightingi(pool, I); a negative Erase result returns -1, else 0.
// Inferred: the word at +4 is a reference count, the word at +0 marks a dictionary-registered resource and the member at +8 is the array of
// colour buffers read by GetNextColorBuffer__7VIScene.
// Unknown: whether the raster word may be null here (it is passed through unchecked).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary(unsigned char* lighting, unsigned char* raster, unsigned char* dictionary);
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
void _$_t7VIArray1Zi(unsigned char* array, int flags);
void __builtin_delete(unsigned char* block);
int Erase__t6VIPool1ZP16VIStaticLightingi(unsigned char* pool, int index);

int ReleaseStaticLighting__7VIScenei(unsigned char* self, int index) {
    unsigned char* pool = self + 1068;
    unsigned char* lighting = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12));
    int references = *reinterpret_cast<int*>(lighting + 4) - 1;
    *reinterpret_cast<int*>(lighting + 4) = references;
    if (references > 0) return 0;
    if (ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary(lighting, NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19356)), NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19352))) < 0) return -1;
    if (*reinterpret_cast<int*>(lighting) != 0) {
        if (Remove__12VIDictionaryR10VIResource(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19352)), lighting) < 0) return -1;
    }
    _$_t7VIArray1Zi(lighting + 8, 2);
    __builtin_delete(lighting);
    if (Erase__t6VIPool1ZP16VIStaticLightingi(pool, index) < 0) return -1;
    return 0;
}
}
