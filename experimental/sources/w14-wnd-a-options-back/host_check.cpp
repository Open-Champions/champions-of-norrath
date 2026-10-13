// Host check only; not a build input.
#include "back.cpp"
#include <cstdio>
#include <initializer_list>

unsigned _5VIWnd$mUI = 0x1234;
static int plays, last_ui, last_id;
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound) { ++plays; last_ui = static_cast<int>(ui); last_id = sound; }

int main() {
    bool ok = true;
    for (int v : {-1, -2, 0, 1, 5, 0x7fffffff}) {
        int words[300];
        for (int& w : words) w = 0x55;
        words[286] = v;
        plays = 0;
        const int r = Back__12VIWndOptions(words);
        ok = ok && r == (v < 0 ? 1 : 0) && words[286] == (v < 0 ? v : -1) && plays == 1 && last_ui == 0x1234 && last_id == 8;
        for (int i = 0; i < 300; ++i) ok = ok && (i == 286 || words[i] == 0x55);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
