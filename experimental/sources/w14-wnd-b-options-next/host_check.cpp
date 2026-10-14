// Host check only; not a build input.
#include "next.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0;
static std::string trace;
static unsigned flags;
extern "C" unsigned VIGetFeatureFlags__Fv() { trace += "flags;"; return flags; }
extern "C" void SoundPlay__4VIUI11VIUISoundId(const void*, int s) { trace += "snd" + std::to_string(s) + ";"; }
extern "C" void ResetTickPulse__10VIUIWindow(void*) { trace += "tick;"; }

int main() {
    static int w[400];
    bool ok = true;
    w[286] = -1; w[295] = 2; trace.clear();
    Next__12VIWndOptions(w);
    ok = ok && w[295] == 3 && trace == "tick;snd11;";
    trace.clear();
    Next__12VIWndOptions(w);
    ok = ok && w[295] == 3 && trace == "snd9;";
    w[286] = 1; w[287] = 3; flags = 0; trace.clear();
    Next__12VIWndOptions(w);
    ok = ok && w[287] == 3 && trace == "flags;snd9;";
    flags = 0x100; trace.clear();
    Next__12VIWndOptions(w);
    ok = ok && w[287] == 4 && trace == "flags;tick;snd11;";
    w[286] = 0; w[287] = 2; flags = 0; trace.clear();
    Next__12VIWndOptions(w);
    ok = ok && w[287] == 2 && trace == "flags;snd9;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
