// Host check only; not a build input.
#include "blitfillframe.cpp"
#include <cstdio>
#include <string>

static int rec[3][8];
static bool present[3] = {true, true, true};
static std::string trace;
static std::string pt(const VIVect2I& v) { return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")"; }
extern "C" {
const int* GetUITexture__F9VIUITexId(int id) { return present[id / 10 - 1] ? rec[id / 10 - 1] : nullptr; }
int BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void*, int tex, VIVect2I pos, VIVect2I size, int blend, float) {
    trace += "R" + std::to_string(tex) + pt(pos) + pt(size) + "b" + std::to_string(blend) + ";"; return 55;
}
int BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void*, const VIVect2I& pos, const VIVect2I& size,
                                                                        const VIUIFrameParam& p, int blend, float, unsigned flag) {
    trace += "F" + pt(pos) + pt(size) + "p" + std::to_string(p.v[3]) + "b" + std::to_string(blend) + "f" + std::to_string(flag) + ";";
    return -4;
}
}

int main() {
    bool ok = true;
    rec[0][4] = 4; rec[0][5] = 5;      // first corner (p[0])
    rec[1][4] = 6; rec[1][5] = 7;      // third corner (p[8])
    VIUIFrameParam p;
    for (int i = 0; i < 16; ++i) p.v[i] = 100 + i;
    p.v[0] = 10; p.v[8] = 20;
    VIVect2I pos, size;
    pos.x = 50; pos.y = 60; size.x = 100; size.y = 80;
    int r = BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(nullptr, pos, size, p, 30, 2, 1.0f, 0x1ff);
    ok = ok && r == -4 && trace == "R30(54,65)(90,68)b2;F(50,60)(100,80)p103b2f255;";
    trace.clear();
    present[2] = false;
    ok = ok && BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(nullptr, pos, size, p, 30, 2, 1.0f, 1) == -1 && trace.empty();
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
