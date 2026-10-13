// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "shop_draw_task_bar.cpp"
#include <cmath>
#include <cstdio>
#include <string>

extern "C" {
int shopRunning, drawFrameNm;
void* invHeader = reinterpret_cast<void*>(0xabc0);
void* taskBar[12];
}
static std::string trace;
extern "C" {
float isin__Fi(int a) { trace += "isin" + std::to_string(a) + ";"; return std::sin(a * 6.283185307179586 / 65536.0); }
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("find:") + l + "," + n + ";"; return reinterpret_cast<void*>(0x5000 + std::string(n).size()); }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int m, unsigned c) {
    char buf[96];
    std::snprintf(buf, sizeof buf, "tex%lx,%d,%d,%d,%08x;", static_cast<unsigned long>(reinterpret_cast<__UINTPTR_TYPE__>(t)), x, y, m, c);
    trace += buf;
}
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}

int main() {
    for (int i = 0; i < 12; ++i) taskBar[i] = reinterpret_cast<void*>(0x7000 + i * 16);
    int screen[24] = {0};
    // sin(0) = 0 gives alpha 132 = 0x84; frame 4096 << 10 = 2^22 = 64 turns of 65536: also 0.
    // frame 16 gives angle 16384: sin = 1, alpha 164 = 0xa4.
    screen[0] = 100; screen[20] = 1; shopRunning = 1; drawFrameNm = 0; trace.clear();
    shopDrawTaskBar__FP12screenStruct(screen);
    expect("shop", trace,
           "find:shop.lmp,tabarmorglow.tex;isin0;tex5010,242,59,0,84808080;find:shop.lmp,tabarmor.tex;tex500c,100,48,0,80808080;texabc0,100,0,0,80808080;");
    screen[20] = 2; drawFrameNm = 16; trace.clear();
    shopDrawTaskBar__FP12screenStruct(screen);
    expect("shop tab 2", trace,
           "find:shop.lmp,tabmagicglow.tex;isin16384;tex5010,288,59,0,a4808080;find:shop.lmp,tabmagic.tex;tex500c,100,48,0,80808080;texabc0,100,0,0,80808080;");
    screen[20] = 0; drawFrameNm = 48; trace.clear();  // angle 49152: sin = -1, alpha 100 = 0x64
    shopDrawTaskBar__FP12screenStruct(screen);
    expect("shop tab 0", trace,
           "find:shop.lmp,tabweaponglow.tex;isin49152;tex5011,197,59,0,64808080;find:shop.lmp,tabweapons.tex;tex500e,100,48,0,80808080;texabc0,100,0,0,80808080;");
    // Not running: isin first, then the glow strip texture (taskBar[tab + 6]) and the base (taskBar[tab]).
    const int offs[6] = {27, 72, 118, 165, 211, 256};
    shopRunning = 0; screen[0] = -5; drawFrameNm = 0;
    for (int tab = 0; tab < 6; ++tab) {
        screen[20] = tab; trace.clear();
        shopDrawTaskBar__FP12screenStruct(screen);
        char want[200];
        std::snprintf(want, sizeof want, "isin0;tex%x,%d,59,0,84808080;tex%x,-5,48,0,80808080;", 0x7000 + (tab + 6) * 16, -5 + offs[tab], 0x7000 + tab * 16);
        expect("strip", trace, want);
    }
    std::printf(bad ? "FAIL\n" : "ok: shop draw task bar\n");
    return bad;
}
