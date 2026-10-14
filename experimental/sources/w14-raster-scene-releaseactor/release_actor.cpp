// Observed: ReleaseActor__7VIScenei at 0x0113f058 is 148 bytes in the pinned SLUS-20565 image. The second argument is used both as the
// argument of the scene calls and as a pointer to an actor record, so it is treated here as a 32-bit address. The routine:
//   1. calls RemoveActor__7VIScenei(scene, actor); a negative result returns -1;
//   2. reads the word at actor+96; unless it is -1, calls ReleaseSprite__7VIScenei(scene, that word); a negative result returns -1;
//   3. reads the word at actor+108; unless it is -1, calls ReleaseStaticLighting__7VIScenei(scene, that word); a negative result returns -1;
//   4. tail-calls Erase__t6VIList1Z7VIActori(scene + 972, actor) and returns its result.
// Inferred: releases everything an actor holds (its scene registration, sprite and static lighting) and then removes it from the
// actor list embedded in the scene at +972. The words at +96 and +108 are -1 when the actor has no sprite or lighting.
// Unknown: whether the argument is an index or a pointer in the original source (the mangled name says int), and the meaning of
// the three callees' results beyond sign. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int RemoveActor__7VIScenei(unsigned char* scene, int actor);
int ReleaseSprite__7VIScenei(unsigned char* scene, int sprite);
int ReleaseStaticLighting__7VIScenei(unsigned char* scene, int lighting);
int Erase__t6VIList1Z7VIActori(unsigned char* list, int actor);

int ReleaseActor__7VIScenei(unsigned char* scene, int actor) {
    if (RemoveActor__7VIScenei(scene, actor) < 0) return -1;
    const unsigned char* record = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(actor));
    int sprite = *reinterpret_cast<const int*>(record + 96);
    if (sprite != -1) {
        if (ReleaseSprite__7VIScenei(scene, sprite) < 0) return -1;
    }
    int lighting = *reinterpret_cast<const int*>(record + 108);
    if (lighting != -1) {
        if (ReleaseStaticLighting__7VIScenei(scene, lighting) < 0) return -1;
    }
    return Erase__t6VIList1Z7VIActori(scene + 972, actor);
}
}
