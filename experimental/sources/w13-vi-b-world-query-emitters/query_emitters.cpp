// Observed: QueryEmitters__7VIWorldR23VISceneEmitterQueryVars at 0x010feac0, 108 bytes. It
// copies the world word at +68 into the first word of the query-vars argument, then calls
// QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(world, mode, vars) where mode is 0 if
// the world word at +52 is nonzero and 0x40000000 if it is zero. When that call returns a
// non-negative value it increments the world word at +68 and returns 0; otherwise it
// returns -1.
// Inferred: the word at +68 is a query serial number recorded in the vars and advanced on
// success; the int mode is a flag word selected by the world word at +52.
// Unknown: the member names, the meaning of mode 0x40000000, and the vars layout.
extern "C" {
int QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(void* world, int mode, void* vars);

int QueryEmitters__7VIWorldR23VISceneEmitterQueryVars(void* world, void* vars) {
    unsigned char* w = static_cast<unsigned char*>(world);
    *reinterpret_cast<int*>(vars) = *reinterpret_cast<int*>(w + 68);
    int mode = *reinterpret_cast<int*>(w + 52) != 0 ? 0 : 0x40000000;
    if (QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(world, mode, vars) < 0) return -1;
    *reinterpret_cast<int*>(w + 68) += 1;
    return 0;
}
}
