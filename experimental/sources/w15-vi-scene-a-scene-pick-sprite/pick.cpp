// Observed: PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri at 0x01145b10, 120 bytes. Arguments: the scene (S), the pick segment (G), the scene
// filter (F) and a sprite index (I). The sprite P is the first word of record I in the 12-byte record array whose pointer is at S+1004. It
// calls the function in the slot at 48 of the table at P+60 (a signed halfword adjustment at +0 and the function word at +4) with
// (P + adjustment, G, F, S, the raster word at S+19356) and keeps its result R. When R is nonzero it also stores -1 at F+12, I at F+8 and
// -1 at F+4. It returns R.
// Inferred: the scene-level pick entry: it asks one sprite to test the segment and, on a hit, records the sprite index in the filter's
// result fields; the slot has the same signature as Pick__13VIGroupSprite.
// Unknown: the names of the filter's fields at +4, +8 and +12 and whether the filter's first word is also written by callers.
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
int PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(unsigned char* self, unsigned char* segment, unsigned char* filter, int index) {
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 1004));
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + index * 12));
    unsigned raster = *reinterpret_cast<unsigned*>(self + 19356);
    unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 48;
    typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
    Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
    int result = function(sprite + *reinterpret_cast<short*>(slot), segment, filter, self, NORRATH_WORD_TO_POINTER(raster));
    if (result != 0) {
        *reinterpret_cast<int*>(filter + 12) = -1;
        *reinterpret_cast<int*>(filter + 8) = index;
        *reinterpret_cast<int*>(filter + 4) = -1;
    }
    return result;
}
}
