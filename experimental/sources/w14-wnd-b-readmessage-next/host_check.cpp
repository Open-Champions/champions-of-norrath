// Host check only; not a build input.
#include "next.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0;
static std::string trace;
extern "C" void SoundPlay__4VIUI11VIUISoundId(const void*, int sound) { trace += std::to_string(sound) + ";"; }

int main() {
    static int w[4300];
    bool ok = true;
    w[4199] = 20; w[4200] = 12; trace.clear();
    Next__16VIWndReadMessage(w);
    ok = ok && w[4200] == 13 && trace == "11;";
    w[4200] = 13; trace.clear();
    Next__16VIWndReadMessage(w);
    ok = ok && w[4200] == 13 && trace == "9;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
