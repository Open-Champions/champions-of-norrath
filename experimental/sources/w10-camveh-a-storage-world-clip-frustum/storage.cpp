// Observed worldClipFrustum at 0x006eb9f8: zero-initialised storage (.bss) of 96 bytes,
// the exact distance to the next symbol (clipViewFrustum at 0x006eba58). It is read and
// written as six planes of four floats by the authored world frustum and clip-cube code.
// What the planes mean is inferred from those names only.
extern "C" {
float worldClipFrustum[24];
}
