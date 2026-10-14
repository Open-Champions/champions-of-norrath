// Host check only; not a build input. The unit passes a 32-bit ui word through; the host build uses its low bits.
#include "select.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI = 0x1234;
static std::string trace;
static unsigned flags;
extern "C" unsigned VIGetFeatureFlags__Fv() { trace += "flags;"; return flags; }
extern "C" void SoundPlay__4VIUI11VIUISoundId(const void*, int s) { trace += "snd" + std::to_string(s) + ";"; }
extern "C" void Close__10VIUIWindowb(void*, bool f) { trace += "close" + std::to_string(f) + ";"; }
extern "C" void Open__10VIUIWindowb(void*, bool f) { trace += "open" + std::to_string(f) + ";"; }
extern "C" void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void*, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init(" + std::to_string(ui) + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," + std::to_string(s[0]) + "," +
             std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ");";
}
extern "C" void SetTitle__10VIUIWindowPCc(void*, const char* t) { trace += std::string("title(") + t + ");"; }

static int run(int section, int cursor, unsigned f, int* w) {
    std::memset(w, 0, 400 * 4);
    w[286] = section; w[287] = 9; w[295] = cursor; w[13] = 70; w[14] = 80; flags = f; trace.clear();
    return Select__12VIWndOptions(w);
}

int main() {
    static int w[400];
    bool ok = true;
    ok = ok && run(0, 1, 0, w) == 0 && trace == "snd9;" && w[287] == 9 && w[286] == 0;
    ok = ok && run(-1, 0, 0, w) == 0 && w[286] == 0 && w[287] == 0 && trace == "snd8;close1;init(4660,0,0,70,80,440,6,0);open0;title();";
    ok = ok && run(-1, 1, 0, w) == 0 && w[286] == 1 && trace == "snd8;close1;flags;init(4660,0,0,70,80,440,7,0);open0;title();";
    ok = ok && run(-1, 1, 0x200, w) == 0 && trace == "snd8;close1;flags;init(4660,0,0,70,80,440,8,0);open0;title();";
    ok = ok && run(-1, 2, 0x200, w) == 0 && w[286] == 2 && trace == "snd8;close1;init(4660,0,0,70,80,440,7,0);open0;title();";
    ok = ok && run(-1, 3, 0, w) == 1 && w[286] == -1 && w[287] == 0 && trace == "snd8;";
    ok = ok && run(-1, 4, 0, w) == 0 && trace == "snd8;" && run(-1, -2, 0, w) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
