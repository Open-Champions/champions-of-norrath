// Observed RenderByPortals__7VIWorldR15VISceneRendVars at 0x010fb9d0, 372 bytes. With the world as
// this and the render variables as vars (offsets in bytes), in this order:
//   1. vars + 8 = world. If both the words at world + 52 and world + 60 are zero the result is 0.
//   2. FindLeaf__C7VIWorldRC7VIVect3RiT2(world, *(vars + 12) + 28, &x, &y); negative gives -1.
//   3. zone = *(world + 76) + 592 * x; room = *(int*)(*(zone + 80) + 8 * y); entry = *(zone + 96) + 120 * room.
//   4. If bit 0 of the word at entry is set: the result is that of RenderByFrustum(world, vars)
//      (-1 if negative, otherwise 0).
//   5. Otherwise: vars + 48 = x; r = SetPretranslations__6VIZoneR8VIRasterR8VICamera(zone, *(vars + 16),
//      *(vars + 12)), r < 0 gives -1; PushFrustum__11VIClipStackR8VICamera(*(vars + 20), *(vars + 12)),
//      negative gives -1; GetTop(*(vars + 20), vars + 24, vars + 28, vars + 32, vars + 36) unused;
//      RenderRoom__6VIZoneiR15VISceneRendVars(*(world + 76) + 592 * x, room, vars), negative gives -1;
//      Pop__11VIClipStack(*(vars + 20)); the result is -1 if its value is negative, otherwise 0.
// The zone table pointer is re-read for the RenderRoom call, as the original does.
// Unknown: what x, y, room and the bit-0 flag mean, and the layouts behind the strides 592, 8 and 120.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIWorld;
struct VISceneRendVars;
int FindLeaf__C7VIWorldRC7VIVect3RiT2(VIWorld* world, const void* position, int* x, int* y);
int RenderByFrustum__7VIWorldR15VISceneRendVars(VIWorld* world, VISceneRendVars* vars);
int SetPretranslations__6VIZoneR8VIRasterR8VICamera(void* zone, unsigned raster, unsigned camera);
int PushFrustum__11VIClipStackR8VICamera(unsigned stack, unsigned camera);
void GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(unsigned stack, void* a, void* b, void* c, void* d);
int RenderRoom__6VIZoneiR15VISceneRendVars(void* zone, int room, VISceneRendVars* vars);
int Pop__11VIClipStack(unsigned stack);

static unsigned word_at(const char* address) { return *reinterpret_cast<const unsigned*>(address); }

int RenderByPortals__7VIWorldR15VISceneRendVars(VIWorld* world, VISceneRendVars* vars) {
    char* w = reinterpret_cast<char*>(world);
    char* v = reinterpret_cast<char*>(vars);
    *reinterpret_cast<unsigned*>(v + 8) = static_cast<unsigned>(reinterpret_cast<unsigned long>(world));
    if (word_at(w + 52) == 0 && word_at(w + 60) == 0) return 0;
    int x, y;
    const char* camera = NORRATH_TRANSLATE(word_at(v + 12));
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(world, camera + 28, &x, &y) < 0) return -1;
    char* zone = NORRATH_TRANSLATE(word_at(w + 76)) + 592 * x;
    int room = *reinterpret_cast<const int*>(NORRATH_TRANSLATE(word_at(zone + 80)) + 8 * y);
    const char* entry = NORRATH_TRANSLATE(word_at(zone + 96)) + 120 * room;
    int result;
    if (word_at(entry) & 1) {
        result = RenderByFrustum__7VIWorldR15VISceneRendVars(world, vars);
    } else {
        *reinterpret_cast<int*>(v + 48) = x;
        if (SetPretranslations__6VIZoneR8VIRasterR8VICamera(zone, word_at(v + 16), word_at(v + 12)) < 0) return -1;
        if (PushFrustum__11VIClipStackR8VICamera(word_at(v + 20), word_at(v + 12)) < 0) return -1;
        GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(word_at(v + 20), v + 24, v + 28, v + 32, v + 36);
        if (RenderRoom__6VIZoneiR15VISceneRendVars(NORRATH_TRANSLATE(word_at(w + 76)) + 592 * x, room, vars) < 0)
            return -1;
        result = Pop__11VIClipStack(word_at(v + 20));
    }
    return result < 0 ? -1 : 0;
}
}
