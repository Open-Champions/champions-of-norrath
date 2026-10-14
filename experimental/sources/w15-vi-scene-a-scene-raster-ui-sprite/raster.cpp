// Observed: RasterUISprite__7VISceneR8VICameraiUi at 0x01144c60, 152 bytes. Arguments: the scene (S), the camera (C), a sprite index (I) and an
// unsigned value (U). It saves the word at S+1092, stores -1 there, and returns -1 if the raster word at S+19356 is zero. Otherwise it takes
// the sprite P from record I of the 12-byte record array whose pointer is at S+1004 and calls the function in slot 24 of the table at P+60
// (halfword adjustment at +0, function word at +4) with (P + adjustment, C, S, the raster word, U). If the result is negative it returns -1
// and S+1092 stays -1; otherwise it restores the saved word at S+1092 and returns 0.
// Inferred: S+1092 is the same current static-lighting index that GetNextColorBuffer__7VIScene reads; it is cleared while the sprite is
// drawn so that nothing hands out colour buffers, and put back after a successful draw.
// Unknown: why the index is left at -1 after a failed draw, the meaning of U, and the slot's name.
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
int RasterUISprite__7VISceneR8VICameraiUi(unsigned char* self, unsigned char* camera, int index, unsigned flags) {
    int saved = *reinterpret_cast<int*>(self + 1092);
    unsigned raster = *reinterpret_cast<unsigned*>(self + 19356);
    *reinterpret_cast<int*>(self + 1092) = -1;
    if (raster == 0) return -1;
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 1004));
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + index * 12));
    unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 24;
    typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned);
    Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
    if (function(sprite + *reinterpret_cast<short*>(slot), camera, self, NORRATH_WORD_TO_POINTER(raster), flags) < 0) return -1;
    *reinterpret_cast<int*>(self + 1092) = saved;
    return 0;
}
}
