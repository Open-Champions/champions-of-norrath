// Observed: VertexGroup__12VIPrimBuffers at 0x01102e50 is 16 bytes. It stores its 16-bit argument
// into the static object _12VIPrimBuffer$PrimVertexGroup (0x01228a30 in the pinned image, as a
// halfword store) and returns 0; the object itself is not read or written.
// Inferred: the static is the group value attached to the vertices that follow.
// Unknown: the static's size beyond the stored halfword and its consumer (an unresolved import here).
extern short PrimVertexGroup __asm__("_12VIPrimBuffer$PrimVertexGroup");

extern "C" {
int VertexGroup__12VIPrimBuffers(void*, short group) {
    PrimVertexGroup = group;
    return 0;
}
}
