// Observed: PickPrimBuffer__8VIRasterR13VIPickSegmentii at 0x0111d380 is 532 bytes in the pinned SLUS-20565 image. Arguments: the raster
// (this), a pick segment, a prim-buffer index p and a material-palette index m. It returns 0 (no hit), 1 (hit) or -1. Steps:
//   1. construct a local pick segment (__13VIPickSegment), invert the matrix at scratchpad 0x70000000 into 0x70000100
//      (Invert__10VIMatrix44RC10VIMatrix44) and initialise the local from the argument segment and the inverse
//      (Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44); a negative result returns -1;
//   2. pal = the first word of the 12-byte entry m in the table at raster+19436; buffer = PrimBuffer__8VIRasteri(raster, p);
//   3. if Cull__8VIRasterR13VIPickSegmentR6VIBBox(raster, local, buffer + 20) is nonzero the result is 0;
//   4. otherwise the buffer's mode is the word at +8 of PrimBuffer__8VIRasteri(raster, p) (fetched again): mode 0 calls
//      PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal, 2 calls the PackVUNC variant, 4 the PackVUNCG variant,
//      5 the PackVUNBW variant (each as (raster, local, buffer, pal)); any other mode gives 0;
//   5. if the picker returned nonzero the result is 1 and the argument segment is re-initialised from the local one with the matrix at
//      0x70000000 (Init__13VIPickSegment..., a negative result returns -1), then the segment's floats at +76, +80 and +84 get the scratchpad
//      floats at 0x70000200, 0x70000204 and 0x70000208 added and the float at +88 is multiplied by the one at 0x7000020c.
// Inferred: this tests a pick ray against one primitive buffer in its own space (the local segment is the ray transformed by the inverse
// matrix) and, on a hit, maps the result back and adjusts the hit record; the four picker variants match the buffer's vertex formats.
// Unknown: the layouts of the pick segment (its local copy is at least 92 bytes; 96 is reserved here), the VIPrimBuffer mode names, and
// what the scratchpad floats are. Nothing here ran on the EE.
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
int Cull__8VIRasterR13VIPickSegmentR6VIBBox(unsigned char* raster, unsigned char* segment, unsigned char* bbox);
int PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char*, unsigned char*, unsigned char*, unsigned char*);
int PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char*, unsigned char*, unsigned char*, unsigned char*);
int PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char*, unsigned char*, unsigned char*, unsigned char*);
int PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char*, unsigned char*, unsigned char*, unsigned char*);

int PickPrimBuffer__8VIRasterR13VIPickSegmentii(unsigned char* self, unsigned char* segment, int primIndex, int palIndex) {
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
    if (Cull__8VIRasterR13VIPickSegmentR6VIBBox(self, local, buffer + 20) == 0) {
        switch (*reinterpret_cast<int*>(PrimBuffer__8VIRasteri(self, primIndex) + 8)) {
            case 0: hit = PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(self, local, buffer, palette); break;
            case 2: hit = PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(self, local, buffer, palette); break;
            case 4: hit = PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(self, local, buffer, palette); break;
            case 5: hit = PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(self, local, buffer, palette); break;
            default: break;
        }
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
