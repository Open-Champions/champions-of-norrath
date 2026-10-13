// Observed: Begin2D__8VIRaster at 0x0110d2c0 is 212 bytes. It builds a 64-byte zeroed matrix on
// its stack (the image zeroes it twice, with word stores and then with vector stores). If the
// word at offset 19532 of the raster is not 1 it calls UploadRasterMicro__8VIRasterPP10_sceDmaTag
// (raster, raster+19492). It then calls Store2D__8VIRaster(raster) and Identity__10VIMatrix44 on
// the stack matrix, copies that matrix to 0x70000000 with sceVu0CopyMatrix, stores the float with
// bit pattern 0xbf7fffef at word index 14 of the matrix, copies the matrix again to 0x70000040,
// sets the word at offset 17888 of the raster to 1 and runs the matrix destructor with flag 2.
// The result is zero.
// Inferred: the two scratchpad copies are the model-view and projection matrices (as in
// SetModelView and SetProjection) and the second one is an orthographic-style projection whose
// word 14 is just under -1; 17888 is a dirty flag.
// Unknown: the numeric value of the float beyond its bit pattern, and the names of the callees'
// effects.
extern "C" {
struct VIRaster;

struct VIMatrix44 {
    float m[16];
};

int UploadRasterMicro__8VIRasterPP10_sceDmaTag(VIRaster* raster, void* tagPointer);
int Store2D__8VIRaster(VIRaster* raster);
void Identity__10VIMatrix44(VIMatrix44* matrix);
void sceVu0CopyMatrix(void* destination, const void* source);
void _$_10VIMatrix44(VIMatrix44* matrix, int flags);

int Begin2D__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    VIMatrix44 matrix;
    for (int i = 0; i < 16; ++i) matrix.m[i] = 0.0f;
    if (*reinterpret_cast<int*>(base + 19532) != 1)
        UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, base + 19492);
    Store2D__8VIRaster(raster);
    Identity__10VIMatrix44(&matrix);
    sceVu0CopyMatrix(reinterpret_cast<void*>(0x70000000u), &matrix);
    matrix.m[14] = __builtin_bit_cast(float, 0xbf7fffefu);
    sceVu0CopyMatrix(reinterpret_cast<void*>(0x70000040u), &matrix);
    *reinterpret_cast<int*>(base + 17888) = 1;
    _$_10VIMatrix44(&matrix, 2);
    return 0;
}
}
