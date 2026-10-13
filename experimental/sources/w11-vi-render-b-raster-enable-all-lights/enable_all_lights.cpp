// Observed: EnableAllLights__8VIRasteri at 0x01114170 is 48 bytes. It stores the low byte of its
// integer argument at the three absolute addresses 0x70000220, 0x70000221 and 0x70000222 and,
// after each store, writes 1 to offset 17888 of the raster. It returns zero.
// Inferred: three consecutive scratchpad bytes are per-light enables; 17888 is a dirty flag
// (SetModelView and SetProjection also set it).
// Unknown: which light each byte is.
extern "C" {
struct VIRaster;

int EnableAllLights__8VIRasteri(VIRaster* raster, int enable) {
    volatile unsigned char* flags = reinterpret_cast<volatile unsigned char*>(0x70000220u);
    unsigned* dirty = reinterpret_cast<unsigned*>(reinterpret_cast<char*>(raster) + 17888);
    flags[0] = static_cast<unsigned char>(enable);
    *dirty = 1;
    flags[1] = static_cast<unsigned char>(enable);
    *dirty = 1;
    flags[2] = static_cast<unsigned char>(enable);
    *dirty = 1;
    return 0;
}
}
