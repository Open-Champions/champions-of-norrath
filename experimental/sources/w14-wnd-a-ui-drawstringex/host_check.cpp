// Host check only; not a build input.
#include "drawstringex.cpp"
#include <cstdio>
#include <string>

static std::string trace;
extern "C" {
void SetTextColor__6VIFontRC10VIColor32F(void*, const VIColor32F* c) {
    trace += "col" + std::to_string(static_cast<int>(c->v[0])) + std::to_string(static_cast<int>(c->v[1])) +
             std::to_string(static_cast<int>(c->v[2])) + "/" + std::to_string(static_cast<int>(c->v[3])) + ";";
}
void DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void*, void*, void*, const unsigned short*, int x, int y) {
    trace += "draw" + std::to_string(x) + "," + std::to_string(y) + ";";
}
}

int main() {
    bool ok = true;
    alignas(8) unsigned char ui[128] = {};
    float* font_color = reinterpret_cast<float*>(ui + 56);
    font_color[0] = 9; font_color[1] = 8; font_color[2] = 7; font_color[3] = 6;
    const unsigned short text[2] = {65, 0};
    VIColor32F color = {{1, 2, 3, 4}};
    trace.clear();
    ok = ok && DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, text, 10, 20, color, 1) == 0 &&
         trace == "col000/4;draw12,22;col123/4;draw10,20;col987/6;";
    trace.clear();
    ok = ok && DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(ui, text, -5, 7, color, 0) == 0 && trace == "col123/4;draw-5,7;col987/6;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
