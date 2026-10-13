// Observed: SetProjection__8VIRasterRC10VIMatrix44 at 0x01112f60 is 60 bytes. It calls
// sceVu0CopyMatrix(0x70000040, matrix), then stores 1 at offsets 17892 and 17888 of the raster
// and returns zero.
// Inferred: scratchpad 0x70000040 holds the projection matrix, 64 bytes after the model-view
// matrix SetModelView writes.
// Unknown: what each flag word triggers.
extern "C" {
struct VIRaster;
struct VIMatrix44;

void sceVu0CopyMatrix(void* destination, const void* source);

int SetProjection__8VIRasterRC10VIMatrix44(VIRaster* raster, const VIMatrix44* matrix) {
    sceVu0CopyMatrix(reinterpret_cast<void*>(0x70000040u), matrix);
    unsigned* words = reinterpret_cast<unsigned*>(reinterpret_cast<char*>(raster) + 17888);
    words[1] = 1;
    words[0] = 1;
    return 0;
}
}
