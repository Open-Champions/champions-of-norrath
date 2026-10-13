// Observed: MaxPointLights__8VIRaster at 0x01114168, 8 bytes. It returns the constant 1
// without reading the raster. Inferred: the point-light limit of this renderer is one.
// Unknown: whether any caller treats the value as a hard limit, and what a point light
// is in this renderer.
extern "C" {
int MaxPointLights__8VIRaster(void*) {
    return 1;
}
}
