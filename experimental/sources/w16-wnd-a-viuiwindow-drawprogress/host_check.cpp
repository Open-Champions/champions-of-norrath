// Host check only; not a build input. Also compared with the original in a private interpreter (120 paired
// scenarios, all 196 words covered). The window and UI object live in the arena (the UI word and the raster and
// dictionary words are guest addresses). memset is renamed so the check can log it. The expected call log follows the
// description in the source comment, over fractions inside and outside 0..1, sizes (including zero and negative offsets)
// and two different font heights for the two measurements.
#define NORRATH_HOST_ARENA 1
#define memset host_memset
#include "drawprogress.cpp"
#undef memset
#include "../w16-wnd-a-common/host_world.hpp"
#include <cmath>

static const unsigned window_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x2000, raster_g = hw::BASE + 0x3000, dictionary_g = hw::BASE + 0x3100;
static std::string trace;
static int heights[2], height_calls;
static std::string N(long v) { return std::to_string(v); }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return N(v); }
extern "C" void* host_memset(void* d, int value, unsigned size) { trace += "M" + N(value) + "," + N(size) + ";"; std::memset(d, value, size); return d; }
extern "C" int GetHeight__6VIFontP8VIRasterP12VIDictionary(void* font, void* raster, void* dictionary) {
    trace += "G" + N(hw::guest(font) - ui_g) + "," + N(hw::guest(raster)) + "," + N(hw::guest(dictionary)) + ";";
    return heights[height_calls++];
}
extern "C" const int* GetUITexture__F9VIUITexId(int id) { trace += "T" + N(id) + ";"; static int record[24]; return record; }
extern "C" int StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void* ui, int texture, VIVect2I pos, VIVect2I size, int blend, float alpha) {
    trace += "S" + N(hw::guest(ui)) + "," + N(texture) + "," + N(pos.x) + "/" + N(pos.y) + "," + N(size.x) + "/" + N(size.y) + "," + N(blend) + "," + fbits(alpha) + ";";
    return 0;
}
extern "C" int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void* ui, const VIVect2I& pos, const VIVect2I& size, const VIUIFrameParam& param, int blend, float alpha, unsigned flag) {
    trace += "B" + N(hw::guest(ui)) + "," + N(pos.x) + "/" + N(pos.y) + "," + N(size.x) + "/" + N(size.y) + ",[";
    for (int i = 0; i < 16; ++i) trace += N(param.v[i]) + (i < 15 ? "." : "");
    trace += "]," + N(blend) + "," + fbits(alpha) + "," + N(flag) + ";";
    return 0;
}
extern "C" void FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(void* ui, const int* rect, const VIColor32F* color) {
    trace += "F" + N(hw::guest(ui)) + "," + N(rect[0]) + "." + N(rect[1]) + "." + N(rect[2]) + "." + N(rect[3]) + "," + fbits(color->v[0]) + "." + fbits(color->v[1]) + "." + fbits(color->v[2]) + "." + fbits(color->v[3]) + ";";
}
static unsigned& W(unsigned off) { return hw::w(window_g + off); }

int main() {
    bool ok = true;
    long scenarios = 0;
    const float fractions[] = {0.0f, 0.25f, 0.5f, 0.333f, 0.9999f, 1.0f, -0.3f, 1.7f};
    for (float f : fractions) for (int a : {0, 10, -7, 100}) for (int b : {0, 2, 5, -1}) for (int c : {0, 17, 333, 640}) for (int h1 : {20, 9}) for (int h2 : {20, 31}) {
        hw::clear();
        W(0) = ui_g; W(12) = 400; W(16) = 300; W(20) = 201; W(24) = 99; W(28) = 13;
        const float alpha = 0.625f;
        std::memcpy(hw::at(window_g + 40), &alpha, 4);
        hw::w(ui_g + 72) = raster_g; hw::w(ui_g + 80) = dictionary_g;
        heights[0] = h1; heights[1] = h2; height_calls = 0;
        unsigned char before[0x1000];
        std::memcpy(before, hw::at(window_g), sizeof before);
        trace.clear();
        DrawProgress__10VIUIWindowiiif(hw::at(window_g), a, b, c, f);

        const int x0 = 400 - (201 >> 1), y0 = 300 - (99 >> 1);
        const int left = x0 + a, top = y0 + 13 + (h2 - 4) * b + 5;
        const int px = left - 4, py = top - 4 - 3, sx = c + 8, sy = h1 - 4;
        const int width = static_cast<int>(static_cast<float>(c) * f);
        const unsigned blue_bits = 0x3f4ccccc;
        float blue; std::memcpy(&blue, &blue_bits, 4);
        const float shadow_factor = [] { const unsigned b = 0x3dcccccc; float v; std::memcpy(&v, &b, 4); return v; }();
        const std::string g = "G56," + N(raster_g) + "," + N(dictionary_g) + ";";
        const std::string want = g + g + "M0,64;T101;T106;S" + N(ui_g) + ",61," + N(px) + "/" + N(py) + "," + N(sx) + "/" + N(sy) + ",3," + fbits(alpha * shadow_factor) + ";B" + N(ui_g) + "," + N(px) + "/" + N(py) + "," +
                                 N(sx) + "/" + N(sy) + ",[101.0.102.0.103.0.104.0.106.0.102.0.105.0.104.0],0," + fbits(alpha) + ",1;F" + N(ui_g) + "," + N(left) + "." + N(top - 4) + "." + N(left + width) + "." + N(top - 4 + h1 - 8) +
                                 "," + fbits(0.0f) + "." + fbits(0.0f) + "." + fbits(blue) + "." + fbits(1.0f) + ";";
        if (trace != want) { ok = false; std::printf("MISMATCH f=%f a=%d b=%d c=%d\n got  %s\n want %s\n", f, a, b, c, trace.c_str(), want.c_str()); break; }
        if (std::memcmp(hw::at(window_g), before, sizeof before) != 0) { ok = false; std::puts("window changed"); break; }
        ++scenarios;
    }
    std::puts(ok ? ("ok (" + std::to_string(scenarios) + " scenarios)").c_str() : "FAIL");
    return !ok;
}
