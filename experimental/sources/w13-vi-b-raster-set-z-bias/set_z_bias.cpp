// Observed: SetZBias__8VIRasteri at 0x01114438, 84 bytes. If the int argument is greater
// than zero it stores (float)argument * 166.0f (the image's constant is 0x43260000) at
// raster + 17544; otherwise it stores integer zero there. It then calls
// CalcClipToScreen__8VIRaster(raster), stores 1 at raster + 17888 and returns the result
// of that call unchanged.
// Inferred: the float at raster + 17544 is a depth offset applied when the clip-to-screen
// matrix is rebuilt, which is why that routine is called straight afterwards.
// Unknown: the member name, the unit of the scale 166.0, and the true return type (an int
// is assumed).
extern "C" {
int CalcClipToScreen__8VIRaster(void* raster);

int SetZBias__8VIRasteri(void* raster, int bias) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    if (bias > 0) {
        *reinterpret_cast<float*>(r + 17544) = static_cast<float>(bias) * 166.0f;
    } else {
        *reinterpret_cast<int*>(r + 17544) = 0;
    }
    int result = CalcClipToScreen__8VIRaster(raster);
    *reinterpret_cast<int*>(r + 17888) = 1;
    return result;
}
}
