// Observed: ReleaseRefMap__7VIScenei at 0x0113ce58, 180 bytes. Arguments: the scene (S) and an index (I). The pool is at S+1044; its record array
// pointer is 8 bytes in and the map M is the first word of 12-byte record I. It decrements the word at M+4 (stored at once); when the result is still
// above zero it returns 0. Otherwise, if the word at M+0 is nonzero it calls Remove__12VIDictionaryR10VIResource(the dictionary word at S+19352, M)
// and returns -1 for a negative result. Then it calls _$_t5VIMap2ZiZi(M+8, 2), __builtin_delete(M) and Erase__t6VIPool1ZP8VIRefMapi(pool, I);
// a negative Erase result returns -1, else 0.
// Inferred: the word at +4 is a reference count, the word at +0 marks a dictionary-registered resource, and the member at +8 is a two-parameter
// integer map.
// Unknown: the template arguments' meaning, and why the member destructor is called with flag 2 and the block then released by hand.
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
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
void _$_t5VIMap2ZiZi(unsigned char* map, int flags);
void __builtin_delete(unsigned char* block);
int Erase__t6VIPool1ZP8VIRefMapi(unsigned char* pool, int index);

int ReleaseRefMap__7VIScenei(unsigned char* self, int index) {
    unsigned char* pool = self + 1044;
    unsigned char* map = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12));
    int references = *reinterpret_cast<int*>(map + 4) - 1;
    *reinterpret_cast<int*>(map + 4) = references;
    if (references > 0) return 0;
    if (*reinterpret_cast<int*>(map) != 0) {
        if (Remove__12VIDictionaryR10VIResource(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19352)), map) < 0) return -1;
    }
    _$_t5VIMap2ZiZi(map + 8, 2);
    __builtin_delete(map);
    if (Erase__t6VIPool1ZP8VIRefMapi(pool, index) < 0) return -1;
    return 0;
}
}
