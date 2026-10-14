// Host check only; not a build input.
#include "blitframe.cpp"
#include <cstdio>
#include <string>

static int rec[4][8];
static bool present[4] = {true, true, true, true};
static std::string trace;
static std::string pt(const VIVect2I& v) { return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")"; }
extern "C" {
const int* GetUITexture__F9VIUITexId(int id) { return present[id / 10 - 1] ? rec[id / 10 - 1] : nullptr; }
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void*, int tex, VIVect2I at, int, float, unsigned flags) {
    trace += "e" + std::to_string(tex) + pt(at) + "f" + std::to_string(flags) + ";"; return 0;
}
int BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void*, VIVect2I at, int tex, int length, int, float, unsigned flags) {
    trace += "h" + std::to_string(tex) + pt(at) + "l" + std::to_string(length) + "f" + std::to_string(flags) + ";"; return 0;
}
int BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void*, VIVect2I at, int tex, int length, int, float, unsigned flags) {
    trace += "v" + std::to_string(tex) + pt(at) + "l" + std::to_string(length) + "f" + std::to_string(flags) + ";"; return 0;
}
}

int main() {
    bool ok = true;
    // Textures 0..3 are 4x5, 6x7, 8x9, 10x11 (width at [4], height at [5]).
    for (int k = 0; k < 4; ++k) { rec[k][4] = 4 + 2 * k; rec[k][5] = 5 + 2 * k; }
    VIUIFrameParam p;
    for (int i = 0; i < 16; ++i) p.v[i] = 100 + i;
    p.v[0] = 10; p.v[4] = 20; p.v[8] = 30; p.v[12] = 40;
    VIVect2I pos, size;
    pos.x = 50; pos.y = 60; size.x = 100; size.y = 80;
    int r = BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(nullptr, pos, size, p, 2, 1.0f, 1);
    ok = ok && r == 0 && trace ==
         "h102(54,60)l90f103;h110(60,129)l82f111;v114(50,65)l64f115;v106(144,67)l64f107;"
         "e10(50,60)f101;e20(144,60)f105;e40(50,129)f113;e30(142,131)f109;";
    trace.clear();
    r = BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(nullptr, pos, size, p, 2, 1.0f, 0x100);
    ok = ok && r == 0 && trace == "h102(50,60)l94f103;h110(50,129)l92f111;v106(144,67)l64f107;e20(144,60)f105;e30(142,131)f109;";
    trace.clear();
    present[3] = false;
    ok = ok && BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(nullptr, pos, size, p, 2, 1.0f, 1) == -1 && trace.empty();
    size.x = 0;
    present[3] = true;
    ok = ok && BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(nullptr, pos, size, p, 2, 1.0f, 1) == 0 && trace.empty();
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
