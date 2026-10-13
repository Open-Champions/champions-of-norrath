// Host check only; not a build input.
#include "movecursor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
unsigned char sKeyMap[66];
static std::string trace;
extern "C" void SoundPlay__4VIUI11VIUISoundId(const void*, int s) { trace += "snd" + std::to_string(s) + ";"; }

struct Obj { alignas(8) int w[128]; };
static Obj make(int x, int y, int px, int py) {
    Obj o; std::memset(&o, 0, sizeof o);
    o.w[82] = x; o.w[83] = y; o.w[84] = px; o.w[85] = py; o.w[89] = 0x40000000;
    return o;
}

int main() {
    bool ok = true;
    std::memset(sKeyMap, 1, sizeof sKeyMap);
    { Obj o = make(3, 2, 3, 2); trace.clear(); MoveCursor__9VIWndEditii(&o, 1, 0);
      ok = ok && o.w[82] == 4 && o.w[83] == 2 && o.w[84] == 3 && o.w[85] == 2 && o.w[89] == 0 && o.w[97] == 0 && trace == "snd11;"; }
    { Obj o = make(10, 1, 10, 1); MoveCursor__9VIWndEditii(&o, 1, 0);
      ok = ok && o.w[82] == 0 && o.w[84] == 0 && o.w[97] == 0; }
    { Obj o = make(0, 0, 5, 5); MoveCursor__9VIWndEditii(&o, -1, 0);
      ok = ok && o.w[82] == 10 && o.w[84] == 10 && o.w[85] == 0 && o.w[97] == 0x3e800000; }
    { Obj o = make(4, 5, 4, 5); MoveCursor__9VIWndEditii(&o, 0, 1);
      ok = ok && o.w[83] == 0 && o.w[85] == 0 && o.w[97] == 0; }
    { Obj o = make(4, 0, 4, 0); MoveCursor__9VIWndEditii(&o, 0, -1);
      ok = ok && o.w[83] == 5 && o.w[85] == 5; }
    sKeyMap[2 + 11 * 1] = 0; sKeyMap[3 + 11 * 1] = 0;
    { Obj o = make(1, 1, 1, 1); MoveCursor__9VIWndEditii(&o, 1, 0);
      ok = ok && o.w[82] == 4 && o.w[83] == 1 && o.w[84] == 1; }
    { Obj o = make(4, 1, 4, 1); MoveCursor__9VIWndEditii(&o, -1, 0);
      ok = ok && o.w[82] == 1 && o.w[84] == 4; }
    { Obj o = make(1, 1, 1, 1); MoveCursor__9VIWndEditii(&o, 1, 1);
      ok = ok && o.w[82] == 4 && o.w[83] == 2; }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
