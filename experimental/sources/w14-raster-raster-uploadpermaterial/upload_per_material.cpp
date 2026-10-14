// Observed: UploadPerMaterial__8VIRasterPP10_sceDmaTag at 0x01112310 is 336 bytes in the pinned SLUS-20565 image. The raster
// (first argument) is not read. The second argument points at the current DMA write pointer. The routine reads four bytes at the fixed
// address 0x70000224 (the EE scratchpad region), converts each to float and multiplies it by 0x3b808080 (a bit pattern near 1/255),
// giving four colour values. With p the write pointer it then writes 64 bytes and advances the pointer by 64:
//   p+0:  byte 3 = 0x10, halfword at 0 = 3, word at +4 = 0 (a DMA tag with QWC 3 and tag id 1; words +8 and +12 are not written)
//   p+16: words 0, 0, 0 and, at +12, (_8VIRaster$ColorMatrix + 2) | 0x6c010000
//   p+32: four floats: colour[i] + the float at 0x700001e0 + 4*i
//   p+48: words 0, 0, 0 and, at +12, _8VIRaster$RMaterialOffset | 0x14000000
// It returns 0.
// Inferred: this uploads the per-material colour (scaled to 0..1 and offset by a scratchpad vector) to VU1 memory through a VIF
// UNPACK at the ColorMatrix offset and then starts the material program with a VIF code of the form 0x14000000 | offset (MSCAL).
// The scratchpad contents are set elsewhere. The two statics are VU memory offsets.
// Unknown: what the scratchpad bytes and floats hold, the roles of the two offsets, and the DMA buffer's capacity. Nothing here ran on
// the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
extern unsigned norrath_raster_color_matrix __asm__("_8VIRaster$ColorMatrix");
extern unsigned norrath_raster_material_offset __asm__("_8VIRaster$RMaterialOffset");

int UploadPerMaterial__8VIRasterPP10_sceDmaTag(void* self, unsigned* writePointer) {
    (void)self;
    float color[4];
    const unsigned char* bytes = NORRATH_WORD_TO_POINTER(0x70000224u);
    const float scale = __builtin_bit_cast(float, 0x3b808080u);
    color[0] = static_cast<float>(bytes[0]) * scale;
    color[1] = static_cast<float>(bytes[1]) * scale;
    color[2] = static_cast<float>(bytes[2]) * scale;
    color[3] = static_cast<float>(bytes[3]) * scale;
    unsigned char* p = NORRATH_WORD_TO_POINTER(*writePointer);
    p[3] = 0x10;
    *reinterpret_cast<unsigned*>(p + 4) = 0;
    *writePointer = *writePointer + 64;
    *reinterpret_cast<unsigned short*>(p) = 3;
    unsigned* vif = reinterpret_cast<unsigned*>(p + 16);
    vif[0] = 0;
    vif[1] = 0;
    vif[2] = 0;
    vif[3] = (norrath_raster_color_matrix + 2) | 0x6c010000u;
    const float* offset = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(0x700001e0u));
    float* data = reinterpret_cast<float*>(p + 32);
    data[0] = color[0] + offset[0];
    data[1] = color[1] + offset[1];
    data[2] = color[2] + offset[2];
    data[3] = color[3] + offset[3];
    unsigned* start = reinterpret_cast<unsigned*>(p + 48);
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = norrath_raster_material_offset | 0x14000000u;
    return 0;
}
}
