// Observed lightGlobalCharLightScale__Ff at 0x0013eac0, 8 bytes. The float argument
// (F12) is stored to the single-precision word globalCharLightScale (0x00483ab4, a
// gp-relative object) in the return's delay slot. Nothing is read and nothing is returned.
// Unknown: the permitted range of the value. Fresh compilation is not an original-byte match.
extern "C" {
extern float globalCharLightScale;

void lightGlobalCharLightScale__Ff(float scale) {
    globalCharLightScale = scale;
}
}
