// Observed: Lock__7VIRFontR8VIRaster at 0x01136028 is 32 bytes in the pinned SLUS-20565 image. If the word at offset 8 of the font
// object is zero it stores 1 there and returns 0; if it is nonzero it returns -1 and changes nothing. The raster argument is not used.
// Inferred: the word is a lock flag.
// Unknown: what the lock protects. Nothing here ran on the EE.
extern "C" {
int Lock__7VIRFontR8VIRaster(unsigned char* self, unsigned char*) {
    int* flag = reinterpret_cast<int*>(self + 8);
    if (*flag != 0) return -1;
    *flag = 1;
    return 0;
}
}
