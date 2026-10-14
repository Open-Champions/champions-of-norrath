// Observed: CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi at 0x01134800 is 532 bytes in the pinned SLUS-20565 image.
// Offsets written +N are byte offsets from this (words); surface+N from the surface. Arguments: this, raster, dictionary, an int that is never read (Init passes its `size` there), extra, bitDepth, last. It stops with
// -1 at the first failing step; stores made before the failure stay. In order:
//   id = CreateSurface__8VIRasteri(raster, 0); +12 = id (stored even if negative); if id < 0 return -1.
//   if ShareSurface__8VIRasteri(raster, id) < 0 return -1.
//   cell = max(extra, +24); +48 = (256 / cell) * (256 / extra) (signed); if +48 <= 0 return -1 (the store happens first).
//     A zero `cell` traps (break 7); a zero `extra` with a non-zero `cell` does NOT trap, because the code issues the 256 / extra divide
//     without a zero check and the R5900 divide-by-zero result (quotient -1 for a non-negative dividend) is used, so +48 <= 0.
//   surface = Surface__8VIRasteri(raster, +12);
//   Init__9VISurfaceR8VIRasterii10VIBitDepthi(surface, raster, 256, 256, bitDepth, 1); if negative return -1.
//   +28 = surface+16; if surface+40 == 0: +52 = 0; else +32 = surface+52 and +52 = 16 if surface+16 == 0, 256 if
//   surface+16 == 1, and any other surface+16 returns -1.
//   mid = CreateMaterial__8VIRasteri(raster, 0); +16 = mid (stored even if negative); if mid < 0 return -1.
//   if ShareMaterial__8VIRasteri(raster, mid) < 0 return -1.
//   material = Material__8VIRasteri(raster, +16); Init__10VIMateriali(material, 1);
//   SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, +12, raster, dictionary);
//   SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(material, 0, 3);
//   SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, last ? 0 : 3);
//   SetLayerColor__10VIMaterialiRC9VIColor32(material, 0, &color) with color four bytes of 0xff; SetDither__10VIMateriali(material, 0); return 0.
// Inferred: the loader makes a 256x256 texture surface and a one-layer material for a font; +48 counts cells in that texture (256 /
// widest cell times 256 / cell extent); +28, +32 and +52 copy format fields of the surface. 0 and 1 for surface+16 look like
// the 4- and 8-bit palettised formats (16 and 256 palette entries), which fits Init__7VIRFont's use of the pixel-store enum.
// Unknown: the three raster calls' exact argument types (the second argument of the Share calls is whatever the previous call returned),
// the VIAlphaBlendMode and VIMaterialWrapMode value names, and every callee's behaviour (all imports). Nothing here ran on the EE.
extern "C" {
int CreateSurface__8VIRasteri(unsigned char* raster, int arg);
int ShareSurface__8VIRasteri(unsigned char* raster, int id);
unsigned char* Surface__8VIRasteri(unsigned char* raster, int id);
int Init__9VISurfaceR8VIRasterii10VIBitDepthi(unsigned char* surface, unsigned char* raster, int width, int height, int bitDepth, int flag);
int CreateMaterial__8VIRasteri(unsigned char* raster, int arg);
int ShareMaterial__8VIRasteri(unsigned char* raster, int id);
unsigned char* Material__8VIRasteri(unsigned char* raster, int id);
void Init__10VIMateriali(unsigned char* material, int arg);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned char* material, int layer, int surfaceId, unsigned char* raster, unsigned char* dictionary);
void SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(unsigned char* material, int layer, int mode);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(unsigned char* material, int layer, int mode);
void SetLayerColor__10VIMaterialiRC9VIColor32(unsigned char* material, int layer, const unsigned char* color);
void SetDither__10VIMateriali(unsigned char* material, int arg);

// The divide below is the R5900 hardware result for a zero divisor (quotient -1 for a non-negative dividend); the original issues it
// without the usual zero check.
static inline int divide_unchecked(int a, int b) {
    if (b == 0) return a < 0 ? 1 : -1;
    return a / b;
}

int CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi(unsigned char* self, unsigned char* raster, unsigned char* dictionary,
                                                                     int, int extra, int bitDepth, int last) {
    int* f = reinterpret_cast<int*>(self);
    int id = CreateSurface__8VIRasteri(raster, 0);
    f[3] = id;
    if (id < 0) return -1;
    if (ShareSurface__8VIRasteri(raster, id) < 0) return -1;
    int widest = f[6];
    int cell = extra < widest ? widest : extra;
    int perRow = divide_unchecked(256, extra);
    int perColumn = 256 / cell;
    f[12] = perColumn * perRow;
    if (f[12] <= 0) return -1;
    unsigned char* surface = Surface__8VIRasteri(raster, f[3]);
    if (Init__9VISurfaceR8VIRasterii10VIBitDepthi(surface, raster, 256, 256, bitDepth, 1) < 0) return -1;
    int format = *reinterpret_cast<int*>(surface + 16);
    f[7] = format;
    if (*reinterpret_cast<int*>(surface + 40) == 0) {
        f[13] = 0;
    } else {
        f[8] = *reinterpret_cast<int*>(surface + 52);
        if (format == 0) f[13] = 16;
        else if (format == 1) f[13] = 256;
        else return -1;
    }
    int materialId = CreateMaterial__8VIRasteri(raster, 0);
    f[4] = materialId;
    if (materialId < 0) return -1;
    if (ShareMaterial__8VIRasteri(raster, materialId) < 0) return -1;
    unsigned char* material = Material__8VIRasteri(raster, f[4]);
    Init__10VIMateriali(material, 1);
    SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, 0, f[3], raster, dictionary);
    SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(material, 0, 3);
    SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, last != 0 ? 0 : 3);
    unsigned char color[4] = {0xff, 0xff, 0xff, 0xff};
    SetLayerColor__10VIMaterialiRC9VIColor32(material, 0, color);
    SetDither__10VIMateriali(material, 0);
    return 0;
}
}
