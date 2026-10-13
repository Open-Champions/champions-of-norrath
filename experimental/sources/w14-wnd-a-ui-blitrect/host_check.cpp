// Host check only; not a build input.
#include "blitrect.cpp"
#include <cstdio>
#include <string>

static int record[8];
static const int* current;
static std::string trace;
extern "C" {
const int* GetUITexture__F9VIUITexId(int) { return current; }
int BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void*, int tex, VIVect2I at, int blend, float alpha, unsigned flags) {
    trace += "t" + std::to_string(tex) + "(" + std::to_string(at.x) + "," + std::to_string(at.y) + ")b" + std::to_string(blend) + "f" + std::to_string(flags) + ";";
    return 1;
}
int BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi(void*, int tex, VIVect2I at, VIVect2I size, int blend, float alpha, unsigned flags) {
    trace += "T" + std::to_string(tex) + "(" + std::to_string(at.x) + "," + std::to_string(at.y) + ")[" + std::to_string(size.x) + "," + std::to_string(size.y) + "]f" + std::to_string(flags) + ";";
    return 1;
}
}

int main() {
    bool ok = true;
    VIVect2I pos, size;
    pos.x = 0; pos.y = 0; size.x = 25; size.y = 15;
    current = nullptr;
    ok = ok && BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(nullptr, 7, pos, size, 2, 1.0f) == -1 && trace.empty();
    current = record;
    record[0] = 99; record[1] = 0; record[4] = 10; record[5] = 10;
    ok = ok && BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(nullptr, 7, pos, size, 2, 1.0f) == 0 && trace == "T99(0,0)[25,15]f0;";
    trace.clear();
    record[1] = 1;
    ok = ok && BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(nullptr, 7, pos, size, 2, 1.0f) == 0 &&
         trace == "t7(0,0)b2f0;t7(10,0)b2f0;T7(20,0)[5,10]f0;T7(0,10)[10,5]f0;T7(10,10)[10,5]f0;T7(20,10)[5,5]f0;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
