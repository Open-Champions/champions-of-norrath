// Observed: TickR__10VIUIWindowiib at 0x0103dcf0, 244 bytes. Arguments: the window, ints a and b, and a bool c. With
// ui = the word at offset 0 of the window: h = GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, word at
// ui + 72, word at ui + 80). The window fields at +12 (x), +16 (y), +20 (width), +24 (height) and +28 (inner offset)
// give the position Q = (x - (width >> 1) + a - 16, y - (height >> 1) + inner + (h - 4) * b), arithmetic shifts.
// The alpha is TickPulse__C10VIUIWindow(window) when c is nonzero, otherwise 1.0. Then the position-only BlitEx
// overload is called with (ui, 89, Q, 3, alpha, 0). The original also stores two unused pairs of ints in its
// frame, derived from the same values; they are not reproduced. After the call the original leaves BlitEx's result
// in the return register, but no caller uses a result, so none is declared here.
// Inferred: draws one marker texture (id 89) at row b, left of the window's left edge by 16, pulsing when c.
// Unknown: the texture's name, the meaning of the row units, and whether the result is meant to be returned.
extern "C" {
struct VIVect2I {
    int x;
    int y;
    VIVect2I() {}
    VIVect2I(const VIVect2I& other) : x(other.x), y(other.y) {}  // non-trivial copy: passed by hidden reference
};

int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, unsigned raster, unsigned dictionary);
float TickPulse__C10VIUIWindow(const void* self);
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int texture, VIVect2I at, int blend, float alpha,
                                                            unsigned flags);

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void TickR__10VIUIWindowiib(void* self, int a, int b, bool c) {
    const int* words = static_cast<const int*>(self);
    const unsigned ui_word = static_cast<unsigned>(words[0]);
    char* ui = reinterpret_cast<char*>(NORRATH_WORD_TO_POINTER(ui_word));
    const int h = GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, *reinterpret_cast<unsigned*>(ui + 72),
                                                              *reinterpret_cast<unsigned*>(ui + 80));
    VIVect2I q;
    q.x = words[3] - (words[5] >> 1) + a - 16;
    q.y = words[4] - (words[6] >> 1) + words[7] + (h - 4) * b;
    const float alpha = c ? TickPulse__C10VIUIWindow(self) : 1.0f;
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui_word, 89, q, 3, alpha, 0);
}
}
