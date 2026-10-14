// Observed: PickPrimBuffer__8VIRasterR13VIPickSegmentiii at 0x0111d598 is 424 bytes in the pinned SLUS-20565 image. Arguments: the raster
// (this), a pick segment, a prim-buffer index p, a colour-buffer index c and a material-palette index m. It returns 0, 1 or -1 and is the
// colour-buffer sibling of PickPrimBuffer__8VIRasterR13VIPickSegmentii: it builds the same local segment (construct, invert the matrix at
// scratchpad 0x70000000 into 0x70000100, initialise from the argument segment; negative -> -1), takes pal as the first word of entry m in
// the table at raster+19436, buffer = PrimBuffer__8VIRasteri(raster, p) and colours = ColorBuffer__8VIRasteri(raster, c), and returns 0
// when Cull__8VIRasterR13VIPickSegmentR6VIBBox(raster, local, buffer + 20) is nonzero. Otherwise it fetches the prim buffer again and
// only when its word at +8 equals 2 calls PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal
// (raster, local, buffer, colours, pal); any other mode gives 0. A nonzero picker result becomes 1, the argument segment is
// re-initialised from the local one with the matrix at 0x70000000 (negative -> -1) and the same four-float fix-up as the sibling is applied
// to the segment (adds of the scratchpad floats at 0x70000200..208 to the floats at +76, +80, +84 and a multiply of +88 by the float at
// 0x7000020c).
// Inferred: the colour-buffer variant of the picking test; only one buffer mode has a colour-aware picker.
// Unknown: the segment and buffer layouts, mode names and the scratchpad floats. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
void __13VIPickSegment(unsigned char* self);
void Invert__10VIMatrix44RC10VIMatrix44(unsigned char* destination, unsigned char* source);
int Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44(unsigned char* self, unsigned char* segment, unsigned char* matrix);
unsigned char* PrimBuffer__8VIRasteri(unsigned char* raster, int index);
unsigned char* ColorBuffer__8VIRasteri(unsigned char* raster, int index);
int Cull__8VIRasterR13VIPickSegmentR6VIBBox(unsigned char* raster, unsigned char* segment, unsigned char* bbox);
int PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal(unsigned char*, unsigned char*, unsigned char*,
                                                                                                    unsigned char*, unsigned char*);

int PickPrimBuffer__8VIRasterR13VIPickSegmentiii(unsigned char* self, unsigned char* segment, int primIndex, int colorIndex, int palIndex) {
    unsigned char local[96];
    __13VIPickSegment(local);
    int hit = 0;
    unsigned char* matrix = NORRATH_WORD_TO_POINTER(0x70000000u);
    unsigned char* inverse = NORRATH_WORD_TO_POINTER(0x70000100u);
    Invert__10VIMatrix44RC10VIMatrix44(inverse, matrix);
    if (Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44(local, segment, inverse) < 0) return -1;
    unsigned char* palettes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19436));
    unsigned char* palette = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(palettes + palIndex * 12));
    unsigned char* buffer = PrimBuffer__8VIRasteri(self, primIndex);
    unsigned char* colors = ColorBuffer__8VIRasteri(self, colorIndex);
    if (Cull__8VIRasterR13VIPickSegmentR6VIBBox(self, local, buffer + 20) == 0 &&
        *reinterpret_cast<int*>(PrimBuffer__8VIRasteri(self, primIndex) + 8) == 2) {
        hit = PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal(self, local, buffer, colors, palette);
        if (hit != 0) {
            hit = 1;
            if (Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44(segment, local, matrix) < 0) return -1;
        }
    }
    if (hit != 0) {
        const float* offsets = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(0x70000200u));
        float* a = reinterpret_cast<float*>(segment + 76);
        *a = *a + offsets[0];
        float* b = reinterpret_cast<float*>(segment + 80);
        *b = *b + offsets[1];
        float* c = reinterpret_cast<float*>(segment + 84);
        *c = *c + offsets[2];
        float* d = reinterpret_cast<float*>(segment + 88);
        *d = *d * offsets[3];
    }
    return hit;
}
}
