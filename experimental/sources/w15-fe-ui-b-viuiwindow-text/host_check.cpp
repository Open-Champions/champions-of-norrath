#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "text.cpp"

static int height_calls;
static void* height_font;
static unsigned height_raster, height_dictionary;
extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, unsigned raster, unsigned dictionary) {
    ++height_calls; height_font = font; height_raster = raster; height_dictionary = dictionary; return 17;
}
struct Seen { const void* self; const unsigned short* text; int x, y, width, rows; const void* color; unsigned flags; int calls; };
static Seen seen;
extern "C" int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int x, int y, int width,
                                                             int rows, const VIColor32F& color, unsigned flags) {
    seen = {self, text, x, y, width, rows, &color, flags, seen.calls + 1};
    return 9;
}

int main() {
    alignas(16) unsigned char window[500] = {};
    unsigned char* ui = arena + 256;
    const unsigned ui_word = arena_base + 256;
    std::memcpy(window, &ui_word, 4);
    const unsigned raster = 0x1111, dictionary = 0x2222;
    std::memcpy(ui + 72, &raster, 4);
    std::memcpy(ui + 80, &dictionary, 4);
    const unsigned short text[2] = {'x', 0};
    if (Text__10VIUIWindowPCUsii9VIUICOLORUi(window, text, -4, 6, 2, 0x80u) != 9) return 1;
    if (height_calls != 1 || height_font != ui + 56 || height_raster != raster || height_dictionary != dictionary) return 2;
    if (seen.calls != 1 || seen.self != window || seen.text != text || seen.x != -4 || seen.y != 6) return 3;
    if (seen.width != 640 || seen.rows != 1 || seen.color != window + 304 + 32 || seen.flags != 0x80u) return 4;
    return 0;
}
