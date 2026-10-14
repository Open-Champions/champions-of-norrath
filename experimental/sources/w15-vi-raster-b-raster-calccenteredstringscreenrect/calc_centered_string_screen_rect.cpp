// Observed: CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect at 0x0110f790 is 500 bytes in the pinned SLUS-20565 image.
// Arguments: the raster (this), a font slot index, a 3-float position, a string and a rect of four ints. It picks the font pointer from
// the 12-byte slot at that index of the table pointed to by the raster word at +19516 (0x4c3c). The position goes through
// __ml__FRC7VIVect3RC10VIMatrix44(result, position, matrix at 0x70000000) into a four-float result; those four floats go through
// __ml__FRC7VIVect4RC10VIMatrix44(result, vector, matrix at 0x70000040) into a second four-float result (x, y, z, w). Each of the four
// is multiplied by 1.0f / w. With half-extents hx = float(int at raster +17748 >> 1) and hy = float(int at raster +17752 >> 1) the screen
// point is (int)(x * hx + hx) and (int)((-y) * hy + hy), truncated toward zero. GetStringWidth__7VIRFontPCci(font, string, 1) gives the
// width w; the font's word at +20 is the height h. The rect is written as left = sx - (w >> 1), top = sy - (h >> 1), right = left + w,
// bottom = top + h (words +0, +4, +8, +12), and the function returns 0. The products and sums are separate multiply and add steps.
// Inferred: this centres a string on the projection of a 3D point and reports the screen rectangle it covers; the constants
// 0x70000000/0x70000040 are the scratchpad view and projection matrices (GetModelView returns the first).
// Unknown: the 3-component-to-4-component result types, the matrix layouts, the units of the raster words at +17748/+17752 (they look
// like the screen width and height), and the callees' behaviour (imports of this unit). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
void __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
void __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
int GetStringWidth__7VIRFontPCci(unsigned char* font, const char* text, int flag);
int CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect(unsigned char* self, int fontIndex, const float* position, const char* text, int* rect) {
    float vector[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    int point[2] = {0, 0};
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 19516));
    unsigned char* font = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + fontIndex * 12));
    float view[4];
    __ml__FRC7VIVect3RC10VIMatrix44(view, position, NORRATH_WORD_TO_POINTER(0x70000000u));
    vector[0] = view[0];
    vector[1] = view[1];
    vector[2] = view[2];
    vector[3] = view[3];
    float clip[4];
    __ml__FRC7VIVect4RC10VIMatrix44(clip, vector, NORRATH_WORD_TO_POINTER(0x70000040u));
    float inverse = 1.0f / clip[3];
    float x = clip[0] * inverse;
    float y = clip[1] * inverse;
    float halfHeight = static_cast<float>(*reinterpret_cast<int*>(self + 17752) >> 1);
    float halfWidth = static_cast<float>(*reinterpret_cast<int*>(self + 17748) >> 1);
    point[0] = static_cast<int>(x * halfWidth + halfWidth);
    point[1] = static_cast<int>((-y) * halfHeight + halfHeight);
    int width = GetStringWidth__7VIRFontPCci(font, text, 1);
    int height = *reinterpret_cast<int*>(font + 20);
    rect[0] = point[0] - (width >> 1);
    rect[1] = point[1] - (height >> 1);
    rect[2] = rect[0] + width;
    rect[3] = rect[1] + height;
    return 0;
}
}
