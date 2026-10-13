// Observed EndScene__8VIRaster at 0x01112e60, 192 bytes. Let S be the word at
// offset 19504 of the raster.
//  1. If bit 0 of S is set: Flush__8VIRaster(raster), then VIEEDMASync(1).
//  2. If S is now 2: S is cleared and the counter at offset 17880 is
//     incremented. Otherwise Flip__8VIRaster(raster) runs.
//  3. If S is now zero: Flush__8VIRaster runs and S is read again.
//  4. S becomes 2 when the value just read is 1, otherwise zero.
//  5. The words at 17856 and 17852 are set to 0 and -1, and
//     ReleaseDeferred__8VIRaster(raster) runs. The result is -1 when it
//     returned a negative value and zero otherwise.
// The meaning of S and the two stored words is not established here.
extern "C" {
struct VIRaster;
int Flush__8VIRaster(VIRaster* raster);
void VIEEDMASync__Fi(int mode);
void Flip__8VIRaster(VIRaster* raster);
int ReleaseDeferred__8VIRaster(VIRaster* raster);

int EndScene__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    volatile int* state = reinterpret_cast<volatile int*>(base + 19504);
    if (*state & 1) {
        Flush__8VIRaster(raster);
        VIEEDMASync__Fi(1);
    }
    if (*state == 2) {
        *state = 0;
        *reinterpret_cast<int*>(base + 17880) += 1;
    } else {
        Flip__8VIRaster(raster);
    }
    int seen = *state;
    if (seen == 0) {
        Flush__8VIRaster(raster);
        seen = *state;
    }
    *state = (seen == 1) ? 2 : 0;
    *reinterpret_cast<int*>(base + 17856) = 0;
    *reinterpret_cast<int*>(base + 17852) = -1;
    return ReleaseDeferred__8VIRaster(raster) < 0 ? -1 : 0;
}
}
