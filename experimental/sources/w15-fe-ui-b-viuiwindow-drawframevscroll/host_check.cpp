#define NORRATH_HOST_ARENA
#include <cmath>
#include <cstring>
#include <string>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "drawframevscroll.cpp"

static std::string trace;
static int height_calls, frame_args[5], scroll_x, scroll_y, scroll_h, scroll_calls;
static float scroll_f;
extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void*, unsigned, unsigned) { return ++height_calls == 1 ? 17 : 21; }
extern "C" void DrawFrame__10VIUIWindowiiiiUi(void*, int a, int b, int c, int d, unsigned flags) {
    trace += "F;";
    frame_args[0] = a; frame_args[1] = b; frame_args[2] = c; frame_args[3] = d; frame_args[4] = static_cast<int>(flags);
}
extern "C" const int* GetUITexture__F9VIUITexId(int texture) { trace += "T" + std::to_string(texture) + ";"; return nullptr; }
extern "C" void DrawVScroll__10VIUIWindowG8VIVect2IifUi(void*, VIVect2I at, int height, float f, unsigned flags) {
    trace += "V;";
    ++scroll_calls; scroll_x = at.x; scroll_y = at.y; scroll_h = height; scroll_f = f;
    if (flags != 0) scroll_h = -1;
}

int main() {
    int window[16] = {};
    window[0] = static_cast<int>(arena_base + 256);
    window[3] = 300; window[4] = 200; window[5] = 100; window[6] = 60; window[7] = 12;
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, 200, 7, 0.5f);
    if (trace != "F;T84;V;") return 1;
    if (frame_args[0] != 40 || frame_args[1] != 1 || frame_args[2] != 200 || frame_args[3] != 7 || frame_args[4] != 0) return 2;
    // x = 300 - 50 + 40 + 200 = 490; y = 200 - 30 + 12 + (21 - 4) * 1 - 2 = 197; height = (17 - 4) * 7 + 4 = 95
    if (scroll_x != 490 || scroll_y != 197 || scroll_h != 95 || scroll_f != 0.5f) return 3;
    trace.clear(); scroll_calls = 0; height_calls = 0;
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, 200, 7, 1.5f);
    if (trace != "F;" || scroll_calls != 0) return 4;
    height_calls = 0; trace.clear();
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, 200, 7, -0.5f);
    if (trace != "F;") return 5;
    height_calls = 0; trace.clear();
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, 200, 7, std::nanf(""));
    if (trace != "F;") return 6;
    height_calls = 0; trace.clear();
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, 200, 7, 0.0f);
    DrawFrameVScroll__10VIUIWindowiiiif(window, 40, 1, 200, 7, 1.0f);
    if (scroll_calls != 2) return 7;
    return 0;
}
