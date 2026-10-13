// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because the UI object is a 32-bit word).
#define memset memset_target
#include "drawframe.cpp"
#undef memset
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static int window[16];
alignas(8) static unsigned char ui[128];
static int arrow[8];
static int heights[2], height_calls;
static float pulse_value;
static std::string trace;
static std::string pt(const VIVect2I& v) { return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")"; }
extern "C" {
void* memset_target(void* d, int v, unsigned n) { trace += "memset" + std::to_string(v) + "," + std::to_string(n) + ";"; return std::memset(d, v, n); }
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void*, void*, void*) { return heights[height_calls++ & 1]; }
const int* GetUITexture__F9VIUITexId(int id) { trace += "g" + std::to_string(id) + ";"; return id == 128 ? arrow : nullptr; }
int StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void*, int tex, VIVect2I pos, VIVect2I size, int blend, float a) {
    trace += "S" + std::to_string(tex) + pt(pos) + pt(size) + "b" + std::to_string(blend) + "a" + std::to_string(static_cast<int>(a * 1000 + 0.5f)) + ";"; return 0;
}
int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void*, const VIVect2I& pos, const VIVect2I& size, const VIUIFrameParam& p, int blend, float a, unsigned flag) {
    trace += "F" + pt(pos) + pt(size) + "p" + std::to_string(p.v[0]) + "," + std::to_string(p.v[1]) + "," + std::to_string(p.v[8]) + "," + std::to_string(p.v[14]) + "b" + std::to_string(blend) + "f" + std::to_string(flag) + ";";
    return 0;
}
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void*, int tex, VIVect2I at, int blend, float a, unsigned flags) {
    trace += "E" + std::to_string(tex) + pt(at) + "p" + std::to_string(static_cast<int>(a * 100 + 0.5f)) + "f" + std::to_string(flags) + ";"; return 0;
}
float TickPulse__C10VIUIWindow(const void*) { pulse_value += 0.25f; return pulse_value; }
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(ui) > 0xffffffffUL) { std::puts("FAIL (statics above 4 GiB)"); return 1; }
    window[0] = static_cast<int>(reinterpret_cast<unsigned long>(ui));
    window[3] = 100; window[4] = 200; window[5] = 60; window[6] = 40; window[7] = 10;
    reinterpret_cast<float*>(window)[10] = 0.5f;
    arrow[4] = 8; arrow[5] = 6;
    heights[0] = 16; heights[1] = 20;
    // x0 = 100 - 30 = 70, y0 = 200 - 20 = 180; body = 3 * 12 = 36; pos = (70 + 5 - 4, 180 + 10 + 16 * 2 + 5 - 7) = (71, 220).
    // size = (50 + 8, 36 + 4) = (58, 40) without flag 0x40.
    trace.clear();
    DrawFrame__10VIUIWindowiiiiUi(window, 5, 2, 50, 3, 0x20);
    ok = ok && trace ==
         "memset0,64;g101;g106;S61(71,220)(58,40)b3a50;F(71,220)(58,40)p101,0,106,104b0f1;g128;"
         "E128(60,237)p25f0;E128(132,237)p50f1;";
    trace.clear(); height_calls = 0; pulse_value = 0;
    DrawFrame__10VIUIWindowiiiiUi(window, 5, 2, 50, 3, 0x40);
    ok = ok && trace == "memset0,64;g101;g106;S61(71,220)(58,36)b3a50;F(71,220)(58,36)p101,0,106,104b0f1;g128;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
