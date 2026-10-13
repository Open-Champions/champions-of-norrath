// Observed lightSetAmbientOnly__Fv at 0x0013e7d0, extent 52. Stores 1 to the
// word ambientOnly (0x00483ab0), and zeroes the four halfwords of charDirect
// (0x00486858) and the four halfwords of charDirect2 (0x00486860). It does not
// touch charAmbient. Returns nothing.
// Inferred: the two zeroed objects are directional-light colours switched off.
// Unknown: why the original clears them in this order. ambientOnly, charDirect
// and charDirect2 are defined by other packages.
extern "C" {
extern int ambientOnly;
extern unsigned char charDirect[8];
extern unsigned char charDirect2[8];
void lightSetAmbientOnly__Fv() {
    unsigned short* a = reinterpret_cast<unsigned short*>(charDirect);
    unsigned short* b = reinterpret_cast<unsigned short*>(charDirect2);
    a[0] = 0;
    b[0] = 0;
    ambientOnly = 1;
    a[3] = 0;
    a[2] = 0;
    a[1] = 0;
    b[3] = 0;
    b[2] = 0;
    b[1] = 0;
}
}
