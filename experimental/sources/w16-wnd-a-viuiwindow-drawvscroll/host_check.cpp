// Host check only; not a build input. Also compared with the original in a private interpreter (80 paired
// scenarios, all 70 words covered). Callees log; the expected log and positions follow the description in the
// source comment, over f values inside and outside 0..1 (including NaN and the endpoints), several heights and
// start positions, and knob records of different sizes.
#include "drawvscroll.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static int record[24];
static std::string N(long v) { return std::to_string(v); }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return N(v); }
extern "C" const int* GetUITexture__F9VIUITexId(int id) { trace += "T" + N(id) + ";"; return record; }
extern "C" int BlitVertBar__4VIUIG8VIVect2I9VIUITexIdN22i16VIAlphaBlendModef(unsigned ui, VIVect2I at, int first, int second, int third, int height, int blend, float alpha) {
    trace += "V" + N(ui) + "," + N(at.x) + "/" + N(at.y) + "," + N(first) + "," + N(second) + "," + N(third) + "," + N(height) + "," + N(blend) + "," + fbits(alpha) + ";";
    return 0;
}
extern "C" int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int texture, VIVect2I at, int blend, float alpha, unsigned flags) {
    trace += "E" + N(ui) + "," + N(texture) + "," + N(at.x) + "/" + N(at.y) + "," + N(blend) + "," + fbits(alpha) + "," + N(flags) + ";";
    return 0;
}

int main() {
    bool ok = true;
    long scenarios = 0;
    const float values[] = {0.0f, 0.25f, 0.5f, 0.7f, 1.0f, -0.001f, 1.001f, 2.0f, -5.0f, NAN};
    for (float f : values) for (int height : {0, 40, 200, -30, 77}) for (int knob_height : {0, 9, 31}) for (int px : {10, -5}) for (int py : {20, 1000, -3}) {
        int window[16];
        std::memset(window, 0x11, sizeof window);
        window[0] = 0x4321;
        const float alpha = 0.375f;
        std::memcpy(&window[10], &alpha, 4);
        for (int i = 0; i < 24; ++i) record[i] = 1000 + i;
        record[5] = knob_height;
        VIVect2I pos;
        pos.x = px;
        pos.y = py;
        trace.clear();
        DrawVScroll__10VIUIWindowG8VIVect2IifUi(window, pos, height, f, 0xdeadu);

        std::string want = "T84;V17185," + N(px) + "/" + N(py) + ",81,83,82," + N(height) + ",3," + fbits(alpha) + ";";
        if (0.0f <= f && f <= 1.0f)
            want += "E17185,1000," + N(px - 1) + "/" + N(py + static_cast<int>(f * static_cast<float>(height - knob_height))) + ",3," + fbits(alpha) + ",0;";
        if (trace != want) { ok = false; std::printf("MISMATCH f=%f height=%d got '%s' want '%s'\n", f, height, trace.c_str(), want.c_str()); break; }
        ++scenarios;
    }
    std::puts(ok ? ("ok (" + std::to_string(scenarios) + " scenarios)").c_str() : "FAIL");
    return !ok;
}
