// Observed farClip at 0x004869d8. The ELF size field is 0. lightHaloTexture
// is the next symbol, four bytes later, so this object is one word. The
// .sbss file bytes are zero. worldInit stores 0x44e10000 here before it
// calls worldSetFrustum. This unit does not perform that store.
extern "C" {
float farClip;
}
