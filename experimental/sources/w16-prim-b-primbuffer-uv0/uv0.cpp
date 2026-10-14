// Observed: UV0__12VIPrimBufferR7VIVect2 at 0x01102cd0 is 32 bytes. It copies two floats from the
// argument into the static object _12VIPrimBuffer$PrimUV0 (0x011b0110 in the pinned image, 8
// bytes) and returns 0. The buffer object is not read or written.
// Inferred: the static is the current texture coordinate that Vertex__12VIPrimBufferR7VIVect3
// copies into the second slot of each vertex. Unknown: the second coordinate set (a neighbouring
// static named PrimUV1 exists) and its writers.
extern float PrimUV0[2] __asm__("_12VIPrimBuffer$PrimUV0");

extern "C" {
int UV0__12VIPrimBufferR7VIVect2(void*, const float* uv) {
    PrimUV0[0] = uv[0];
    PrimUV0[1] = uv[1];
    return 0;
}
}
