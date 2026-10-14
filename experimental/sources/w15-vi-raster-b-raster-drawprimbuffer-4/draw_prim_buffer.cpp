// Observed: DrawPrimBuffer__8VIRasteriiiUi at 0x01113560 is 240 bytes in the pinned SLUS-20565 image. Arguments: the raster (this), a
// slot index, two ints and an unsigned. It loads the primitive buffer pointer from the 12-byte slot at that index of the table whose
// pointer is the raster word at +19328 (0x4b80). If the raster word at +19532 (0x4c4c) is not 1 it first calls
// UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster + 19492). It then reads the buffer's word at +8: only the value 2 leads
// to a draw, as a tail call to DrawPrimBufferPackVUNC__8VIRasteriiiUi(raster, index, int, int, unsigned) whose result is returned
// unchanged; every other value (the image tests ranges 3-5 and 6 and above separately but they all end in the same -1) returns -1.
// Inferred: this is the four-argument sibling of the three-argument DrawPrimBuffer; only the packed layout with value 2 exists here.
// Unknown: the meaning of the extra int, the callees' behaviour (unresolved imports here), and the raster words. Nothing here ran on
// the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
int DrawPrimBufferPackVUNC__8VIRasteriiiUi(unsigned char* raster, int index, int second, int third, unsigned fourth);
int DrawPrimBuffer__8VIRasteriiiUi(unsigned char* self, int index, int second, int third, unsigned fourth) {
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19328));
    unsigned bufferWord = *reinterpret_cast<unsigned*>(table + index * 12);
    if (*reinterpret_cast<int*>(self + 19532) != 1) {
        UploadRasterMicro__8VIRasterPP10_sceDmaTag(self, self + 19492);
    }
    unsigned char* buffer = NORRATH_WORD_TO_POINTER(bufferWord);
    if (*reinterpret_cast<int*>(buffer + 8) == 2) {
        return DrawPrimBufferPackVUNC__8VIRasteriiiUi(self, index, second, third, fourth);
    }
    return -1;
}
}
