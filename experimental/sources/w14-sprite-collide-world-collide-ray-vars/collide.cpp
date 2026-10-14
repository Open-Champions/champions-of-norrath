// Observed: Collide__7VIWorldR18VISceneCollRayVars at 0x010fe6a0, 128 bytes. Arguments: the world (W) and a vars record (V).
//  1. The word at W+68 is stored to V+0.
//  2. Collide__7VIWorldiR18VISceneCollRayVars(W, first, V) runs, with `first` 0 when the word at W+52 is nonzero and
//     0x40000000 when it is zero.
//  3. The word at W+68 is read again and incremented by one (so the call may have changed it first).
//  4. The result is 1 if the call returned nonzero, otherwise 0.
// Inferred: W+68 is a per-world counter or stamp copied into the vars for each query. Unknown: its name and the meaning of W+52 and the selector values.
extern "C" {
int Collide__7VIWorldiR18VISceneCollRayVars(unsigned char* world, int selector, unsigned char* vars);

int Collide__7VIWorldR18VISceneCollRayVars(unsigned char* world, unsigned char* vars) {
    *reinterpret_cast<unsigned*>(vars) = *reinterpret_cast<unsigned*>(world + 68);
    const int selector = *reinterpret_cast<int*>(world + 52) != 0 ? 0 : 0x40000000;
    const int hit = Collide__7VIWorldiR18VISceneCollRayVars(world, selector, vars) != 0;
    *reinterpret_cast<unsigned*>(world + 68) = *reinterpret_cast<unsigned*>(world + 68) + 1;
    return hit;
}
}
