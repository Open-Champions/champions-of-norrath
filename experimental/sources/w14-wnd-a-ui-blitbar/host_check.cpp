// Host check only; not a build input.
#include "blitbar.cpp"
#include <cstdio>
#include <string>

static int left_record[8], middle_record[8];
static bool have_left = true, have_middle = true;
static std::string trace;
static std::string p(const VIVect2I& v) { return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + ")"; }
extern "C" {
const int* GetUITexture__F9VIUITexId(int id) {
    trace += "g" + std::to_string(id) + ";";
    if (id == 1) return have_left ? left_record : nullptr;
    return have_middle ? middle_record : nullptr;
}
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void*, int tex, VIVect2I at, int blend, float, unsigned flags) {
    trace += "e" + std::to_string(tex) + p(at) + "b" + std::to_string(blend) + "f" + std::to_string(flags) + ";";
    return 0;
}
int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(void*, int tex, VIVect2I at, VIVect2I size, int, float, unsigned) {
    trace += "E" + std::to_string(tex) + p(at) + p(size) + ";";
    return 0;
}
int BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi(void*, VIVect2I at, int tex, int width, int, float, unsigned) {
    trace += "H" + p(at) + std::to_string(tex) + "w" + std::to_string(width) + ";";
    return 0;
}
int DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii(void*, const unsigned short*, int x, int y, const VIColor32F& c, unsigned flags, int extra) {
    trace += "D(" + std::to_string(x) + "," + std::to_string(y) + ")c" + std::to_string(static_cast<int>(c.v[0])) + "f" + std::to_string(flags) + "x" + std::to_string(extra) + ";";
    return 0;
}
}

int main() {
    bool ok = true;
    left_record[4] = 6; middle_record[1] = 1; middle_record[5] = 13;
    VIColor32F color = {{9, 0, 0, 0}};
    VIVect2I pos; pos.x = 100; pos.y = 40;
    const unsigned short text[1] = {0};
    int r = BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(nullptr, text, color, pos, 1, 2, 80, 3, 1.0f, 0, 5);
    ok = ok && r == 0 && trace == "g1;g2;e1(100,40)b3f0;H(106,40)2w68;e1(174,40)b3f1;D(140,43)c9f0x5;";
    trace.clear();
    middle_record[1] = 0;
    r = BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(nullptr, text, color, pos, 1, 2, 80, 3, 1.0f, 1, 5);
    ok = ok && r == 0 && trace == "g1;g2;e1(100,40)b3f0;E2(106,40)(68,13);e1(174,40)b3f1;D(106,43)c9f1x5;";
    trace.clear();
    have_middle = false;
    r = BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(nullptr, text, color, pos, 1, 2, 80, 3, 1.0f, 1, 5);
    ok = ok && r == -1 && trace == "g1;g2;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
