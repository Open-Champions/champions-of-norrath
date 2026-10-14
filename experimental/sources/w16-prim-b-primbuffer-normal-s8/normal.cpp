// Observed: Normal__12VIPrimBufferR9VIVect3S8 at 0x01102d78 is 40 bytes. It copies the first
// three bytes of the argument into the static object _12VIPrimBuffer$PrimNormalS8 (0x011b0140 in
// the pinned image, 3 bytes) and returns 0. The buffer object is not read or written.
// Inferred: the static is the current packed normal that Vertex__12VIPrimBufferR10VIVect3S16
// copies into the vertex slot. Unknown: whether the bytes are signed, and the VIVect3S8 layout
// beyond its first three bytes.
extern unsigned char PrimNormalS8[3] __asm__("_12VIPrimBuffer$PrimNormalS8");

extern "C" {
int Normal__12VIPrimBufferR9VIVect3S8(void*, const unsigned char* n) {
    PrimNormalS8[0] = n[0];
    PrimNormalS8[1] = n[1];
    PrimNormalS8[2] = n[2];
    return 0;
}
}
