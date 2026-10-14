// Observed: DrawProgress__10VIUIWindowiiif at 0x0103d9e0, 784 bytes. Arguments: the window (word 0 the UI object, ints at
// +12 x, +16 y, +20 w, +24 h, +28 inner offset, the float at +40), ints a, b and c, and a float f. With ui = the word at
// this, GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, word at ui + 72, word at ui + 80) called twice (h1 first, then
// h2), X0 = x - (w >> 1), Y0 = y - (h >> 1) (arithmetic shifts), alpha the window float:
//  - left = X0 + a and top = Y0 + inner + (h2 - 4) * b + 5; the frame position is P = (left - 4, top - 4 - 3) and its size is
//    S = (c + 8, h1 - 4).
//  - a 64-byte parameter is cleared with memset(param, 0, 64) and its sixteen ints are set exactly as in DrawFrame
//    (p[0] = 101, p[2] = 102, p[4] = 103, p[6] = 104, p[8] = 106, p[10] = 102, p[12] = 105, p[14] = 104).
//  - GetUITexture__F9VIUITexId(101) and (106) are called, results ignored.
//  - StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 61, P, S, 3, alpha * 0.1) with the factor bits 0x3dcccccc,
//    then BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, P, S, param, 0, alpha, 1).
//  - The fill: width = (int)((float)c * f) (library conversions, truncating); the rectangle (left, top - 4) to
//    (left + width, top - 4 + (h1 - 8)) is passed to FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(ui, rect, colour) with
//    the colour (0.0, 0.0, 0.8, 1.0), the blue component having the bits 0x3f4ccccc.
// No result is produced. The original also stores several unused pairs in its frame; they are not reproduced.
// Inferred: a progress bar drawn as a one-line dialog frame with a blue fill whose length is the fraction f of the width c.
// Unknown: the texture names, the meaning of a, b and c as units, whether f is clamped by callers, and the result type (void
// here).
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
struct VIUIFrameParam {
    int v[16];
};
struct VIColor32F {
    float v[4];
};
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary);
const int* GetUITexture__F9VIUITexId(int texture);
int StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* ui, int texture, VIVect2I pos, VIVect2I size,
                                                                   int blend, float alpha);
int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* ui, const VIVect2I& pos, const VIVect2I& size,
                                                                        const VIUIFrameParam& parameter, int blend,
                                                                        float alpha, unsigned flag);
void FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(void* ui, const int* rect, const VIColor32F* color);
void* memset(void* destination, int value, unsigned size);

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void DrawProgress__10VIUIWindowiiif(void* self, int a, int b, int c, float f) {
    const int* words = static_cast<const int*>(self);
    unsigned char* ui = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(words[0]));
    const float alpha = reinterpret_cast<const float*>(words)[10];
    const int h1 = GetHeight__6VIFontP8VIRasterP12VIDictionary(
        ui + 56, NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(ui + 72)), NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(ui + 80)));
    const int x0 = words[3] - (words[5] >> 1);
    const int y0 = words[4] - (words[6] >> 1);
    const int line = h1 - 8;
    const int left = x0 + a;
    const int h2 = GetHeight__6VIFontP8VIRasterP12VIDictionary(
        ui + 56, NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(ui + 72)), NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(ui + 80)));
    const int top = y0 + words[7] + (h2 - 4) * b + 5;
    VIUIFrameParam param;
    memset(&param, 0, 64);
    VIVect2I pos;
    pos.x = left - 4;
    pos.y = top - 4 - 3;
    VIVect2I size;
    size.x = c + 8;
    size.y = line + 4;
    GetUITexture__F9VIUITexId(101);
    GetUITexture__F9VIUITexId(106);
    param.v[0] = 101;
    param.v[2] = 102;
    param.v[4] = 103;
    param.v[6] = 104;
    param.v[8] = 106;
    param.v[10] = 102;
    param.v[12] = 105;
    param.v[14] = 104;
    StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(ui, 61, pos, size, 3,
                                                                   alpha * __builtin_bit_cast(float, 0x3dcccccc));
    BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(ui, pos, size, param, 0, alpha, 1);
    const int width = static_cast<int>(static_cast<float>(c) * f);
    int rect[4];
    rect[0] = left;
    rect[1] = top - 4;
    rect[2] = left + width;
    rect[3] = top - 4 + line;
    VIColor32F color;
    color.v[0] = 0.0f;
    color.v[1] = 0.0f;
    color.v[2] = __builtin_bit_cast(float, 0x3f4ccccc);
    color.v[3] = 1.0f;
    FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(ui, rect, &color);
}
}
