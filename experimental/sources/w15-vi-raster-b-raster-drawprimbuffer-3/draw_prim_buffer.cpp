// Observed: DrawPrimBuffer__8VIRasteriiUi at 0x0110d6d0 is 412 bytes in the pinned SLUS-20565 image. Arguments: the raster (this), a
// slot index, an int and an unsigned. It loads the primitive buffer pointer from the 12-byte slot at that index of the table whose
// pointer is the raster word at +19328 (0x4b80). If the raster word at +19532 (0x4c4c) is not 1 it first calls
// UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster + 19492). It then switches on the buffer's word at +8 (unsigned):
//   0: tail call DrawPrimBufferVUNC__8VIRasteriiUi(raster, index, int, unsigned)
//   2: tail call DrawPrimBufferPackVUNC__8VIRasteriiUi
//   4: tail call DrawPrimBufferPackVUNCG__8VIRasteriiUi
//   5: tail call DrawPrimBufferPackVUNBW__8VIRasteriiUi
//   8: tail call DrawPrimBufferPackVU__8VIRasteriifUi with the float 1.0 in the first float argument register
//   1, 3, 6, 7 and anything above 8: return -1 without drawing.
// The jump table was read from the image (0x011d9590); the tail calls return their result unchanged.
// Inferred: the word at +8 selects the vertex layout of the buffer (the callee names suggest packed and unpacked vertex-unit formats,
// with or without colour, gouraud or bone weights); the upload call loads the raster microcode when it is not the current one, and
// the raster word at +19532 says which is current.
// Unknown: the exact meaning of each layout, why the float is fixed at 1.0, the callees' behaviour (unresolved imports here), and the
// meaning of the two raster words. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
int DrawPrimBufferVUNC__8VIRasteriiUi(unsigned char* raster, int index, int second, unsigned third);
int DrawPrimBufferPackVUNC__8VIRasteriiUi(unsigned char* raster, int index, int second, unsigned third);
int DrawPrimBufferPackVUNCG__8VIRasteriiUi(unsigned char* raster, int index, int second, unsigned third);
int DrawPrimBufferPackVUNBW__8VIRasteriiUi(unsigned char* raster, int index, int second, unsigned third);
int DrawPrimBufferPackVU__8VIRasteriifUi(unsigned char* raster, int index, int second, float scale, unsigned third);
int DrawPrimBuffer__8VIRasteriiUi(unsigned char* self, int index, int second, unsigned third) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19328));
    unsigned bufferWord = *reinterpret_cast<unsigned*>(table + index * 12);
    if (*reinterpret_cast<int*>(self + 19532) != 1) {
        UploadRasterMicro__8VIRasterPP10_sceDmaTag(self, self + 19492);
    }
    unsigned char* buffer = NORRATH_WORD_TO_POINTER(bufferWord);
    switch (*reinterpret_cast<unsigned*>(buffer + 8)) {
    case 0:
        return DrawPrimBufferVUNC__8VIRasteriiUi(self, index, second, third);
    case 2:
        return DrawPrimBufferPackVUNC__8VIRasteriiUi(self, index, second, third);
    case 4:
        return DrawPrimBufferPackVUNCG__8VIRasteriiUi(self, index, second, third);
    case 5:
        return DrawPrimBufferPackVUNBW__8VIRasteriiUi(self, index, second, third);
    case 8:
        return DrawPrimBufferPackVU__8VIRasteriifUi(self, index, second, 1.0f, third);
    default:
        return -1;
    }
}
}
