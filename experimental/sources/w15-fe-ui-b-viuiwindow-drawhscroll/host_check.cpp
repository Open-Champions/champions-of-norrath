#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "drawhscroll.cpp"
#include <string>
#include <vector>

struct Blit { int texture, x, y; float alpha; unsigned flags; };
static std::vector<Blit> blits;
static std::string bar_name;
static int bar[8];
static float bar_alpha, bar_color[4];
static int pulses;
static int record_a[9] = {0, 0, 0, 0, 10, 0, 0, 0, 0};   // texture 128
static int record_b[9] = {166, 0, 0, 0, 30, 0, 0, 0, 0};  // texture 166
extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void*, unsigned, unsigned) { return 17; }
extern "C" const int* GetUITexture__F9VIUITexId(int texture) { return texture == 128 ? record_a : record_b; }
extern "C" float TickPulse__C10VIUIWindow(const void*) { return 0.125f * static_cast<float>(++pulses); }
extern "C" int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned, int texture, VIVect2I at, int blend, float alpha, unsigned flags) {
    if (blend != 3) return 99;
    blits.push_back({texture, at.x, at.y, alpha, flags});
    return 1;
}
extern "C" int BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(unsigned, const char* name, VIColor32F color, VIVect2I at, int a, int b, int width, int blend, float alpha, unsigned flags, int extra) {
    bar_name = name;
    std::memcpy(bar_color, color.v, 16);
    bar[0] = at.x; bar[1] = at.y; bar[2] = a; bar[3] = b; bar[4] = width; bar[5] = blend; bar[6] = static_cast<int>(flags); bar[7] = extra;
    bar_alpha = alpha;
    return 1;
}

int main() {
    float window_f[16] = {};
    int* window = reinterpret_cast<int*>(window_f);
    window[0] = static_cast<int>(arena_base + 256);
    window[3] = 300; window[4] = 200; window[5] = 100; window[6] = 60; window[7] = 12;
    window_f[10] = 0.5f;
    // P = (300 - 50 + 20, 200 - 30 + 12 + 13 * 2 + 1)
    DrawHScroll__10VIUIWindowiiifUi(window, 20, 2, 140, 0.5f, 0x20u);
    if (!bar_name.empty() || bar[0] != 270 || bar[1] != 209 || bar[2] != 165 || bar[3] != 164 || bar[4] != 140 || bar[5] != 3 || bar[6] != 0 || bar[7] != 1) return 1;
    if (bar_alpha != 0.5f || bar_color[0] != 0 || bar_color[3] != 0) return 2;
    if (blits.size() != 3) return 3;
    if (blits[0].texture != 128 || blits[0].x != 270 - 10 - 3 || blits[0].y != 206 || blits[0].alpha != 0.125f || blits[0].flags != 0u) return 4;
    if (blits[1].texture != 128 || blits[1].x != 270 + 140 + 3 || blits[1].y != 206 || blits[1].alpha != 0.25f || blits[1].flags != 1u) return 5;
    // knob: x += (int)(0.5 * (140 - 30)) = 55
    if (blits[2].texture != 166 || blits[2].x != 325 || blits[2].y != 209 || blits[2].alpha != 0.5f || blits[2].flags != 0u) return 6;
    blits.clear();
    DrawHScroll__10VIUIWindowiiifUi(window, 20, 2, 140, 1.0f, 0u);
    if (blits.size() != 1 || blits[0].x != 270 + 110) return 7;
    blits.clear();
    DrawHScroll__10VIUIWindowiiifUi(window, 20, 2, 140, -0.99f, 0u);  // truncation toward zero
    if (blits.size() != 1 || blits[0].x != 270 - 108) return 8;
    return 0;
}
