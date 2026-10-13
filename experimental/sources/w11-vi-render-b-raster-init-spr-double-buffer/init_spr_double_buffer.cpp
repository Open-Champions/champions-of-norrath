// Observed: InitSPRDoubleBuffer__8VIRaster at 0x01114770 is 32 bytes. Called with the raster
// pointer as its only argument, it stores three words at the pointed-to address: 0x70002000 at
// +0, 0x70000000 at +4 and 0x70002000 at +8, then returns zero.
// Inferred: the three words are scratchpad addresses of a double buffer (the scratchpad base and the
// address 0x2000 bytes into it; the third word equals the first).
// Unknown: which member names these offsets have, and what reads them.
extern "C" {
struct VIRaster;

int InitSPRDoubleBuffer__8VIRaster(VIRaster* self) {
    unsigned* words = reinterpret_cast<unsigned*>(self);
    words[0] = 0x70002000u;
    words[1] = 0x70000000u;
    words[2] = 0x70002000u;
    return 0;
}
}
