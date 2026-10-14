// Observed: QueryEmitters__6VIZoneR23VISceneEmitterQueryVars at 0x01119c38, 68 bytes. Arguments: the zone (Z) and the query-variables record (V).
// It calls QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(Z, flags, V) with flags = 0 when the word at Z+68 is nonzero and flags =
// 0x40000000 when it is zero, then returns -1 for a negative result and 0 otherwise.
// Inferred: Z+68 is the zone's tree (the sibling RenderByVolume entry point uses the same test); the flag marks a start at a leaf.
// Unknown: the meaning of the flag bits beyond the sibling traversal code, and the record layout.
extern "C" {
int QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(unsigned char* zone, int flags, unsigned char* vars);

int QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(unsigned char* zone, unsigned char* vars) {
    int flags = *reinterpret_cast<int*>(zone + 68) != 0 ? 0 : 0x40000000;
    if (QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(zone, flags, vars) < 0) return -1;
    return 0;
}
}
