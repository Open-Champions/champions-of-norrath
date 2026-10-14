// Observed: DrawFrameVScroll__10VIUIWindowiiiif at 0x0103d558, 508 bytes. Arguments: the window, ints a, b, c, d and a
// float f. With ui = the word at offset 0 of the window and GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56,
// word at ui + 72, word at ui + 80) called twice (h1, then h2), and window fields +12 x, +16 y, +20 width,
// +24 height, +28 inner offset (arithmetic shifts for the halves):
//  1. DrawFrame__10VIUIWindowiiiiUi(window, a, b, c, d, 0) is called.
//  2. Only when 0.0 <= f and f <= 1.0 (both compares false for NaN): GetUITexture__F9VIUITexId(84) is called and
//     its result ignored; then DrawVScroll__10VIUIWindowG8VIVect2IifUi(window, P, S, f, 0) is called with
//     P = (x - (width >> 1) + a + c, y - (height >> 1) + inner + (h2 - 4) * b - 2) and
//     S = (h1 - 4) * d + 4 (the third argument, an int).
// The original also stores several unused pairs in its frame; they are not reproduced.
// Inferred: a dialog frame (rows measured in text lines) with a vertical scroll bar on its right edge whose
// position is f in 0..1; texture 84 is looked up but unused, perhaps the bar's thumb or track.
// Unknown: the texture's name, the exact meaning of S (it equals the frame's pixel height), and the result type
// (void here).
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};

int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, unsigned raster, unsigned dictionary);
void DrawFrame__10VIUIWindowiiiiUi(void* self, int a, int b, int c, int d, unsigned flags);
const int* GetUITexture__F9VIUITexId(int texture);
void DrawVScroll__10VIUIWindowG8VIVect2IifUi(void* self, VIVect2I at, int height, float f, unsigned flags);

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void DrawFrameVScroll__10VIUIWindowiiiif(void* self, int a, int b, int c, int d, float f) {
    const int* words = static_cast<const int*>(self);
    char* ui = reinterpret_cast<char*>(NORRATH_WORD_TO_POINTER(static_cast<unsigned>(words[0])));
    const int h1 = GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, *reinterpret_cast<unsigned*>(ui + 72),
                                                               *reinterpret_cast<unsigned*>(ui + 80));
    const int body = (h1 - 4) * d;
    const int left = words[3] - (words[5] >> 1) + a;
    const int h2 = GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, *reinterpret_cast<unsigned*>(ui + 72),
                                                               *reinterpret_cast<unsigned*>(ui + 80));
    const int top = words[4] - (words[6] >> 1) + words[7] + (h2 - 4) * b + 5;
    DrawFrame__10VIUIWindowiiiiUi(self, a, b, c, d, 0);
    if (0.0f <= f && f <= 1.0f) {
        GetUITexture__F9VIUITexId(84);
        VIVect2I at;
        at.x = left - 4 + (c + 8) - 4;
        at.y = top - 4 - 3;
        DrawVScroll__10VIUIWindowG8VIVect2IifUi(self, at, body + 4, f, 0);
    }
}
}
