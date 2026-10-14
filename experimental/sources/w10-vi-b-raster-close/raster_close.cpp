// Observed Close__8VIRaster at 0x01112cd0, 64 bytes. It calls
// VIEEDMASync__Fi(1), then Clear__8VIRaster(raster). The result is -1 when
// Clear returned a negative value and zero otherwise.
extern "C" {
struct VIRaster;
void VIEEDMASync__Fi(int mode);
int Clear__8VIRaster(VIRaster* raster);

int Close__8VIRaster(VIRaster* raster) {
    VIEEDMASync__Fi(1);
    return Clear__8VIRaster(raster) < 0 ? -1 : 0;
}
}
