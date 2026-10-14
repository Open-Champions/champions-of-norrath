// Observed lightPopSceneLights__Fv at 0x0013ea18, 168 bytes; the counterpart of the
// authored lightPushSceneLights. A signed interfaceMode (0x00483aac) at or below zero
// returns at once. Otherwise interfaceMode is stored as itself minus one, the 56 bytes
// of savedScene (0x004f4098) are copied back to scene (0x004f4060), and
// transformSceneLightTask__Fv is called. The copy is done before the call; the final
// bytes land in the call's delay slot. Fresh compilation is not an original-byte match.
extern "C" {
extern int interfaceMode;
extern unsigned char scene[];
extern unsigned char savedScene[];
void transformSceneLightTask__Fv();

void lightPopSceneLights__Fv() {
    const int mode = interfaceMode;
    if (mode <= 0) return;
    interfaceMode = mode - 1;
    for (unsigned n = 0u; n < 56u; n += 4u) {
        *reinterpret_cast<unsigned*>(scene + n) = *reinterpret_cast<const unsigned*>(savedScene + n);
    }
    transformSceneLightTask__Fv();
}
}
