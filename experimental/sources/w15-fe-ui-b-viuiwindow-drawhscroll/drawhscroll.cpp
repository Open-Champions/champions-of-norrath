// Observed: DrawHScroll__10VIUIWindowiiifUi at 0x0103d758, 648 bytes. Arguments: the window, ints a, row and width,
// a float f and an unsigned flags word. With ui = the word at offset 0 of the window and h = GetHeight__6VIFont...
// (ui + 56, word at ui + 72, word at ui + 80), the start position is P = (x - (w >> 1) + a, y - (height >> 1) +
// inner + (h - 4) * row + 1), using window fields +12 x, +16 y, +20 width, +24 height, +28 inner offset and
// arithmetic shifts. Then, in order:
//  1. BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii is called with (ui, the empty
//     string, colour (0, 0, 0, 0), P, textures 165 and 164, width, blend 3, the window's float at +40 as alpha,
//     0, 1).
//  2. GetUITexture__F9VIUITexId(166) is saved as the knob record.
//  3. When (flags & 0x20) is set: GetUITexture(128) is read for its word 4 (width); the position-only BlitEx
//     overload is called with (ui, 128, (P.x - word4 - 3, P.y - 3), 3, TickPulse__C10VIUIWindow(window), 0), and
//     then with (ui, 128, (P.x + width + 3, P.y - 3), 3, a second TickPulse result, 1). TickPulse is called before
//     each BlitEx.
//  4. P.x is advanced by the truncated integer (f * (float)(width - knob[4])), using the library int-to-float
//     conversion and truncating float-to-int conversion; then BlitEx is called with (ui, knob[0], P, 3, the
//     window float at +40, 0).
// The original also builds several unused position pairs in its frame; they are not reproduced.
// Inferred: a horizontal slider: a bar (textures 165, 164) of the given width, optional pulsing end arrows
// (texture 128, flagged 0x20), and a knob (texture 166) placed at fraction f of the travel.
// Unknown: the texture names, the meaning of BlitBar's trailing 0 and 1, whether f is clamped by callers, and
// the exact use of the empty string (it is passed, not interpreted, here).
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};
struct VIColor32F {
    float v[4];
};

int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, unsigned raster, unsigned dictionary);
const int* GetUITexture__F9VIUITexId(int texture);
float TickPulse__C10VIUIWindow(const void* self);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);
int BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(unsigned ui, const char* name,
                                                                                VIColor32F color, VIVect2I at,
                                                                                int texture_a, int texture_b, int width,
                                                                                int blend, float alpha, unsigned flags,
                                                                                int extra);

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void DrawHScroll__10VIUIWindowiiifUi(void* self, int a, int row, int width, float f, unsigned flags) {
    const int* words = static_cast<const int*>(self);
    const float alpha = reinterpret_cast<const float*>(words)[10];
    const unsigned ui_word = static_cast<unsigned>(words[0]);
    char* ui = reinterpret_cast<char*>(NORRATH_WORD_TO_POINTER(ui_word));
    VIVect2I p;
    p.x = words[3] - (words[5] >> 1) + a;
    p.y = words[4] - (words[6] >> 1);
    const int h = GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, *reinterpret_cast<unsigned*>(ui + 72),
                                                              *reinterpret_cast<unsigned*>(ui + 80));
    p.y = p.y + words[7] + (h - 4) * row + 1;
    VIColor32F black;
    black.v[0] = 0.0f;
    black.v[1] = 0.0f;
    black.v[2] = 0.0f;
    black.v[3] = 0.0f;
    BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(ui_word, "", black, p, 165, 164, width,
                                                                                3, alpha, 0, 1);
    const int* knob = GetUITexture__F9VIUITexId(166);
    if (flags & 0x20) {
        const int* arrow = GetUITexture__F9VIUITexId(128);
        VIVect2I left;
        left.x = p.x - arrow[4] - 3;
        left.y = p.y - 3;
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui_word, 128, left, 3, TickPulse__C10VIUIWindow(self), 0);
        VIVect2I right;
        right.x = p.x + width + 3;
        right.y = p.y - 3;
        BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui_word, 128, right, 3, TickPulse__C10VIUIWindow(self), 1);
    }
    p.x = p.x + static_cast<int>(f * static_cast<float>(width - knob[4]));
    VIVect2I at;
    at.x = p.x;
    at.y = p.y;
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui_word, knob[0], at, 3, alpha, 0);
}
}
