#include "textbox_color.cpp"

struct Seen { const void* self; const unsigned short* text; int a, b, c, d; const void* color; unsigned flags; int calls; };
static Seen seen;
extern "C" int TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* self, const unsigned short* text, int a, int b, int c, int d,
                                                             const VIColor32F& color, unsigned flags) {
    seen = {self, text, a, b, c, d, &color, flags, seen.calls + 1};
    return 55;
}

int main() {
    alignas(16) unsigned char window[500] = {};
    const unsigned short text[2] = {'x', 0};
    if (TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 1, -2, 3, 4, 0, 9u) != 55) return 1;
    if (seen.color != window + 304 || seen.a != 1 || seen.b != -2 || seen.c != 3 || seen.d != 4 || seen.flags != 9u) return 2;
    if (TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 0, 0, 0, 0, 5, 0xffffffffu) != 55) return 3;
    if (seen.color != window + 304 + 80 || seen.flags != 0xffffffffu || seen.calls != 2) return 4;
    return 0;
}
