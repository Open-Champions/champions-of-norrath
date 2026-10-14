// Observed: ReleaseSprite__7VIScenei at 0x0113cd48, 272 bytes, recursive. Arguments: the scene (S) and a sprite index (I). The sprite pool is at S+996;
// its record array pointer is 8 bytes in and the sprite P is the first word of 12-byte record I. If the word at P+56 is not -1 the function
// first calls itself with (S, that word); the pool is then read again. It decrements the word at P+4 (stored at once) and returns 0 when the
// result is still above zero. Otherwise it calls the function in slot 80 of the table at P+60 (halfword adjustment at +0, function word at +4) with
// (P + adjustment, S, the words at S+19352, S+19356, S+19360 and S+19364); a negative result returns -1. It reads the record again and, when
// the sprite word is nonzero, calls slot 8 of its table with (P + adjustment, 3). It finally calls Erase__t6VIPool1ZP8VISpritei(pool, I)
// and returns -1 for a negative result, else 0.
// Inferred: the word at +56 is a parent or source sprite index (CreateActor__7VISceneRC7VIVect3T1fi stores one there), +4 a reference count, slot 80 the
// release virtual (dictionary, raster, sound device and collide object follow the scene) and slot 8 the destructor with delete flag 3.
// Unknown: whether the pool or the sprite's own table may change a record's word during the release (the code re-reads it, so this
// follows the image).
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
int Erase__t6VIPool1ZP8VISpritei(unsigned char* pool, int index);

int ReleaseSprite__7VIScenei(unsigned char* self, int index) {
    unsigned char* pool = self + 996;
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12));
    int parent = *reinterpret_cast<int*>(sprite + 56);
    if (parent != -1) ReleaseSprite__7VIScenei(self, parent);
    sprite = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12));
    int references = *reinterpret_cast<int*>(sprite + 4) - 1;
    *reinterpret_cast<int*>(sprite + 4) = references;
    if (references > 0) return 0;
    unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 80;
    typedef int (*Release)(unsigned char*, unsigned char*, unsigned, unsigned, unsigned, unsigned);
    Release release = reinterpret_cast<Release>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
    if (release(sprite + *reinterpret_cast<short*>(slot), self, *reinterpret_cast<unsigned*>(self + 19352), *reinterpret_cast<unsigned*>(self + 19356),
                *reinterpret_cast<unsigned*>(self + 19360), *reinterpret_cast<unsigned*>(self + 19364)) < 0) return -1;
    unsigned word = *reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 12);
    if (word != 0) {
        unsigned char* object = NORRATH_WORD_TO_POINTER(word);
        unsigned char* destructor = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(object + 60)) + 8;
        typedef void (*Destroy)(unsigned char*, int);
        Destroy destroy = reinterpret_cast<Destroy>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(destructor + 4)));
        destroy(object + *reinterpret_cast<short*>(destructor), 3);
    }
    if (Erase__t6VIPool1ZP8VISpritei(pool, index) < 0) return -1;
    return 0;
}
}
