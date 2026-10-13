// Observed: Init__8VIRaster at 0x0110a250 is 884 bytes (the one-argument member; the
// Init__8VIRasterPviii overload is a different function). It keeps a zeroed 64-byte matrix on its
// stack, passed where noted, whose destructor runs with flag 2 on every exit. In order it:
//   1. Init__13VIEESurfCacheii(&SurfCache, 0x230000, 0x400000); negative -> fail.
//   2. stores 0 at offset 19504, then InitMaterialLRU__8VIRasteri(raster, 850); negative -> fail.
//   3. InitDMADoubleBuffer__8VIRaster(raster); negative -> fail.
//   4. with the cursor slot raster+19492: UploadRasterMicro__8VIRasterPP10_sceDmaTag and
//      InitGSRegisters__8VIRasterPP10_sceDmaTag, both given (raster, slot).
//   5. Identity__10VIMatrix44(matrix); SetDefaultLights__8VIRasterRC10VIMatrix44(raster, matrix).
//   6. SetFog__8VIRasteriffRC10VIColor32F(raster, 1, 250.0f, 1000.0f, color) with color a local
//      array of four floats 1.0f.
//   7. With width w = word at 17748 and height h = word at 17752 (converted to float), stores:
//      17724 = w, 17728 = h, 17716 = 0, 17720 = 0, 17784 = 2048.0f, 17780 = 2048.0f,
//      17756 = 17732 = w * 0.5f, 17760 = 17736 = h * 0.5f, 17764 = 17740 = 1.0f / (w * 0.5f),
//      17768 = 17744 = 1.0f / (h * 0.5f), 17860 = 17804 = 17808 = 17812 = 0, 17816 = 1,
//      the doubleword at 17872 = 0, 17884 = 0, 17888 = 17892 = 17896 = 1; and in absolute
//      memory 0x70000210 .. 0x7000021c = four floats 0.0f, 0x70000228 = 1.0f, 0x7000022c = the
//      float with bits 0x3e199999.
//   8. Identity__10VIMatrix44 on 0x70000000 and on 0x70000040, then CalcClipToScreen__8VIRaster,
//      CalcMatrices__8VIRaster and UploadMatrices__8VIRasterPP10_sceDmaTag(raster, slot).
//   9. Each of these returning negative fails: Init__t6VIPool1ZP12VIPrimBufferi(raster+19320, 0),
//      Init__t6VIList1ZP12VIPrimBuffer(raster+19344), Init__t6VIPool1ZP9VISurfacei(raster+19380, 0),
//      Init__t6VIList1ZP9VISurface(raster+19404), Init__t6VIList1Z10VIMaterial(raster+19416),
//      Init__t6VIPool1ZP13VIMaterialPali(raster+19428, 0), CreateDefaultResources__8VIRaster(raster),
//      SetSolidFillMaterial__8VIRaster(raster). Success stores 1 at offset 60.
//  10. Init__12VIRasterTess(raster+64); float zero at 17544; SetClipAndLight__8VIRasteri(raster, 1);
//      Flush__8VIRaster(raster).
//  11. If the byte at 19536 is zero it does three rounds of sceGsSyncV(0); counter = word at 17880;
//      store counter + 1 at 17880; sceGsSwapDBuffDc(&_8VIRaster$DBuffer, counter) (the new counter
//      is already stored when the swap runs); 64-bit store of 32544 to 0x12000000.
//  12. Result zero; a failure returns -1.
// Inferred: 17724/17728 are the viewport size, 17732..17768 are its half-size and reciprocals as
// used by Vertex__8VIRasterRC8VIVect2I, and 17748/17752 hold the display size that the Pviii
// overload stores (640 x 448). The scratchpad stores prepare lighting/clip constants.
// Unknown: the meanings of 17780 and 17784 (2048.0f), 17816, 17860, 17896, 0x7000022c and of the
// 850 passed to InitMaterialLRU.
extern "C" {
struct VIRaster;

struct VIMatrix44 {
    float m[16];
};

extern unsigned char norrath_surf_cache[] __asm__("SurfCache");
extern unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");
struct sceGsDBuffDc;

int Init__13VIEESurfCacheii(void* cache, int a, int b);
int InitMaterialLRU__8VIRasteri(VIRaster* raster, int count);
int InitDMADoubleBuffer__8VIRaster(VIRaster* raster);
int UploadRasterMicro__8VIRasterPP10_sceDmaTag(VIRaster* raster, void* slot);
int InitGSRegisters__8VIRasterPP10_sceDmaTag(VIRaster* raster, void* slot);
void Identity__10VIMatrix44(void* matrix);
int SetDefaultLights__8VIRasterRC10VIMatrix44(VIRaster* raster, const void* matrix);
int SetFog__8VIRasteriffRC10VIColor32F(VIRaster* raster, int mode, float near, float far, const void* color);
int CalcClipToScreen__8VIRaster(VIRaster* raster);
int CalcMatrices__8VIRaster(VIRaster* raster);
int UploadMatrices__8VIRasterPP10_sceDmaTag(VIRaster* raster, void* slot);
int Init__t6VIPool1ZP12VIPrimBufferi(void* pool, int count);
int Init__t6VIList1ZP12VIPrimBuffer(void* list);
int Init__t6VIPool1ZP9VISurfacei(void* pool, int count);
int Init__t6VIList1ZP9VISurface(void* list);
int Init__t6VIList1Z10VIMaterial(void* list);
int Init__t6VIPool1ZP13VIMaterialPali(void* pool, int count);
int CreateDefaultResources__8VIRaster(VIRaster* raster);
int SetSolidFillMaterial__8VIRaster(VIRaster* raster);
int Init__12VIRasterTess(void* tess);
int SetClipAndLight__8VIRasteri(VIRaster* raster, int flag);
int Flush__8VIRaster(VIRaster* raster);
int sceGsSyncV(int mode);
void sceGsSwapDBuffDc(sceGsDBuffDc* db, int id);
void _$_10VIMatrix44(VIMatrix44* matrix, int flags);

static int finish(VIMatrix44* matrix, int result) {
    _$_10VIMatrix44(matrix, 2);
    return result;
}

int Init__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    VIMatrix44 matrix;
    for (int i = 0; i < 16; ++i) matrix.m[i] = 0.0f;
    if (Init__13VIEESurfCacheii(norrath_surf_cache, 0x230000, 0x400000) < 0) return finish(&matrix, -1);
    *reinterpret_cast<int*>(base + 19504) = 0;
    if (InitMaterialLRU__8VIRasteri(raster, 850) < 0) return finish(&matrix, -1);
    if (InitDMADoubleBuffer__8VIRaster(raster) < 0) return finish(&matrix, -1);
    void* slot = base + 19492;
    UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, slot);
    InitGSRegisters__8VIRasterPP10_sceDmaTag(raster, slot);
    Identity__10VIMatrix44(&matrix);
    SetDefaultLights__8VIRasterRC10VIMatrix44(raster, &matrix);
    float color[4];
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    color[3] = 1.0f;
    SetFog__8VIRasteriffRC10VIColor32F(raster, 1, 250.0f, 1000.0f, color);

    int width = *reinterpret_cast<int*>(base + 17748);
    int height = *reinterpret_cast<int*>(base + 17752);
    float halfWidth = static_cast<float>(width) * 0.5f;
    float halfHeight = static_cast<float>(height) * 0.5f;
    *reinterpret_cast<int*>(base + 17724) = width;
    *reinterpret_cast<float*>(base + 17784) = 2048.0f;
    *reinterpret_cast<float*>(base + 17780) = 2048.0f;
    *reinterpret_cast<int*>(base + 17728) = height;
    *reinterpret_cast<int*>(base + 17716) = 0;
    *reinterpret_cast<int*>(base + 17720) = 0;
    float reciprocalWidth = 1.0f / halfWidth;
    *reinterpret_cast<float*>(base + 17756) = halfWidth;
    *reinterpret_cast<float*>(base + 17760) = halfHeight;
    *reinterpret_cast<float*>(base + 17736) = halfHeight;
    *reinterpret_cast<float*>(base + 17732) = halfWidth;
    *reinterpret_cast<int*>(base + 17860) = 0;
    *reinterpret_cast<int*>(base + 17804) = 0;
    float reciprocalHeight = 1.0f / halfHeight;
    *reinterpret_cast<int*>(base + 17808) = 0;
    *reinterpret_cast<int*>(base + 17812) = 0;
    *reinterpret_cast<float*>(base + 17764) = reciprocalWidth;
    *reinterpret_cast<float*>(base + 17740) = reciprocalWidth;
    *reinterpret_cast<int*>(base + 17816) = 1;
    *reinterpret_cast<long long*>(base + 17872) = 0;
    *reinterpret_cast<int*>(base + 17884) = 0;
    *reinterpret_cast<int*>(base + 17888) = 1;
    *reinterpret_cast<float*>(base + 17768) = reciprocalHeight;
    *reinterpret_cast<float*>(base + 17744) = reciprocalHeight;
    *reinterpret_cast<int*>(base + 17892) = 1;
    volatile float* scratch = reinterpret_cast<volatile float*>(0x70000210u);
    scratch[3] = 0.0f;
    scratch[0] = 0.0f;
    scratch[1] = 0.0f;
    scratch[2] = 0.0f;
    *reinterpret_cast<int*>(base + 17896) = 1;
    volatile float* lighting = reinterpret_cast<volatile float*>(0x70000200u);
    lighting[11] = __builtin_bit_cast(float, 0x3e199999u);
    lighting[10] = 1.0f;
    Identity__10VIMatrix44(reinterpret_cast<void*>(0x70000000u));
    Identity__10VIMatrix44(reinterpret_cast<void*>(0x70000040u));
    CalcClipToScreen__8VIRaster(raster);
    CalcMatrices__8VIRaster(raster);
    UploadMatrices__8VIRasterPP10_sceDmaTag(raster, slot);
    if (Init__t6VIPool1ZP12VIPrimBufferi(base + 19320, 0) < 0) return finish(&matrix, -1);
    if (Init__t6VIList1ZP12VIPrimBuffer(base + 19344) < 0) return finish(&matrix, -1);
    if (Init__t6VIPool1ZP9VISurfacei(base + 19380, 0) < 0) return finish(&matrix, -1);
    if (Init__t6VIList1ZP9VISurface(base + 19404) < 0) return finish(&matrix, -1);
    if (Init__t6VIList1Z10VIMaterial(base + 19416) < 0) return finish(&matrix, -1);
    if (Init__t6VIPool1ZP13VIMaterialPali(base + 19428, 0) < 0) return finish(&matrix, -1);
    if (CreateDefaultResources__8VIRaster(raster) < 0) return finish(&matrix, -1);
    if (SetSolidFillMaterial__8VIRaster(raster) < 0) return finish(&matrix, -1);
    *reinterpret_cast<int*>(base + 60) = 1;
    Init__12VIRasterTess(base + 64);
    *reinterpret_cast<float*>(base + 17544) = 0.0f;
    SetClipAndLight__8VIRasteri(raster, 1);
    Flush__8VIRaster(raster);
    if (*reinterpret_cast<unsigned char*>(base + 19536) == 0) {
        int* counter = reinterpret_cast<int*>(base + 17880);
        for (int round = 0; round < 3; ++round) {
            sceGsSyncV(0);
            int id = *counter;
            *counter = id + 1;
            sceGsSwapDBuffDc(reinterpret_cast<sceGsDBuffDc*>(norrath_raster_dbuffer), id);
            *reinterpret_cast<volatile unsigned long long*>(0x12000000u) = 32544ull;
        }
    }
    return finish(&matrix, 0);
}
}
