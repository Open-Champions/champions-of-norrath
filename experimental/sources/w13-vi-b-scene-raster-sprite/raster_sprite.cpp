// Observed: RasterSprite__7VISceneR8VICameraiUi at 0x01144bf8, 100 bytes. If the word at
// scene + 19356 is zero it returns -1. Otherwise it takes the pointer stored at
// (the word at scene + 1004) + index * 12, calls a virtual-style method of that object, and
// returns its result. The method is found through the object's word at +60, entry +24 of
// that table: a 16-bit adjustment added to the object pointer to give the first argument,
// and a function pointer at entry + 4. The remaining arguments are the camera (unchanged),
// the scene, the scene's word at 19356, and the unsigned flags.
// Inferred: the 12-byte records at scene + 1004 start with a sprite-like object pointer;
// the table entry is a compiler-generated virtual call slot.
// Unknown: the member names, the record's other two words, the callee's declared types, and
// the type of the scene word passed as the fourth argument (an int is assumed).
struct VITableEntry {
    short adjust;
    short pad;
    int (*function)(void* self, void* camera, void* scene, int sceneWord, unsigned flags);
}
#if defined(NORRATH_HOST_CONTRACT)
__attribute__((packed))
#endif
;

extern "C" {
int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags) {
    unsigned char* s = static_cast<unsigned char*>(scene);
    int sceneWord = *reinterpret_cast<int*>(s + 19356);
    if (sceneWord == 0) return -1;
    unsigned char* records = *reinterpret_cast<unsigned char**>(s + 1004);
    unsigned char* object = *reinterpret_cast<unsigned char**>(records + index * 12);
    VITableEntry* entry = reinterpret_cast<VITableEntry*>(*reinterpret_cast<unsigned char**>(object + 60) + 24);
    return entry->function(object + entry->adjust, camera, scene, sceneWord, flags);
}
}
