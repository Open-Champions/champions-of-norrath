// Observed: Normal__12VIPrimBufferR7VIVect3 at 0x01102d50 is 40 bytes. It copies three floats
// (offsets 0, 4, 8) from the vector argument into the static data object named
// _12VIPrimBuffer$PrimNormal (0x011b0130 in the pinned image) and returns 0. `this` is unused.
// Inferred: the object holds the current normal applied to following vertices.
// Unknown: its full size and which routine consumes it (it is an unresolved import here).
extern float PrimNormal[3] __asm__("_12VIPrimBuffer$PrimNormal");

extern "C" {
int Normal__12VIPrimBufferR7VIVect3(void*, const float* normal) {
    PrimNormal[0] = normal[0];
    PrimNormal[1] = normal[1];
    PrimNormal[2] = normal[2];
    return 0;
}
}
