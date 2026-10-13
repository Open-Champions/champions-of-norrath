#include "load_thread_main.cpp"
#include <cstring>
#include <string>
#include <vector>

int engineDisplayDisable;
unsigned char loadingScreenRequestExit;
int drawFrameNm;
unsigned char titleloadTexCopy[64], blackoutTexCopy[64], foregrndTexCopy[128], scrollmapTexCopy[128];

static std::vector<std::string> calls;
static int frames_to_run, frames_run, title_w, title_h, black_h;
static int display_disable_at_first_call;

static std::string tex(const void* t) {
    const unsigned char* p = static_cast<const unsigned char*>(t);
    if (p == titleloadTexCopy) return "title";
    if (p == blackoutTexCopy) return "black";
    if (p >= foregrndTexCopy && p < foregrndTexCopy + 128) return "fore" + std::to_string((p - foregrndTexCopy) / 64);
    if (p >= scrollmapTexCopy && p < scrollmapTexCopy + 128) return "scroll" + std::to_string((p - scrollmapTexCopy) / 64);
    return "?";
}
extern "C" void engineFrameStart__FUib(unsigned c, int f) {
    if (calls.empty()) display_disable_at_first_call = engineDisplayDisable;
    calls.push_back("start" + std::to_string(c) + "," + std::to_string(f));
}
extern "C" void engineFrameEnd__Fb(int f) {
    calls.push_back("end" + std::to_string(f));
    if (++frames_run >= frames_to_run) loadingScreenRequestExit = 1;
}
extern "C" void engineSetDisplayDisable__Fi(int v) { calls.push_back("disable" + std::to_string(v)); }
extern "C" int texNormalizedWidth__FP8_texture(void* t) { return tex(t) == "title" ? title_w : 1; }
extern "C" int texNormalizedHeight__FP8_texture(void* t) { return tex(t) == "title" ? title_h : black_h; }
extern "C" void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int z, unsigned c) {
    calls.push_back("color " + tex(t) + " " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(c));
}
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* t, int x, int y, int z, unsigned c, int a, int b, int d, int e, char f, char g) {
    calls.push_back("super " + tex(t) + " " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(c) + "," +
                    std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(g));
}
extern "C" void textureDrawFloat__FP8_textureffiUiff(void* t, float x, float y, int z, unsigned c, float sx, float sy) {
    calls.push_back("float " + tex(t) + " " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(c) + "," +
                    std::to_string(sx) + "," + std::to_string(sy));
}

static void setShorts(unsigned char* t, short a, short b) { std::memcpy(t, &a, 2); std::memcpy(t + 2, &b, 2); }

int main() {
    const int frame_values[] = {0, 1, 100, 1519, 1520, 4700, -1, -1500, 70000};
    const short widths[] = {-200, 0, 50, 320, 700};
    for (int exit_first = 0; exit_first < 2; ++exit_first)
    for (int frame : frame_values)
    for (short fw : widths)
    for (short fh : {1, 33, 447}) {
        calls.clear(); frames_run = 0; frames_to_run = 2;
        loadingScreenRequestExit = static_cast<unsigned char>(exit_first); drawFrameNm = frame;
        engineDisplayDisable = 77; title_w = 201; title_h = 87; black_h = 301;
        setShorts(foregrndTexCopy, fw, fh);
        setShorts(foregrndTexCopy + 64, 123, 5);
        setShorts(scrollmapTexCopy, 640, 11);
        setShorts(scrollmapTexCopy + 64, 640, 20);
        loadThreadMain__FPv(nullptr);
        if (engineDisplayDisable != 4) return 1;
        if (exit_first) {
            if (calls.size() != 1 || calls[0] != "disable1") return 2;
            continue;
        }
        if (display_disable_at_first_call != 4) return 3;
        // Expected output, built independently for each frame.
        std::vector<std::string> want;
        for (int f = 0; f < 2; ++f) {
            want.push_back("start2147483648,1");
            want.push_back("color title " + std::to_string(320 - 201 / 2) + "," + std::to_string(194 - 87 / 2) + ",6,2155905152");
            want.push_back("color black 0," + std::to_string(224 - 301 / 2) + ",6,2155905152");
            want.push_back("super black 320," + std::to_string(224 - 301 / 2) + ",6,2155905152,321,0,642,447,1,0");
            for (int i = 0; i < 2; ++i) {
                int x = 2 * i * fw - frame % 1520 + 640;
                short wi = i == 0 ? fw : 123;
                if (x >= -2 * wi && x <= 640)
                    want.push_back("float fore" + std::to_string(i) + " " + std::to_string(static_cast<float>(x)) + "," +
                                   std::to_string(static_cast<float>(224 - fh / 2)) + ",6,2155905152,2.000000,0.990000");
            }
            int q = frame * 100 / 140;
            int x = -(q % 640);
            for (int k = q / 640; x < 640; ++k, x += 640) {
                int h = (k & 1) ? 20 : 11;
                want.push_back("float scroll" + std::to_string(k & 1) + " " + std::to_string(static_cast<float>(x)) + "," +
                               std::to_string(static_cast<float>(224 - h / 2)) + ",6,2155905152,2.000000,0.990000");
            }
            want.push_back("end1");
        }
        want.push_back("disable1");
        if (calls != want) return 4;
    }
    return 0;
}
