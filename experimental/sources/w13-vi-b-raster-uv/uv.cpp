// Observed: UV__8VIRasterRC7VIVect2 at 0x01113268, 28 bytes. It copies the two floats
// at the vector address (offsets 0 and 4) to raster + 12 and raster + 16, in that
// order, and returns 0.
// Inferred: the pair is the current texture coordinate the vertex calls pick up.
// Unknown: the member names, and whether the vertex code reads the pair as floats.
extern "C" {
int UV__8VIRasterRC7VIVect2(void* raster, const void* uv) {
    const float* from = static_cast<const float*>(uv);
    float* to = reinterpret_cast<float*>(static_cast<unsigned char*>(raster) + 12);
    to[0] = from[0];
    to[1] = from[1];
    return 0;
}
}
