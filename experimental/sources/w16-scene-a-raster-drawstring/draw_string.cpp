// Observed: DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs at 0x0110e750 is 748 bytes in the pinned SLUS-20565 image. All offsets are
// bytes. Arguments: raster, font index, position (two ints x, y), colour (16 bytes, four words), text (16-bit units).
//   font = first word of the 12-byte slot [index] of the table whose pointer is the word at raster + 19516.
//   n = Cache__7VIRFontR8VIRasterPCUsiPi(font, raster, text, 256, raster + 17900): the glyph indices for the text go to the int array at
//   raster + 17900; n < 0 returns -1. AddMaterial__8VIRasteriPP10_sceDmaTag(raster, word at font + 16, raster + 19492) < 0 returns -1.
//   The cursor starts at (x, y); the bottom edge is y + the word at font + 20. If n <= 0 it returns 0.
//   For each of the n glyph indices g (read from the int array in order):
//     if the word at raster + 19532 is not 1: UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster + 19492);
//     if the word at raster + 17888 is non-zero: CalcMatrices__8VIRaster(raster), then UploadMatrices__8VIRasterPP10_sceDmaTag(raster, raster + 19492);
//     tag = the word at raster + 19492; raster word +17884 = 2, +48 = 0, +56 = tag + 16, +52 = tag + 48; the 16 colour bytes are copied to raster + 32.
//     rec = (word at font + 64) + 12 * g; slot = (word at font + 68) + 56 * (halfword at rec + 8); adv = the byte at rec + 10.
//     Four vertices, each preceded by the two floats of the slot being copied to raster + 12 and raster + 16 (the current texture coordinate),
//     then Vertex__8VIRasterRC8VIVect2I(raster, &point): point (cursor x, y) with slot floats +24, +28; (cursor x + adv, y) with +32, +36;
//     (cursor x + adv, bottom) with +40, +44; (cursor x, bottom) with +48, +52. Then EndTriFan__8VIRaster(raster); the cursor x becomes
//     cursor x + adv + 1.
//   It returns 0.
// Inferred: this draws a string as one textured quad per glyph through the raster's immediate-mode triangle-fan path; the stores to
// +17884, +48, +56 and +52 stand in for a begin-fan call (no BeginTriFan call appears); the slot floats are a glyph's four texture corners
// in a font texture (InitSlots__7VIRFontR8VIRaster builds the slot table whose records start with +0 offset, +16/+20 position).
// Unknown: the meaning of the raster words listed, the tag bookkeeping, and the callees' behaviour (all imports). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int Cache__7VIRFontR8VIRasterPCUsiPi(unsigned char* font, unsigned char* raster, const unsigned short* text, int limit, int* out);
int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* raster, int material, unsigned* tagSlot);
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned* tagSlot);
void CalcMatrices__8VIRaster(unsigned char* raster);
void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned* tagSlot);
void Vertex__8VIRasterRC8VIVect2I(unsigned char* raster, const int* point);
void EndTriFan__8VIRaster(unsigned char* raster);

int DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs(unsigned char* raster, int fontIndex, const int* position, const unsigned char* color,
                                                        const unsigned short* text) {
    unsigned char* fontTable = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(raster + 19516));
    unsigned char* font = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(fontTable + fontIndex * 12));
    int* glyphs = reinterpret_cast<int*>(raster + 17900);
    unsigned* tagSlot = reinterpret_cast<unsigned*>(raster + 19492);
    int count = Cache__7VIRFontR8VIRasterPCUsiPi(font, raster, text, 256, glyphs);
    if (count < 0) return -1;
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, *reinterpret_cast<int*>(font + 16), tagSlot) < 0) return -1;
    int cursor[2];
    cursor[0] = position[0];
    cursor[1] = position[1];
    int corner[2];
    corner[0] = position[0];
    corner[1] = position[1] + *reinterpret_cast<int*>(font + 20);
    if (count <= 0) return 0;
    int remaining = count;
    const int* next = glyphs;
    do {
        if (*reinterpret_cast<int*>(raster + 19532) != 1) UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, tagSlot);
        if (*reinterpret_cast<int*>(raster + 17888) != 0) {
            CalcMatrices__8VIRaster(raster);
            UploadMatrices__8VIRasterPP10_sceDmaTag(raster, tagSlot);
        }
        unsigned tag = *tagSlot;
        *reinterpret_cast<int*>(raster + 17884) = 2;
        *reinterpret_cast<int*>(raster + 48) = 0;
        *reinterpret_cast<unsigned*>(raster + 56) = tag;
        --remaining;
        int g = *next;
        unsigned top = *reinterpret_cast<unsigned*>(raster + 56) + 16;
        *reinterpret_cast<unsigned*>(raster + 56) = top;
        *reinterpret_cast<unsigned*>(raster + 52) = top + 32;
        unsigned char* record = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 64)) + g * 12;
        unsigned slotIndex = *reinterpret_cast<unsigned short*>(record + 8);
        for (int i = 0; i < 16; ++i) raster[32 + i] = color[i];
        int slotOffset = slotIndex * 56;
        g = *next;
        ++next;
        record = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 64)) + g * 12;
        unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 68)) + slotOffset;
        int advance = record[10];
        float* texture = reinterpret_cast<float*>(raster + 12);
        texture[0] = *reinterpret_cast<float*>(slot + 24);
        corner[0] = cursor[0] + advance;
        texture[1] = *reinterpret_cast<float*>(slot + 28);
        Vertex__8VIRasterRC8VIVect2I(raster, cursor);
        slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 68)) + slotOffset;
        int second[2];
        second[0] = corner[0];
        second[1] = cursor[1];
        texture[0] = *reinterpret_cast<float*>(slot + 32);
        texture[1] = *reinterpret_cast<float*>(slot + 36);
        Vertex__8VIRasterRC8VIVect2I(raster, second);
        slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 68)) + slotOffset;
        texture[0] = *reinterpret_cast<float*>(slot + 40);
        texture[1] = *reinterpret_cast<float*>(slot + 44);
        Vertex__8VIRasterRC8VIVect2I(raster, corner);
        slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 68)) + slotOffset;
        texture[0] = *reinterpret_cast<float*>(slot + 48);
        texture[1] = *reinterpret_cast<float*>(slot + 52);
        int fourth[2];
        fourth[0] = cursor[0];
        fourth[1] = corner[1];
        Vertex__8VIRasterRC8VIVect2I(raster, fourth);
        EndTriFan__8VIRaster(raster);
        cursor[0] = corner[0] + 1;
    } while (remaining != 0);
    return 0;
}
}
