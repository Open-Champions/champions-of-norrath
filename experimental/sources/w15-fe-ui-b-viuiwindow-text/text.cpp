// Observed: Text__10VIUIWindowPCUsii9VIUICOLORUi at 0x0103f420, 332 bytes. Arguments: the window, a 16-bit string,
// x, y, a colour index and a flags word. With ui = the word at offset 0 of the window, it calls
// GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, word at ui + 72, word at ui + 80) once and then calls the
// colour-reference TextBox overload (0x0103c660) with (window, string, x, y, 640, 1, the 16-byte record at
// window + 304 + index * 16, flags) and returns its result. Between the two calls the original also builds
// an unused window-relative position, a copy of the colour with its fourth float scaled by
// 1 - (1 - p) * (1 - p) (p = the float at offset 40), and a few stack temporaries; none of them reaches a
// call or a store outside the frame, and none is reproduced.
// Inferred: a one-row TextBox call (640 wide, one row) with a table colour; the GetHeight call result is only
// used by the dead computations, but the call itself is retained here because it is observable.
// Unknown: why the original computes values it does not pass on (an earlier version of the call is likely).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
struct VIColor32F;

int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, unsigned raster, unsigned dictionary);
int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int x, int y, int width,
                                                  int rows, const VIColor32F& color, unsigned flags);

int Text__10VIUIWindowPCUsii9VIUICOLORUi(void* self, const unsigned short* text, int x, int y, int color,
                                         unsigned flags) {
    char* window = static_cast<char*>(self);
    char* ui = reinterpret_cast<char*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(window)));
    GetHeight__6VIFontP8VIRasterP12VIDictionary(ui + 56, *reinterpret_cast<unsigned*>(ui + 72),
                                                *reinterpret_cast<unsigned*>(ui + 80));
    return TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(
        self, text, x, y, 640, 1, *reinterpret_cast<const VIColor32F*>(window + 304 + color * 16), flags);
}
}
