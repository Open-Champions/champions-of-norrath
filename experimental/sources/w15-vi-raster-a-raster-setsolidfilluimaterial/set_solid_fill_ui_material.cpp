// Observed: SetSolidFillUIMaterial__8VIRaster at 0x01113948 is 80 bytes in the pinned SLUS-20565 image. It calls
// AddMaterial__8VIRasteriPP10_sceDmaTag(this, word at +17832, this + 19492), where the last argument is the address of a field (the
// current DMA write pointer slot). If that call returns a negative number the result is -1 and nothing else happens. Otherwise it calls
// FlushDMABufferIfFull__8VIRaster(this), ignores its result, and returns 0.
// Inferred: the word at +17832 is the index of the solid-fill UI material and +19492 is the DMA write pointer used by the UI passes.
// Unknown: the meaning of the +17832 word's value, and AddMaterial's and FlushDMABufferIfFull's behavior (not authored here).
// Nothing here ran on the EE.
extern "C" {
int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* self, int material, unsigned char* writePointer);
void FlushDMABufferIfFull__8VIRaster(unsigned char* self);
int SetSolidFillUIMaterial__8VIRaster(unsigned char* self) {
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(self, *reinterpret_cast<int*>(self + 17832), self + 19492) < 0) return -1;
    FlushDMABufferIfFull__8VIRaster(self);
    return 0;
}
}
