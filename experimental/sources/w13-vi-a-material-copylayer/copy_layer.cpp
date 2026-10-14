// Observed: CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary at 0x010cea18 is 340 bytes.
// Arguments: material (a0), two layer indices (a1 = from, a2 = to), raster (a3) and dictionary
// (t0). Layers are 144-byte records at material+48. If the indices are equal it returns 0
// without any effect. Otherwise, with layer F = from and T = to:
//   T[4] = F[4]; the four bytes at F+8 are copied to T+8 (unaligned word access); T[12] = F[12];
//   __as__10VIMatrix33RC10VIMatrix33(T + 16, F + 16); then the floats at F+52, +56, +60, +64 and
//   +68 are copied to T+52 .. T+68. Then the old surface id T[0] is read, and the new id
//   newId = F[0] is read. If T[0] was not -1 it calls ReleaseSurface__8VIRasteriR12VIDictionary(
//   raster, T[0], dictionary); then T[0] = newId; if newId is not -1 it calls
//   ShareSurface__8VIRasteri(raster, newId). Finally material[32] = 1 and it returns 0.
// Inferred: duplicates one texture layer of a material onto another one, taking a new reference
// on the shared surface and dropping the old one, the same sequencing as SetLayerTexture. The
// operator= call is the matrix assignment; its argument order is destination first.
// Unknown: the meaning of the words at layer+4 and layer+12 and of the floats at +52..+68
// (+52 .. +60 may be a vector, +64/+68 the offset pair that SetNumLayers clears).
extern "C" {
void* __as__10VIMatrix33RC10VIMatrix33(void* destination, const void* source);
void ReleaseSurface__8VIRasteriR12VIDictionary(void* raster, int surface, void* dictionary);
void ShareSurface__8VIRasteri(void* raster, int surface);

int CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned char* material, int from, int to, void* raster, void* dictionary) {
    if (from == to) {
        return 0;
    }
    unsigned char* source = material + 48 + 144 * from;
    unsigned char* target = material + 48 + 144 * to;
    *reinterpret_cast<int*>(target + 4) = *reinterpret_cast<int*>(source + 4);
    for (int i = 0; i < 4; ++i) {
        target[8 + i] = source[8 + i];
    }
    *reinterpret_cast<int*>(target + 12) = *reinterpret_cast<int*>(source + 12);
    __as__10VIMatrix33RC10VIMatrix33(target + 16, source + 16);
    for (int offset = 52; offset <= 68; offset += 4) {
        *reinterpret_cast<float*>(target + offset) = *reinterpret_cast<float*>(source + offset);
    }
    int old = *reinterpret_cast<int*>(target);
    int surface = *reinterpret_cast<int*>(source);
    if (old != -1) {
        ReleaseSurface__8VIRasteriR12VIDictionary(raster, old, dictionary);
    }
    *reinterpret_cast<int*>(target) = surface;
    if (surface != -1) {
        ShareSurface__8VIRasteri(raster, surface);
    }
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
