// Observed: DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc at
// 0x0110f1b0, 1504 bytes. Arguments: the raster, an int font index, the
// address of a 3-float position, the address of a 16-byte colour and the
// address of a string. With r the raster:
//   font = the word at (the word at r + 19516) + 12 * index.
//   n = Cache__7VIRFontR8VIRasterPCciPi(font, r, text, 256, r + 17900); n < 0
//   returns -1. AddMaterial__8VIRasteriPP10_sceDmaTag(r, the int at font + 16,
//   r + 19492) < 0 returns -1.
//   Position: a = pos * the matrix at scratchpad 0x70000000 (Vect3 * Matrix44,
//   a 16-byte result), b = a * the matrix at scratchpad 0x70000040 (Vect4 *
//   Matrix44). inv = 1.0 / b.w; all four components of b are multiplied by inv.
//   With hw = (int at r + 17748) >> 1 and hh = (int at r + 17752) >> 1 (as
//   floats): centre x = (int)(b.x * hw + hw), centre y = (int)(-b.y * hh + hh).
//   Then Store2D__8VIRaster(r), Identity__10VIMatrix44 on both scratchpad
//   matrices, and the word at r + 17888 is set to 1.
//   r + 17900 holds n glyph indices. width = the sum over them of
//   (unsigned byte at glyph + 10) + 1, where glyph = (word at font + 64) +
//   12 * index. The pen starts at x = centre x - (width >> 1) and
//   y = centre y - ((int at font + 20) >> 1); the bottom edge is pen y plus
//   the int at font + 20.
//   For each glyph in order: if the word at r + 19532 is not 1,
//   UploadRasterMicro__8VIRasterPP10_sceDmaTag(r, r + 19492); if the word at
//   r + 17888 is non-zero, CalcMatrices__8VIRaster(r) then
//   UploadMatrices__8VIRasterPP10_sceDmaTag(r, r + 19492). Then 2 is stored at
//   r + 17884, the word at r + 19492 at r + 56, 0 at r + 48; r + 56 gains 16
//   and r + 52 becomes the new r + 56 plus 32. The 16 colour bytes are copied
//   to r + 32. The right edge is pen x + (unsigned byte at glyph + 10). With
//   t = (word at font + 68) + 56 * (halfword at glyph + 8), four corners are
//   sent through Vertex__8VIRasterRC7VIVect3, each after storing the float pair
//   t[24,28], t[32,36], t[40,44], t[48,52] at r + 12 / r + 16 in turn:
//   (pen x, pen y), (right, pen y), (right, bottom), (pen x, bottom). A corner
//   (X, Y) becomes x = ((float)X - f(r + 17732) + 0.5) * f(r + 17740),
//   y = ((float)Y - f(r + 17736) + 0.5) * -f(r + 17744), z = b.z.
//   EndTriFan__8VIRaster(r) follows, and the pen x becomes right + 1.
//   After the last glyph (or at once when n is zero), Restore2D__8VIRaster(r)
//   and the result is zero.
// Inferred: a screen-space text draw that centres the string on the projected
// position, one fan per glyph; the glyph record has the horizontal advance at
// +10 and a texture-rectangle index at +8; r + 17732..17744 are viewport
// offsets and scales; r + 17748 and r + 17752 the viewport size.
// Unknown: member names, why the scratchpad matrices are reset to identity
// (they are not restored here; Restore2D presumably does that), and what the
// word at r + 17884 selects.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int Cache__7VIRFontR8VIRasterPCciPi(void* font, void* raster, const char* text, int limit, int* indices);
int AddMaterial__8VIRasteriPP10_sceDmaTag(void* raster, int material, void* slot);
void __ml__FRC7VIVect3RC10VIMatrix44(void* result, const void* vector, const void* matrix);
void __ml__FRC7VIVect4RC10VIMatrix44(void* result, const void* vector, const void* matrix);
void Store2D__8VIRaster(void* raster);
void Restore2D__8VIRaster(void* raster);
void Identity__10VIMatrix44(void* matrix);
void UploadRasterMicro__8VIRasterPP10_sceDmaTag(void* raster, void* slot);
void CalcMatrices__8VIRaster(void* raster);
void UploadMatrices__8VIRasterPP10_sceDmaTag(void* raster, void* slot);
void Vertex__8VIRasterRC7VIVect3(void* raster, const void* vertex);
void EndTriFan__8VIRaster(void* raster);

static void corner(char* r, int x, int y, float z) {
    float out[3];
    float fx = static_cast<float>(x) - *reinterpret_cast<float*>(r + 17732);
    float fy = static_cast<float>(y) - *reinterpret_cast<float*>(r + 17736);
    out[0] = (fx + 0.5f) * *reinterpret_cast<float*>(r + 17740);
    out[1] = (fy + 0.5f) * -*reinterpret_cast<float*>(r + 17744);
    out[2] = z;
    Vertex__8VIRasterRC7VIVect3(r, out);
}

int DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(void* raster, int fontIndex, const void* position,
                                                               const void* color, const char* text) {
    char* r = static_cast<char*>(raster);
    char* fonts = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(r + 19516));
    char* font = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(fonts + 12 * fontIndex));
    int* indices = reinterpret_cast<int*>(r + 17900);
    int count = Cache__7VIRFontR8VIRasterPCciPi(font, raster, text, 256, indices);
    if (count < 0) return -1;
    void* slot = r + 19492;
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, *reinterpret_cast<int*>(font + 16), slot) < 0) return -1;

    float projected[4];
    float scaled[4];
    __ml__FRC7VIVect3RC10VIMatrix44(projected, position, reinterpret_cast<void*>(0x70000000));
    __ml__FRC7VIVect4RC10VIMatrix44(scaled, projected, reinterpret_cast<void*>(0x70000040));
    float inverse = 1.0f / scaled[3];
    float halfWidth = static_cast<float>(*reinterpret_cast<int*>(r + 17748) >> 1);
    float halfHeight = static_cast<float>(*reinterpret_cast<int*>(r + 17752) >> 1);
    float x = scaled[0] * inverse;
    float y = scaled[1] * inverse;
    float z = scaled[2] * inverse;
    int centerX = static_cast<int>(x * halfWidth + halfWidth);
    int centerY = static_cast<int>(-y * halfHeight + halfHeight);

    Store2D__8VIRaster(raster);
    Identity__10VIMatrix44(reinterpret_cast<void*>(0x70000000));
    Identity__10VIMatrix44(reinterpret_cast<void*>(0x70000040));
    *reinterpret_cast<int*>(r + 17888) = 1;

    char* glyphs = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(font + 64));
    int width = 0;
    for (int i = 0; i < count; ++i)
        width += *reinterpret_cast<unsigned char*>(glyphs + 12 * indices[i] + 10) + 1;
    int penX = centerX - (width >> 1);
    int lineHeight = *reinterpret_cast<int*>(font + 20);
    int penY = centerY - (lineHeight >> 1);
    int bottom = penY + lineHeight;

    for (int i = 0; i < count; ++i) {
        if (*reinterpret_cast<int*>(r + 19532) != 1) UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, slot);
        if (*reinterpret_cast<int*>(r + 17888) != 0) {
            CalcMatrices__8VIRaster(raster);
            UploadMatrices__8VIRasterPP10_sceDmaTag(raster, slot);
        }
        unsigned packet = *reinterpret_cast<unsigned*>(r + 19492);
        *reinterpret_cast<int*>(r + 17884) = 2;
        *reinterpret_cast<unsigned*>(r + 56) = packet;
        *reinterpret_cast<int*>(r + 48) = 0;
        unsigned next = *reinterpret_cast<unsigned*>(r + 56) + 16;
        *reinterpret_cast<unsigned*>(r + 56) = next;
        *reinterpret_cast<unsigned*>(r + 52) = next + 32;
        const unsigned* from = static_cast<const unsigned*>(color);
        unsigned* to = reinterpret_cast<unsigned*>(r + 32);
        to[0] = from[0];
        to[1] = from[1];
        to[2] = from[2];
        to[3] = from[3];

        char* glyph = glyphs + 12 * indices[i];
        char* rect = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(font + 68)) + 56 * *reinterpret_cast<unsigned short*>(glyph + 8);
        int right = penX + *reinterpret_cast<unsigned char*>(glyph + 10);
        *reinterpret_cast<float*>(r + 12) = *reinterpret_cast<float*>(rect + 24);
        *reinterpret_cast<float*>(r + 16) = *reinterpret_cast<float*>(rect + 28);
        corner(r, penX, penY, z);
        *reinterpret_cast<float*>(r + 12) = *reinterpret_cast<float*>(rect + 32);
        *reinterpret_cast<float*>(r + 16) = *reinterpret_cast<float*>(rect + 36);
        corner(r, right, penY, z);
        *reinterpret_cast<float*>(r + 12) = *reinterpret_cast<float*>(rect + 40);
        *reinterpret_cast<float*>(r + 16) = *reinterpret_cast<float*>(rect + 44);
        corner(r, right, bottom, z);
        *reinterpret_cast<float*>(r + 12) = *reinterpret_cast<float*>(rect + 48);
        *reinterpret_cast<float*>(r + 16) = *reinterpret_cast<float*>(rect + 52);
        corner(r, penX, bottom, z);
        EndTriFan__8VIRaster(raster);
        penX = right + 1;
    }
    Restore2D__8VIRaster(raster);
    return 0;
}
}
