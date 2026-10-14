// Observed: ReleaseAnimation__7VIScenei at 0x01144e50, 172 bytes. Arguments: the scene (S) and an animation index (I). The pool is at S+1020; its
// record array pointer is 8 bytes in and the animation A is the first word of 12-byte record I. It decrements the word at A+4 (stored
// at once). When the result is still above zero it returns 0. Otherwise, if the word at A+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(the dictionary word at S+19352, A) and returns -1 for a negative result. Then it calls
// _$_13VIHSpriteAnim(A, 3) and Erase__t6VIPool1ZP13VIHSpriteAnimi(pool, I); a negative Erase result returns -1, else 0.
// Inferred: the word at +4 is a reference count and the word at +0 marks an animation registered in the scene's dictionary; the destructor
// flag 3 is passed as in the sprite release routines.
// Unknown: the destructor's flag meanings and whether the block is freed by it.
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
void _$_13VIHSpriteAnim(unsigned char* animation, int flags);
int Erase__t6VIPool1ZP13VIHSpriteAnimi(unsigned char* pool, int index);

int ReleaseAnimation__7VIScenei(unsigned char* self, int index) {
    unsigned char* pool = self + 1020;
    unsigned char* animation = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12));
    int references = *reinterpret_cast<int*>(animation + 4) - 1;
    *reinterpret_cast<int*>(animation + 4) = references;
    if (references > 0) return 0;
    if (*reinterpret_cast<int*>(animation) != 0) {
        if (Remove__12VIDictionaryR10VIResource(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19352)), animation) < 0) return -1;
    }
    _$_13VIHSpriteAnim(animation, 3);
    if (Erase__t6VIPool1ZP13VIHSpriteAnimi(pool, index) < 0) return -1;
    return 0;
}
}
