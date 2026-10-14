// Host check only; not a build input.
#include "prev.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0;
static std::string trace;
extern "C" void SoundPlay__4VIUI11VIUISoundId(const void*, int s) { trace += "snd" + std::to_string(s) + ";"; }
extern "C" void ResetTickPulse__10VIUIWindow(void* w) { trace += "tick;"; (void)w; }

int main() {
    static int w[400];
    bool ok = true;
    w[286] = -1; w[295] = 2; w[287] = 5; trace.clear();
    Prev__12VIWndOptions(w);
    ok = ok && w[295] == 1 && w[287] == 5 && trace == "tick;snd11;";
    w[295] = 0; trace.clear();
    Prev__12VIWndOptions(w);
    ok = ok && w[295] == 0 && trace == "snd9;";
    w[286] = 1; w[287] = 3; w[295] = 9; trace.clear();
    Prev__12VIWndOptions(w);
    ok = ok && w[287] == 2 && w[295] == 9 && trace == "tick;snd11;";
    w[287] = 0; trace.clear();
    Prev__12VIWndOptions(w);
    ok = ok && w[287] == 0 && trace == "snd9;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
