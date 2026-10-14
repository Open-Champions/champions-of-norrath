#include "count_lines.cpp"
#include <cstring>

struct Seen { const void* self; const unsigned short* text; int x, y, width, rows; const void* color; unsigned flags; int calls; };
static Seen seen;
extern "C" int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int x, int y, int width,
                                                             int rows, const VIColor32F& color, unsigned flags) {
    seen = {self, text, x, y, width, rows, &color, flags, seen.calls + 1};
    return 31;
}

int main() {
    alignas(16) unsigned char window[400] = {};
    const unsigned short text[2] = {'x', 0};
    if (CountLines__10VIUIWindowPCUsi(window, text, 77) != 31) return 1;
    if (seen.calls != 1 || seen.self != window || seen.text != text) return 2;
    if (seen.x != 0 || seen.y != 0 || seen.width != 77 || seen.rows != 256) return 3;
    if (seen.color != window + 304 || seen.flags != 0xffffffffu) return 4;
    return 0;
}
