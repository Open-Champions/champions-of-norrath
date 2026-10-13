// Host check only; not a build input.
#include "prev.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0;
static std::string trace;
extern "C" void SoundPlay__4VIUI11VIUISoundId(const void*, int sound) { trace += std::to_string(sound) + ";"; }

int main() {
    static int w[4300];
    bool ok = true;
    w[4200] = 2; trace.clear();
    Prev__16VIWndReadMessage(w);
    ok = ok && w[4200] == 1 && trace == "11;";
    w[4200] = 0; trace.clear();
    Prev__16VIWndReadMessage(w);
    ok = ok && w[4200] == 0 && trace == "9;";
    w[4200] = -1; trace.clear();
    Prev__16VIWndReadMessage(w);
    ok = ok && w[4200] == -1 && trace == "9;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
