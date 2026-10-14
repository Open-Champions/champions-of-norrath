// Observed: UV0__12VIPrimBufferR10VIVect2S16 at 0x01102cf0 is 32 bytes. It copies two halfwords
// from the argument into the static object _12VIPrimBuffer$PrimUV0S16 (0x011b0118 in the pinned
// image, 4 bytes) and returns 0. The buffer object is not read or written.
// Inferred: the static is the current 16-bit texture coordinate that
// Vertex__12VIPrimBufferR10VIVect3S16 copies into each vertex slot. Unknown: the fixed-point format.
extern unsigned short PrimUV0S16[2] __asm__("_12VIPrimBuffer$PrimUV0S16");

extern "C" {
int UV0__12VIPrimBufferR10VIVect2S16(void*, const unsigned short* uv) {
    PrimUV0S16[0] = uv[0];
    PrimUV0S16[1] = uv[1];
    return 0;
}
}
