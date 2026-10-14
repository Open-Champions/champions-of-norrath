// Observed: QueryIntersection__7VIWorldR20VISceneIntersectVars at 0x010feb30, 108 bytes. Arguments: the world (W) and the intersection variables (X).
//  1. The word at W+68 is copied to X+0.
//  2. QueryIntersection__7VIWorldiR20VISceneIntersectVars(W, N, X) runs with N = 0 if the word at W+52 is nonzero and 0x40000000 if it is zero.
//  3. A negative result returns -1. Otherwise the word at W+68 is read again and stored back plus one, and the result is 0.
// Same wrapper shape as the world's two collide wrappers (an integer-indexed traversal started at node 0 or at 0x40000000).
// Inferred: the word at W+52 chooses a traversal start (a node versus a root selector), and the word at W+68 counts queries made.
// Unknown: the field names and the meaning of the flag bits of N.
extern "C" {
int QueryIntersection__7VIWorldiR20VISceneIntersectVars(unsigned char* world, int node, unsigned char* vars);

int QueryIntersection__7VIWorldR20VISceneIntersectVars(unsigned char* world, unsigned char* vars) {
    *reinterpret_cast<int*>(vars) = *reinterpret_cast<int*>(world + 68);
    int start = *reinterpret_cast<int*>(world + 52) != 0 ? 0 : 0x40000000;
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, start, vars) < 0) return -1;
    *reinterpret_cast<int*>(world + 68) = *reinterpret_cast<int*>(world + 68) + 1;
    return 0;
}
}
