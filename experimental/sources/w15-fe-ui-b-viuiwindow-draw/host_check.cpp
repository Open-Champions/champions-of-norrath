#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <string>
#define memset norrath_host_memset
#include "draw.cpp"
#undef memset

static std::string trace;
static int memset_size;
static int fill_texture, fill_blend, fill_flag, fill_pos[2], fill_size[2], fill_param[16];
static float fill_alpha;
static int frame_pos[2], frame_size[2], frame_blend, frame_flag, frame_param[16];
static float frame_alpha;
static int rect_texture, rect_pos[2], rect_size[2];
static float rect_alpha;
static std::string bar_name;
static int bar_pos[2], bar_width, bar_a, bar_b;
static float bar_alpha, bar_color[4];

extern "C" void* norrath_host_memset(void* destination, int value, unsigned size) {
    trace += "M;";
    memset_size = static_cast<int>(size);
    unsigned char* p = static_cast<unsigned char*>(destination);
    for (unsigned i = 0; i < size; ++i) p[i] = static_cast<unsigned char>(value);
    return destination;
}
extern "C" int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void*, unsigned, unsigned, const char* text) {
    trace += "W;";
    return static_cast<int>(std::strlen(text)) * 10;
}
extern "C" int BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(
    void*, VIVect2I at, VIVect2I size, VIUIFrameParam parameter, int texture, int blend, float alpha, unsigned char flag) {
    trace += "F;";
    fill_pos[0] = at.x; fill_pos[1] = at.y; fill_size[0] = size.x; fill_size[1] = size.y;
    std::memcpy(fill_param, parameter.v, 64);
    fill_texture = texture; fill_blend = blend; fill_alpha = alpha; fill_flag = flag;
    return 0;
}
extern "C" int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void*, VIVect2I at, VIVect2I size,
                                                                                   VIUIFrameParam parameter, int blend,
                                                                                   float alpha, unsigned char flag) {
    trace += "R;";
    frame_pos[0] = at.x; frame_pos[1] = at.y; frame_size[0] = size.x; frame_size[1] = size.y;
    std::memcpy(frame_param, parameter.v, 64);
    frame_blend = blend; frame_alpha = alpha; frame_flag = flag;
    return 0;
}
extern "C" int BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void*, int texture, VIVect2I at, VIVect2I size, int blend, float alpha) {
    trace += "T;";
    rect_texture = texture; rect_pos[0] = at.x; rect_pos[1] = at.y; rect_size[0] = size.x; rect_size[1] = size.y; rect_alpha = alpha;
    return blend == 3 ? 0 : 1;
}
extern "C" int BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(
    void*, const char* name, VIColor32F color, VIVect2I at, int a, int b, int width, int blend, float alpha, unsigned, int) {
    trace += "B;";
    bar_name = name; std::memcpy(bar_color, color.v, 16); bar_pos[0] = at.x; bar_pos[1] = at.y;
    bar_a = a; bar_b = b; bar_width = width; bar_alpha = alpha;
    return blend;
}

static float window_storage[100];
static float* floats() { return window_storage; }
static int* words() { return reinterpret_cast<int*>(window_storage); }
static void setup(float p, float rate, int style, const char* title) {
    std::memset(window_storage, 0, sizeof window_storage);
    words()[0] = static_cast<int>(arena_base + 256);
    words()[3] = 300; words()[4] = 200; words()[5] = 100; words()[6] = 60;
    floats()[10] = p; floats()[11] = rate; words()[90] = style;
    std::strcpy(reinterpret_cast<char*>(window_storage) + 48, title);
    trace.clear();
}

int main() {
    // style 0, opening: p 0.5 -> 0.65, alpha = 1 - 0.35^2, no title
    setup(0.5f, 1.0f, 0, "");
    if (Draw__10VIUIWindow(window_storage) != 0) return 1;
    if (trace != "M;F;R;" || memset_size != 256) return 2;
    if (floats()[10] != 0.5f + __builtin_bit_cast(float, 0x3e199999u)) return 3;
    const float alpha = 1.0f - (1.0f - floats()[10]) * (1.0f - floats()[10]);
    // x0 = 250, y0 = 170
    if (fill_pos[0] != 260 || fill_pos[1] != 180 || fill_size[0] != 81 || fill_size[1] != 40 || fill_texture != 35 || fill_alpha != alpha || fill_flag != 1) return 4;
    if (fill_param[0] != 44 || fill_param[2] != 45 || fill_param[14] != 47 || fill_param[1] != 0) return 5;
    if (frame_pos[0] != 250 || frame_pos[1] != 170 || frame_size[0] != 100 || frame_size[1] != 60 || frame_blend != 3 || frame_flag != 1) return 6;
    if (frame_param[0] != 36 || frame_param[5] != 1 || frame_param[7] != 1 || frame_param[9] != 1 || frame_param[14] != 42 || frame_param[3] != 0) return 7;
    // style 1: one fill with the 61 texture and 0.75 alpha
    setup(1.0f, 1.0f, 1, "");
    if (Draw__10VIUIWindow(window_storage) != 1 || trace != "M;F;") return 8;
    if (fill_texture != 61 || fill_alpha != 0.75f || fill_size[0] != 80 || fill_param[0] != 101 || fill_param[7] != 1 || fill_param[11] != 2 || fill_param[14] != 104) return 9;
    // style 2 with a title; closing to 0
    setup(0.1f, -1.0f, 2, "abc");
    if (Draw__10VIUIWindow(window_storage) != 1 || floats()[10] != 0.0f) return 10;
    if (trace != "M;T;R;W;B;" || rect_texture != 51 || rect_pos[0] != 260 || rect_size[0] != 81) return 11;
    // title width 30 + 50 = 80; x = 300 - 40, y = 170 + 10 - 21; colour (1,1,1,alpha)
    if (bar_name != "abc" || bar_pos[0] != 260 || bar_pos[1] != 159 || bar_width != 80 || bar_a != 33 || bar_b != 34) return 12;
    if (bar_color[0] != 1.0f || bar_color[2] != 1.0f || bar_color[3] != bar_alpha) return 13;
    // unknown style draws nothing but the memset; p between 0 and 1 reports not settled
    setup(0.5f, 0.0f, 7, "");
    if (Draw__10VIUIWindow(window_storage) != 0 || trace != "M;") return 14;
    return 0;
}
