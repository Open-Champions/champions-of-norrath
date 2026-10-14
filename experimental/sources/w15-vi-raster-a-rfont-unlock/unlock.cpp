// Observed: Unlock__7VIRFontR8VIRaster at 0x01136048 is 60 bytes in the pinned SLUS-20565 image. If the word at offset 8 of the font
// object is zero it returns -1. Otherwise it stores zero there, calls ResetCache__7VIRFontR8VIRaster(font, raster), and returns -1 if
// that call's result is negative, else 0.
// Inferred: the word at +8 is the lock flag set by Lock__7VIRFontR8VIRaster.
// Unknown: ResetCache's behavior (not authored here; it stays an unresolved import). Nothing here ran on the EE.
extern "C" {
int ResetCache__7VIRFontR8VIRaster(unsigned char* self, unsigned char* raster);
int Unlock__7VIRFontR8VIRaster(unsigned char* self, unsigned char* raster) {
    int* flag = reinterpret_cast<int*>(self + 8);
    if (*flag == 0) return -1;
    *flag = 0;
    return ResetCache__7VIRFontR8VIRaster(self, raster) < 0 ? -1 : 0;
}
}
