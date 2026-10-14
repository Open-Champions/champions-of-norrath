// Observed: DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi at 0x0110ea40 is 948 bytes in the pinned SLUS-20565 image.
// Arguments: the raster (this), a font slot index, a pointer to two ints (x, y), a pointer to a 16-byte colour, a string and a mode.
// The font is the first word of the 12-byte slot at that index in the table pointed to by the raster word at +19516 (0x4c3c).
//  1. Cache__7VIRFontR8VIRasterPCciPi(font, raster, string, 256, raster + 17900) returns a glyph count n (negative -> return -1) and
//     fills n glyph numbers at raster +17900. AddMaterial__8VIRasteriPP10_sceDmaTag(raster, font word +16, raster + 19492) (negative
//     -> -1).
//  2. width = sum over the n glyphs of (byte at +10 of the 12-byte record glyph * 12 in the table whose pointer is font word +64) + 1.
//     (x, y) come from the position. Mode 0 or 3: x -= width >> 1 and y -= (font word +20) >> 1; mode 1: only y; mode 2: only x; any
//     other mode returns -1 before drawing. The cursor x is then the left edge and bottom = y + font word +20.
//  3. For each glyph in order: if the raster word at +19532 is not 1, UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster+19492);
//     if the raster word at +17888 is non-zero, CalcMatrices__8VIRaster(raster) and
//     UploadMatrices__8VIRasterPP10_sceDmaTag(raster, raster + 19492). Then raster +17884 = 2, +48 = 0, +56 = the raster word at
//     +19492 plus 16, +52 = that plus 32 (as one running value read back after the first store), and the 16 colour bytes are copied to
//     raster +32. The glyph's halfword at +8 of its 12-byte record (call it g) selects a 56-byte quad record of the table whose
//     pointer is font word +68. Four corner coordinates are given to Vertex__8VIRasterRC8VIVect2I(raster, int pair) in the order
//     (left, top), (right, top), (right, bottom), (left, bottom) with right = left + advance (the record byte +10), after copying the
//     two float words at quad +24, +32, +40, +48 to raster +12/+16 before each call; then EndTriFan__8VIRaster(raster). The next
//     glyph starts one pixel right of the previous right edge. It returns 0 (also when n is zero).
// Inferred: the quad record holds four (u, v) pairs at +24..+55, the words at raster +12/+16 are the current texture coordinate, +32
// the current colour and +17884 a primitive mode (2 = triangle fan); the width loop's +1 is a one-pixel gap between glyphs.
// Unknown: the layouts of the font, the glyph record and the quad record beyond these words, the meaning of raster +48, +52, +56 and
// +17888, and all callees (imports of this unit). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int Cache__7VIRFontR8VIRasterPCciPi(unsigned char* font, unsigned char* raster, const char* text, int limit, int* glyphs);
int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* raster, int material, unsigned char* tagSlot);
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
void CalcMatrices__8VIRaster(unsigned char* raster);
void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
void Vertex__8VIRasterRC8VIVect2I(unsigned char* raster, const int* point);
void EndTriFan__8VIRaster(unsigned char* raster);
int DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(unsigned char* self, int fontIndex, const int* position, const unsigned* color, const char* text, unsigned mode) {
    int origin[2] = {0, 0};
    int right[2] = {0, 0};
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19516));
    unsigned char* font = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + fontIndex * 12));
    int* glyphs = reinterpret_cast<int*>(self + 17900);
    int count = Cache__7VIRFontR8VIRasterPCciPi(font, self, text, 256, glyphs);
    if (count < 0) return -1;
    unsigned char* tagSlot = self + 19492;
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(self, *reinterpret_cast<int*>(font + 16), tagSlot) < 0) return -1;
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 64));
    int width = 0;
    for (int i = 0; i < count; ++i) {
        width += records[glyphs[i] * 12 + 10] + 1;
    }
    origin[0] = position[0];
    origin[1] = position[1];
    int height = *reinterpret_cast<int*>(font + 20);
    switch (mode) {
    case 1:
        origin[1] -= height >> 1;
        break;
    case 0:
    case 3:
        origin[0] -= width >> 1;
        origin[1] -= height >> 1;
        break;
    case 2:
        origin[0] -= width >> 1;
        break;
    default:
        return -1;
    }
    right[0] = origin[0];
    right[1] = origin[1];
    right[1] += height;
    for (int i = 0; i < count; ++i) {
        if (*reinterpret_cast<int*>(self + 19532) != 1) {
            UploadRasterMicro__8VIRasterPP10_sceDmaTag(self, tagSlot);
        }
        if (*reinterpret_cast<int*>(self + 17888) != 0) {
            CalcMatrices__8VIRaster(self);
            UploadMatrices__8VIRasterPP10_sceDmaTag(self, tagSlot);
        }
        *reinterpret_cast<int*>(self + 17884) = 2;
        *reinterpret_cast<int*>(self + 48) = 0;
        *reinterpret_cast<unsigned*>(self + 56) = *reinterpret_cast<unsigned*>(self + 19492);
        unsigned running = *reinterpret_cast<unsigned*>(self + 56) + 16;
        *reinterpret_cast<unsigned*>(self + 56) = running;
        *reinterpret_cast<unsigned*>(self + 52) = running + 32;
        unsigned char* glyphRecord = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 64)) + glyphs[i] * 12;
        for (int w = 0; w < 4; ++w) {
            reinterpret_cast<unsigned*>(self + 32)[w] = color[w];
        }
        unsigned g = *reinterpret_cast<unsigned short*>(glyphRecord + 8);
        unsigned char* quad = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 68)) + g * 56;
        int advance = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(font + 64))[glyphs[i] * 12 + 10];
        reinterpret_cast<unsigned*>(self + 12)[0] = *reinterpret_cast<unsigned*>(quad + 24);
        reinterpret_cast<unsigned*>(self + 12)[1] = *reinterpret_cast<unsigned*>(quad + 28);
        right[0] = origin[0] + advance;
        Vertex__8VIRasterRC8VIVect2I(self, origin);
        int topRight[2] = {right[0], origin[1]};
        reinterpret_cast<unsigned*>(self + 12)[0] = *reinterpret_cast<unsigned*>(quad + 32);
        reinterpret_cast<unsigned*>(self + 12)[1] = *reinterpret_cast<unsigned*>(quad + 36);
        Vertex__8VIRasterRC8VIVect2I(self, topRight);
        reinterpret_cast<unsigned*>(self + 12)[0] = *reinterpret_cast<unsigned*>(quad + 40);
        reinterpret_cast<unsigned*>(self + 12)[1] = *reinterpret_cast<unsigned*>(quad + 44);
        Vertex__8VIRasterRC8VIVect2I(self, right);
        int bottomLeft[2] = {origin[0], right[1]};
        reinterpret_cast<unsigned*>(self + 12)[0] = *reinterpret_cast<unsigned*>(quad + 48);
        reinterpret_cast<unsigned*>(self + 12)[1] = *reinterpret_cast<unsigned*>(quad + 52);
        Vertex__8VIRasterRC8VIVect2I(self, bottomLeft);
        EndTriFan__8VIRaster(self);
        origin[0] = right[0] + 1;
    }
    return 0;
}
}
