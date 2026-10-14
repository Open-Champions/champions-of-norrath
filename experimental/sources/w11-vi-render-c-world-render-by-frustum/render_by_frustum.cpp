// Observed RenderByFrustum__7VIWorldR15VISceneRendVars at 0x010fb910, 188 bytes. With the world as
// this and the render variables as vars (word offsets in bytes):
//   1. vars + 40 = 1 and vars + 8 = world, stored first.
//   2. If both the word at world + 52 and the word at world + 60 are zero, the result is 0 and
//      nothing else happens.
//   3. r = PushFrustum__11VIClipStackR8VICamera(*(vars + 20), *(vars + 12)); r < 0 gives -1.
//   4. GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(*(vars + 20), vars + 24, vars + 28, vars + 32,
//      vars + 36), result unused.
//   5. r = RenderByVolume__7VIWorldR15VISceneRendVars(world, vars); r < 0 gives -1.
//   6. r = Pop__11VIClipStack(*(vars + 20)); the result is -1 if r < 0, otherwise 0.
// The words at vars + 20 and vars + 12 are a clip stack and a camera by their callees' names.
// Unknown: the layout of the render variables beyond these offsets, and what world + 52 / + 60 are.
extern "C" {
struct VIWorld;
struct VISceneRendVars;
int PushFrustum__11VIClipStackR8VICamera(unsigned stack, unsigned camera);
void GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(unsigned stack, void* a, void* b, void* c, void* d);
int RenderByVolume__7VIWorldR15VISceneRendVars(VIWorld* world, VISceneRendVars* vars);
int Pop__11VIClipStack(unsigned stack);

int RenderByFrustum__7VIWorldR15VISceneRendVars(VIWorld* world, VISceneRendVars* vars) {
    char* w = reinterpret_cast<char*>(world);
    char* v = reinterpret_cast<char*>(vars);
    *reinterpret_cast<int*>(v + 40) = 1;
    *reinterpret_cast<unsigned*>(v + 8) = static_cast<unsigned>(reinterpret_cast<unsigned long>(world));
    if (*reinterpret_cast<int*>(w + 52) == 0 && *reinterpret_cast<int*>(w + 60) == 0) return 0;
    if (PushFrustum__11VIClipStackR8VICamera(*reinterpret_cast<unsigned*>(v + 20), *reinterpret_cast<unsigned*>(v + 12)) < 0)
        return -1;
    GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(*reinterpret_cast<unsigned*>(v + 20), v + 24, v + 28, v + 32, v + 36);
    if (RenderByVolume__7VIWorldR15VISceneRendVars(world, vars) < 0) return -1;
    return Pop__11VIClipStack(*reinterpret_cast<unsigned*>(v + 20)) < 0 ? -1 : 0;
}
}
