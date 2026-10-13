// Observed: EnableTessellation__8VIRasteri at 0x011141e0, 8 bytes. It stores its int
// argument at raster + 60 and returns without setting a result register.
// Inferred: the word is the tessellation switch read by the drawing code.
// Unknown: its member name, and which values other than 0 and 1 are used.
extern "C" {
void EnableTessellation__8VIRasteri(void* raster, int enable) {
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 60) = enable;
}
}
